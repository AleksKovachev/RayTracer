#include "Render.h" // Render
#include "UI_MainWindow.h"
#include "ImageViewer.h" // ImageViewer

#include <QtWidgets/QFileDialog>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QColorDialog>
#include <QtCore/qDir>
#include <QtConcurrent/QtConcurrent>
#include <Qt3DInput/Qt3DInput>

#include <vector> // vector


MainWindow::MainWindow( QWidget* parent )
	: QMainWindow( parent ), timerStart{}, timerEnd{}, execDuration{}, bgColor{} {
	setupUi( this );
	setWindowIcon( QIcon( ":/icons/icons/RayTracing.png" ) );

	// Replace the QGraphicsView with ImageViewer
	imageViewer = new ImageViewer( this );
	viewportLayout->replaceWidget( qViewport, imageViewer );
	delete qViewport;
	qViewport = nullptr;
	imageViewer->setFrameStyle( QFrame::Box | QFrame::Plain );

	qApp->installEventFilter( this );

	// Set cursor for Color button.
	colorPickerBtn->setCursor( Qt::PointingHandCursor );

	// Open file dialog when Render To button is clicked.
	connect( saveDirBtn, &QToolButton::clicked, this, [this]() {
		// Open directory selection dialog.
		QString dir = QFileDialog::getExistingDirectory(
			this,
			tr( "Select Directory" ),
			QDir::homePath(),
			QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks
		);

		// If user didn't cancel.
		if ( !dir.isEmpty() )
			saveDirEntry->setText( QDir::toNativeSeparators( dir ) );
		}
	);

	// Open file dialog when Scene File button is clicked.
	connect( sceneFileBtn, &QToolButton::clicked, this, [this]() {
		// Open directory selection dialog.
		QString file = QFileDialog::getOpenFileName(
			this,
			tr( "Select Directory" ),
			QDir::currentPath(),
			QString( "CRTScene (*.crtscene);;OBJ (*.obj)" )
		);

		// If user didn't cancel
		if ( !file.isEmpty() )
			sceneFileEntry->setText( QDir::toNativeSeparators( file ) );
		}
	);

	// Open file dialog when Open Image button is clicked.
	connect( openImgBtn, &QToolButton::clicked, this, [this]() {
		// Open directory selection dialog.
		QString file = QFileDialog::getOpenFileName(
			this,
			tr( "Select Directory" ),
			QDir::currentPath(),
			QString( "Portable Pixel Map (*.ppm)" )
		);

		// If user didn't cancel.
		if ( !file.isEmpty() ) {
			imageViewer->DisplayImage( QDir::toNativeSeparators( file ) );
			LoadDirectoryContents( QDir::toNativeSeparators( file ) );
		}
		}
	);

	// Open color picker dialog when Color Button is clicked.
	connect( colorPickerBtn, &QToolButton::clicked, this, [this]() {
		QColor color = QColorDialog::getColor( Qt::white, this, "Select Color" );
		if ( color.isValid() ) {
			// Update the preview label.
			QString btnStyle = QString( R"(
				QToolButton#colorPickerBtn {
					background-color: %1;
				}

				QToolButton#colorPickerBtn:hover {
					background-color: %2;
				}

				QToolTip#colorPickerBtn {
					background-color: black;
					color: white;
				})"
			).arg( color.name(), color.darker().name() );

			colorPickerBtn->setStyleSheet( btnStyle );

			bgColor = color;
		}}
	);

	connect( zoomResetBtn, &QPushButton::clicked, [this]() {
		imageViewer->fitInView( imageViewer->scene()->itemsBoundingRect(), Qt::KeepAspectRatio );
		} );
	connect( actionExit, &QAction::triggered, this, &QApplication::quit );
	connect( actionRender, &QAction::triggered, this, &MainWindow::OnRenderClicked );
	connect( renderBtn, &QPushButton::clicked, this, &MainWindow::OnRenderClicked );
	connect( bucketSizeSpin, &QSpinBox::valueChanged, this, &MainWindow::UpdateBucketSizeMax );
	connect( imageViewer, &ImageViewer::KeyPressed, this, &MainWindow::HandleKeyPress );
	connect( nextImgBtn, &QPushButton::clicked, this, [this]() { NavigateImages( 1 ); } );
	connect( prevImgBtn, &QPushButton::clicked, this, [this]() { NavigateImages( -1 ); } );
}

MainWindow::~MainWindow() {
	delete scene;
	delete imageViewer;
}

bool MainWindow::VerifyRenderDir() {
	// Check if export path exists and is a directory.
	QString path{ QDir::cleanPath( saveDirEntry->text().trimmed() ) };
	QStringList components{ path.split( QDir::separator(), Qt::SkipEmptyParts ) };
	QRegularExpression invalidChars( R"([<>:"/\\|?*\x00])" );

	for ( const QString& component : components ) {
		if ( component.contains( invalidChars ) ) {
			QMessageBox::warning( this, "Invalid Directory Name",
				QString( "The folder name \"%1\" contains invalid characters.\n"
					"Please enter a valid directory path." ).arg( component ) );
			return false;
		}
	}

	QFileInfo fi( path );

	if ( !fi.exists() ) {
		// Ask if to create the directory.
		int retVal = QMessageBox::question( this, "Directory Does Not Exist",
			"The directory does not exist.\n"
			"Do you want to create it?",
			QMessageBox::Yes | QMessageBox::No );

		if ( retVal == QMessageBox::Yes ) {
			QDir dir;
			if ( !dir.mkpath( path ) ) {
				QMessageBox::critical( this, "Creation Failed",
					"Failed to create the directory.\n"
					"Please check permissions or choose another path." );
				return false;
			}
		} else {
			return false;
		}
	} else if ( !fi.isDir() ) {
		// Exists but is not a directory.
		QMessageBox::warning( this, "Invalid Path",
			"The specified path exists but is not a directory." );
		return false;
	}

	return true;
}

bool MainWindow::ManageSceneSettings() {
	scene->settings.renderWidth = static_cast<unsigned>(resWidthSpin->value());
	scene->settings.renderHeight = static_cast<unsigned>(resHeightSpin->value());

	if ( !VerifyRenderDir() )
		return false;

	QString scenePath{ QDir::cleanPath( sceneFileEntry->text().trimmed() ) };

	QFileInfo sceneFile( scenePath );
	if ( !sceneFile.isFile() ) {
		QMessageBox::warning( this, "Invalid Scene File",
			"The specified scene file does not exist." );
		return false;
	}

	if ( rendNameEntry->text().trimmed().isEmpty() )
		scene->settings.SetSaveFileName( sceneFile.baseName().trimmed().toStdString() );
	else
		scene->settings.SetSaveFileName( rendNameEntry->text().trimmed().toStdString() );

	scene->settings.colorDepth = static_cast<unsigned>(colorDepthSpin->value());
	scene->settings.saveDir = saveDirEntry->text().toStdString();
	scene->SetRenderScene( sceneFileEntry->text().toStdString() );
	scene->settings.shadowBias = static_cast<float>(shadowBiasSpin->value());
	scene->settings.refractBias = static_cast<float>(refractBiasSpin->value());
	scene->settings.pathDepth = pathDepthSpin->value();
	scene->settings.renderMode = static_cast<RenderMode>(renderModeCombo->currentIndex());
	scene->settings.ignoreBackface = ignoreBackfaceBtn->isChecked();
	scene->settings.bucketSize = static_cast<unsigned>(bucketSizeSpin->value());
	scene->settings.accTreeMaxDepth = static_cast<unsigned>(accTreeMaxDepthSpin->value());
	scene->settings.maxAABBTriangleCount = static_cast<unsigned>(maxAABBTriCountSpin->value());
	scene->settings.BGColor = Color( bgColor.redF(), bgColor.greenF(), bgColor.blueF() );
	scene->settings.overrideBGColor = overrideBGColorBtn->isChecked();

	if ( sceneFile.suffix().toLower() == "crtscene" ) {
		scene->ParseSceneFile();
	} else if ( sceneFile.suffix().toLower() == "obj" ) {
		QMessageBox::warning( this, "Feature In Development",
			"The OBJ file parser is still in development and currently can't be used." );
		return false;
		//scene.ParseObjFile();
	}
	return true;
}

void MainWindow::UpdateBucketSizeMax() {
	int maxSize = std::min( resWidthSpin->value(), resHeightSpin->value() );
	if ( bucketSizeSpin->value() > maxSize )
		bucketSizeSpin->setValue( maxSize );
}

void MainWindow::OnRenderClicked() {
	scene = new Scene(); // Reset scene.

	// Disable render buttons while rendering.
	renderBtn->setEnabled( false );
	actionRender->setEnabled( false );

	if ( !ManageSceneSettings() )
		return;

	Render render( *scene );

	// Run render asynchronously.
	auto future = QtConcurrent::run( [this, render]() mutable {
		timerStart = std::chrono::high_resolution_clock::now();

		//render.RenderImage();
		render.RenderBuckets();

		timerEnd = std::chrono::high_resolution_clock::now();

		std::chrono::microseconds duration{
			std::chrono::duration_cast<std::chrono::microseconds>(timerEnd - timerStart) };
		execDuration = duration.count() / 1'000'000.0;
		} );

	auto watcher = new QFutureWatcher<void>( this );

	connect( watcher, &QFutureWatcher<void>::finished, this, [this, watcher]() {
		QString message( "Rendering completed in: "
			+ QString::number( execDuration ) + " seconds." );
		statusbar->showMessage( message, 7000 );
		renderBtn->setEnabled( true );
		actionRender->setEnabled( true );
		QString imagePath = QString::fromStdString(
			scene->settings.saveDir + "/" + scene->settings.GetSaveFileName() + ".ppm" );
		LoadDirectoryContents( imagePath );
		imageViewer->DisplayImage( imagePath );
		watcher->deleteLater();
		delete scene;
		} );
	watcher->setFuture( future );
}

void MainWindow::HandleKeyPress( int key ) {
	switch ( key ) {
		case Qt::Key_Comma:
			NavigateImages( -1 );
			break;
		case Qt::Key_Period:
			NavigateImages( 1 );
			break;
	}
}

void MainWindow::NavigateImages( int offset ) {
	if ( imageFiles.isEmpty() )
		return;

	// Set the index so that it wraps around when it gets to the end of the list.
	currentImageIndex = (currentImageIndex + offset + imageFiles.size()) % imageFiles.size();

	QString newPath = currentDirectory + "/" + imageFiles[currentImageIndex];
	imageViewer->DisplayImage( newPath );

	// Update status bar.
	statusbar->showMessage( imageFiles[currentImageIndex], 5000 );
}

void MainWindow::LoadDirectoryContents( const QString& imagePath ) {
	QFileInfo fileInfo( imagePath );
	currentDirectory = fileInfo.absolutePath();

	// Get all supported image formats in directory.
	QDir dir( currentDirectory );
	QStringList filters( "*.ppm" );

	imageFiles = dir.entryList( filters, QDir::Files, QDir::Name );
	currentImageIndex = imageFiles.indexOf( fileInfo.fileName() );
}

bool MainWindow::eventFilter( QObject* watched, QEvent* event ) {
	if ( event->type() == QEvent::KeyPress ) {
		QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);

		// Check if the event comes from one a widget that needs the used keys.
		QWidget* focusedWidget = qApp->focusWidget();
		std::vector<QWidget*> excludedWidgets = {
			saveDirEntry, sceneFileEntry, rendNameEntry, shadowBiasSpin, refractBiasSpin
		};

		bool isExcludedWidget{ false };
		for ( const QWidget* exclWidget : excludedWidgets ) {
			if ( focusedWidget == exclWidget || focusedWidget == exclWidget->focusProxy() ) {
				isExcludedWidget = true;
				break;
			}
		}

		if ( !isExcludedWidget ) {
			if ( keyEvent->key() == Qt::Key_Comma || keyEvent->text() == "," ) {
				NavigateImages( -1 );
				return true;
			} else if ( keyEvent->key() == Qt::Key_Period || keyEvent->text() == "." ) {
				NavigateImages( 1 );
				return true;
			}
		}
	}
	return QMainWindow::eventFilter( watched, event );
}
