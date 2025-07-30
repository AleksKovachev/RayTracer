#include "Render.h" // Render
#include "UI_MainWindow.h"
#include "CustomWidgets.h" // ImageViewer

#include <QtWidgets/QAbstractItemView>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QColorDialog>
#include <QtCore/qDir>
#include <QtConcurrent/QtConcurrent>
#include <Qt3DInput/Qt3DInput>

#include <format> // format
#include <vector> // vector


MainWindow::MainWindow( QWidget* parent )
	: QMainWindow( parent ), timerStart{}, timerEnd{}, execDuration{}, bgColor{} {
	setupUi( this );
	setWindowIcon( QIcon( ":/icons/icons/RayTracing.png" ) );

	// Install event filter to handle Image Viewer next/previous shortcuts.
	qApp->installEventFilter( this );

	// Set this Combo Box' dropdown menu wide enough to fit longest entry name.
	renderModeCombo->view()->setMinimumWidth( 150 );

	cameraBox->setFixedWidth( 225 );

	// Set cursor style for Color button.
	colorPickerBtn->setCursor( Qt::PointingHandCursor );

	SetupCameraWidgets();

	connect( custWidthBtn, &QPushButton::toggled, this, [this]() {
		resWidthSpin->setEnabled( custWidthBtn->isChecked() ); } );
	connect( custHeightBtn, &QPushButton::toggled, this, [this]() {
		resHeightSpin->setEnabled( custHeightBtn->isChecked() ); } );
	connect( custBucketSizeBtn, &QPushButton::toggled, this, [this]() {
		bucketSizeSpin->setEnabled( custBucketSizeBtn->isChecked() ); } );
	connect( saveDirBtn, &QToolButton::clicked, this, &MainWindow::OpenRenderToBtnClicked );
	connect( sceneFileBtn, &QToolButton::clicked, this, &MainWindow::OpenSceneBtnClicked );
	connect( openImgBtn, &QToolButton::clicked, this, &MainWindow::OpenImageBtnClicked );
	connect( colorPickerBtn, &QToolButton::clicked, this, &MainWindow::SetupColorPicker );
	connect( zoomResetBtn, &QPushButton::clicked, [this]() {
		qViewport->fitInView( qViewport->scene()->itemsBoundingRect(), Qt::KeepAspectRatio ); }
	);
	connect( actionExit, &QAction::triggered, this, &QApplication::quit );
	connect( actionRender, &QAction::triggered, this, &MainWindow::OnRenderClicked );
	connect( renderBtn, &QPushButton::clicked, this, &MainWindow::OnRenderClicked );
	connect( bucketSizeSpin, &QSpinBox::valueChanged, this, &MainWindow::UpdateBucketSizeMax );
	connect( qViewport, &ImageViewer::KeyPressed, this, &MainWindow::HandleKeyPress );
	connect( nextImgBtn, &QPushButton::clicked, this, [this]() { NavigateImages( 1 ); } );
	connect( prevImgBtn, &QPushButton::clicked, this, [this]() { NavigateImages( -1 ); } );
	connect( overrideBGColorBtn, &QPushButton::toggled, this, [this]() {
		colorPickerBtn->setVisible( overrideBGColorBtn->isChecked() ); } );
	connect( camActionCombo, &QComboBox::currentTextChanged, this,
		&MainWindow::OnCameraActionChanged );
	connect( sceneInitialCamPosBtn, &QPushButton::clicked, this,
		&MainWindow::HandleSceneCameraPositionButtonClicked );

	connect( this, &MainWindow::FrameRendered, this, [this]( int frame, QString* imagePath ) {
		statusBar()->showMessage( tr( "Rendering frame %1" ).arg( frame ) );
		qViewport->DisplayImage( *imagePath );
		delete imagePath;
		} );
	connect( enableGIBtn, &QPushButton::clicked, this, [this]() {
		giSamplesLabel->setEnabled( enableGIBtn->isChecked() ); } );

	colorPickerBtn->hide();
}

MainWindow::~MainWindow() {
	// Just a good practice. All will be deleted with the parent (this).
	if ( scene != nullptr )
		delete scene;
}

void MainWindow::OnRenderClicked() {
	scene = new Scene(); // Reset scene.

	if ( !ManageSceneMainSettings() )
		return;

	Render* render = new Render( *scene );

	FVector3 groundCenter{};
	FVector3 actionCoords{};
	Camera* camera = nullptr;
	std::string* saveName = nullptr;

	const QString& enumText = camActionCombo->currentText();
	if ( enumText == "Move Animation" || enumText == "Orbit Animation" ) {
		FVector3 initialPos{};
		render->frames = frameNumSpin->value();
		SetupAnimationCoordinates( initialPos, actionCoords );

		camera = new Camera();
		render->overrideCamera = camera; // camera deleted with render.
		camera->Move( initialPos );
		camera->SetOrientation( render->GetScene().GetCamera().GetOrientationMatrix() );
		saveName = new std::string;

		FVector3 sceneCenter{ (scene->GetAABB().max + scene->GetAABB().min) / 2 };
		groundCenter = { sceneCenter.x, scene->GetAABB().min.y, sceneCenter.z };
	}

	// Disable render buttons while rendering.
	renderBtn->setEnabled( false );
	actionRender->setEnabled( false );

	// Run render asynchronously.
	QFuture future = QtConcurrent::run( [=, this]() mutable {
		RunRender( render, saveName, camera, groundCenter, actionCoords ); } );

	// Prepare actions for when the render is finished.
	QFutureWatcher<void>* watcher = new QFutureWatcher<void>( this );
	connect( watcher, &QFutureWatcher<void>::finished, this, [this, watcher, render]() {
		SetupPostRenderWatcher( watcher, render ); } );
	watcher->setFuture( future );
}

void MainWindow::UpdateBucketSizeMax() {
	int maxSize = std::min( resWidthSpin->value(), resHeightSpin->value() );
	if ( bucketSizeSpin->value() > maxSize )
		bucketSizeSpin->setValue( maxSize );
}

void MainWindow::HandleKeyPress( int key ) {
	switch ( key ) {
		case prevImgShortcut:
			NavigateImages( -1 );
			break;
		case nextImgShortcut:
			NavigateImages( 1 );
			break;
	}
}

void MainWindow::OpenImageBtnClicked() {
	// Open directory selection dialog.
	QString file = QFileDialog::getOpenFileName(
		this,
		tr( "Select Directory" ),
		QDir::currentPath(),
		QString( "Portable Pixel Map (*.ppm)" )
	);

	// If user didn't cancel.
	if ( !file.isEmpty() ) {
		qViewport->DisplayImage( QDir::toNativeSeparators( file ) );
		LoadDirectoryContents( QDir::toNativeSeparators( file ) );
	}
}

void MainWindow::OpenSceneBtnClicked() {
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

void MainWindow::OpenRenderToBtnClicked() {
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

void MainWindow::OnCameraActionChanged() {
	QGridLayout* cameraBoxLayout = qobject_cast<QGridLayout*>(cameraBox->layout());

	if ( camActionCombo->currentText() == "Static Image" ) {
		// Remove all animation controls if static render is chosen.
		for ( QWidget* widget : cameraWidgets ) {
			cameraBox->layout()->removeWidget( widget );
			widget->hide();
		}
	} else {
		// Place all animation controls if an animation mode is chosen.
		for ( QWidget* widget : cameraWidgets ) {
			cameraBox->layout()->removeWidget( widget );
			widget->show();
		}
		cameraBoxLayout->addWidget( camInitialPosLabel, 1, 0, 1, 5 );
		cameraBoxLayout->addWidget(
			sceneInitialCamPosBtn, 1, 3, 1, 3, Qt::AlignRight );
		cameraBoxLayout->addWidget( xInitialLabel, 2, 0 );
		cameraBoxLayout->addWidget( camXInitialSpin, 2, 1 );
		cameraBoxLayout->addWidget( yInitialLabel, 2, 2 );
		cameraBoxLayout->addWidget( camYInitialSpin, 2, 3 );
		cameraBoxLayout->addWidget( zInitialLabel, 2, 4 );
		cameraBoxLayout->addWidget( camZInitialSpin, 2, 5 );
		cameraBoxLayout->addWidget( camActionLabel, 3, 0, 1, 5 );
		cameraBoxLayout->addWidget( xActionLabel, 4, 0 );
		cameraBoxLayout->addWidget( camXActionSpin, 4, 1 );
		cameraBoxLayout->addWidget( yActionLabel, 4, 2 );
		cameraBoxLayout->addWidget( camYActionSpin, 4, 3 );
		cameraBoxLayout->addWidget( zActionLabel, 4, 4 );
		cameraBoxLayout->addWidget( camZActionSpin, 4, 5 );
		cameraBoxLayout->addWidget( frameNumLabel, 5, 0, 1, 3 );
		cameraBoxLayout->addWidget( frameNumSpin, 5, 3 );
	}

	// Reset the camera action spin boxes to zero.
	camXActionSpin->setValue( 0.0 );
	camYActionSpin->setValue( 0.0 );
	camZActionSpin->setValue( 0.0 );

	// Change the label text based on the selected action.
	if ( camActionCombo->currentText() == "Move Animation" ) {
		camActionLabel->setText( "Move Offset" );

		QString camActionLabelMsg = QCoreApplication::translate(
			"RayTracer",
			"The distance the camera should move on each frame and on which ases.",
			nullptr );
		camActionLabel->setToolTip( camActionLabelMsg );
		camActionLabel->setStatusTip( camActionLabelMsg );

		QString camXActionMsg = QCoreApplication::translate(
			"RayTracer", "The camera offset on the X axis.", nullptr );
		xActionLabel->setToolTip( camXActionMsg );
		xActionLabel->setStatusTip( camXActionMsg );
		camXActionSpin->setToolTip( camXActionMsg );
		camXActionSpin->setStatusTip( camXActionMsg );
		camXActionSpin->setSuffix(
			QCoreApplication::translate( "RayTracer", "", nullptr ) );

		QString camYActionMsg = QCoreApplication::translate(
			"RayTracer", "The camera offset on the Y axis.", nullptr );
		yActionLabel->setToolTip( camYActionMsg );
		yActionLabel->setStatusTip( camYActionMsg );
		camYActionSpin->setToolTip( camYActionMsg );
		camYActionSpin->setStatusTip( camYActionMsg );
		camYActionSpin->setSuffix(
			QCoreApplication::translate( "RayTracer", "", nullptr ) );

		QString camZActionMsg = QCoreApplication::translate(
			"RayTracer", "The camera offset on the Z axis.", nullptr );
		zActionLabel->setToolTip( camZActionMsg );
		zActionLabel->setStatusTip( camZActionMsg );
		camZActionSpin->setToolTip( camZActionMsg );
		camZActionSpin->setStatusTip( camZActionMsg );
		camZActionSpin->setSuffix(
			QCoreApplication::translate( "RayTracer", "", nullptr ) );
	} else if ( camActionCombo->currentText() == "Orbit Animation" ) {
		camActionLabel->setText( "Rotation Offset in degrees" );

		QString camActionLabelMsg = QCoreApplication::translate(
			"RayTracer",
			"The degrees the camera should rotate on each frame and on which ases.",
			nullptr );
		camActionLabel->setToolTip( camActionLabelMsg );
		camActionLabel->setStatusTip( camActionLabelMsg );

		QString camXActionMsg = QCoreApplication::translate(
			"RayTracer", "The camera angle offset on the X axis.", nullptr );
		xActionLabel->setToolTip( camXActionMsg );
		xActionLabel->setStatusTip( camXActionMsg );
		camXActionSpin->setToolTip( camXActionMsg );
		camXActionSpin->setStatusTip( camXActionMsg );
		camXActionSpin->setSuffix(
			QCoreApplication::translate( "RayTracer", "\302\260", nullptr ) );

		QString camYActionMsg = QCoreApplication::translate(
			"RayTracer", "The camera angle offset on the Y axis.", nullptr );
		yActionLabel->setToolTip( camYActionMsg );
		yActionLabel->setStatusTip( camYActionMsg );
		camYActionSpin->setToolTip( camYActionMsg );
		camYActionSpin->setStatusTip( camYActionMsg );
		camYActionSpin->setSuffix(
			QCoreApplication::translate( "RayTracer", "\302\260", nullptr ) );

		QString camZActionMsg = QCoreApplication::translate(
			"RayTracer", "The camera angle offset on the Z axis.", nullptr );
		zActionLabel->setToolTip( camZActionMsg );
		zActionLabel->setStatusTip( camZActionMsg );
		camZActionSpin->setToolTip( camZActionMsg );
		camZActionSpin->setStatusTip( camZActionMsg );
		camZActionSpin->setSuffix(
			QCoreApplication::translate( "RayTracer", "\302\260", nullptr ) );
	}
}

void MainWindow::HandleSceneCameraPositionButtonClicked() {
	bool isBtnClicked{ sceneInitialCamPosBtn->isChecked() };
	xInitialLabel->setDisabled( isBtnClicked );
	camXInitialSpin->setDisabled( isBtnClicked );
	yInitialLabel->setDisabled( isBtnClicked );
	camYInitialSpin->setDisabled( isBtnClicked );
	zInitialLabel->setDisabled( isBtnClicked );
	camZInitialSpin->setDisabled( isBtnClicked );

}

void MainWindow::SetupPostRenderWatcher(
	QFutureWatcher<void>* const watcher, const Render* render ) {
	QString message( "Rendering completed in: "
		+ QString::number( execDuration ) + " seconds." );

	statusbar->showMessage( message, 7000 );

	// Enable render buttons.
	renderBtn->setEnabled( true );
	actionRender->setEnabled( true );

	// Define the name and path of the rendered image.
	std::string suffix{};
	if ( camActionCombo->currentText() == "Move Animation" )
		suffix = "_move_00000";
	else if ( camActionCombo->currentText() == "Orbit Animation" )
		suffix = "_orbit_00000";

	const std::string SaveName = scene->settings.GetSaveFileName() + suffix;

	QString imagePath = QString::fromStdString(
		scene->settings.saveDir + "/" + SaveName + ".ppm" );

	// Collect all images in the directory and display the rendered image.
	LoadDirectoryContents( imagePath );
	qViewport->DisplayImage( imagePath );

	watcher->deleteLater();
	delete scene;
	scene = nullptr;
	delete render;
	render = nullptr;
}

void MainWindow::RunRender(
	Render* const render,
	std::string* const saveName,
	Camera* const camera,
	const FVector3 pivot,
	const FVector3 actionCoords
) {
	timerStart = std::chrono::high_resolution_clock::now();

	const QString& enumText = camActionCombo->currentText();
	if ( enumText == "Static Image" )
		//render.RenderImage();
		render->RenderBuckets();
	else if ( enumText == "Move Animation" || enumText == "Orbit Animation" ) {
		std::string suffix{ enumText == "Move Animation" ? "_move" : "_orbit" };

		for ( int frame{}; frame < render->frames; ++frame ) {
			// Manage frame number padding.
			std::string frameDisplay = std::format( "_{:05}", frame );

			// Construct the name for the rendered file.
			*saveName = render->GetScene().settings.GetSaveFileName()
				+ suffix + frameDisplay;
			render->overrideSaveName = saveName; // saveName deleted with render.

			//render->RenderImage();
			render->RenderBuckets();

			if ( enumText == "Move Animation" )
				camera->Move( actionCoords );
			else if ( enumText == "Orbit Animation" )
				camera->RotateAroundPoint( pivot, actionCoords );

			// Construct the path to the rendered file.
			QString* filePath = new QString();
			*filePath = QString::fromStdString(
				render->GetScene().settings.saveDir + "/" + (*saveName) + ".ppm"
			);

			// Report progress after each rendered frame.
			QMetaObject::invokeMethod( this, [this, frame, filePath]() {
				emit FrameRendered( frame, filePath ); }, Qt::QueuedConnection );
		}
	}

	timerEnd = std::chrono::high_resolution_clock::now();

	std::chrono::microseconds duration{
		std::chrono::duration_cast<std::chrono::microseconds>(timerEnd - timerStart) };
	execDuration = duration.count() / 1'000'000.0;
}

void MainWindow::SetupColorPicker() {
	// Open color picker dialog when Color Button is clicked.
	QColor color = QColorDialog::getColor( Qt::white, this, "Select Color" );
	QString hoverColor;
	if ( color.valueF() > 0.5f )
		hoverColor = color.darker().name();
	else if ( color.valueF() > 0.1f )
		hoverColor = color.lighter().name();
	else
		hoverColor = "#222222";

	if ( !color.isValid() )
		return;

	// Update the preview label.
	QString btnStyle = QString( R"(
		QToolButton {
			background-color: %1;
			border-radius: 6px;
		}

		QToolButton:hover {
			background-color: %2;
			border: 1px;
			border-radius: 6px;
		}

		QToolTip {
			background-color: palette(tooltip-base);
			color: palette(tooltip-text);
		})"
	).arg( color.name(), hoverColor );

	colorPickerBtn->setStyleSheet( btnStyle );

	bgColor = color;
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

void MainWindow::NavigateImages( int offset ) {
	if ( imageFiles.isEmpty() )
		return;

	// Set the index so that it wraps around when it gets to the end of the list.
	currentImageIndex = (currentImageIndex + offset + imageFiles.size()) % imageFiles.size();

	QString newPath = currentDirectory + "/" + imageFiles[currentImageIndex];
	qViewport->DisplayImage( newPath );

	// Update status bar.
	statusbar->showMessage( imageFiles[currentImageIndex], 5000 );
}

void MainWindow::SetupCameraWidgets() {
	// Collect all animation-related widgets.
	cameraWidgets.insert( cameraWidgets.end(), {
		camInitialPosLabel, xInitialLabel, sceneInitialCamPosBtn, camXInitialSpin,
		yInitialLabel, camYInitialSpin, zInitialLabel, camZInitialSpin,
		camActionLabel, xActionLabel, camXActionSpin, yActionLabel, camYActionSpin,
		zActionLabel, camZActionSpin, frameNumLabel, frameNumSpin
		} );

	if ( camActionCombo->currentText() == "Static Image" ) {
		// Remove them from the screen.
		for ( QWidget* widget : cameraWidgets ) {
			cameraBox->layout()->removeWidget( widget );
			widget->hide();
		}
	}
}

void MainWindow::SetupAnimationCoordinates( FVector3& initialPos, FVector3& actionCoords ) {
	if ( sceneInitialCamPosBtn->isChecked() )
		// Use the position of the camera read from the scene file
		initialPos = scene->GetCamera().GetLocation();
	else
		// Use the position chosen in the UI.
		initialPos = {
			static_cast<float>(camXInitialSpin->value()),
			static_cast<float>(camYInitialSpin->value()),
			static_cast<float>(camZInitialSpin->value()) };
		// TODO: Add initial rotation.

	actionCoords = {
		static_cast<float>(camXActionSpin->value()),
		static_cast<float>(camYActionSpin->value()),
		static_cast<float>(camZActionSpin->value()) };
}

bool MainWindow::eventFilter( QObject* obj, QEvent* event ) {
	// If the event is mouse wheel and widget is any of the below -> transfer
	// mouse wheel event to the scroll area widget.
	if ( event->type() == QEvent::Wheel
		&& (obj->inherits( "QSpinBox" )
			|| obj->inherits( "QDoubleSpinBox" )
			|| obj->inherits( "QComboBox" ))
		) {
		QApplication::sendEvent( settingsScrollArea->viewport(), event );
		return true;
	}

	if ( event->type() == QEvent::KeyPress ) {
		QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);

		// Check if the event comes from a widget that needs the shortcut keys.
		QWidget* focusedWidget = qApp->focusWidget();
		bool isExcludedWidget{ false };
		if ( focusedWidget )
			isExcludedWidget = focusedWidget->inherits( "QLineEdit" ) ||
			focusedWidget->inherits( "QDoubleSpinBox" );

	// Change the image if the widget doesn't need the shortcut keys.
		if ( !isExcludedWidget ) {
			if ( keyEvent->key() == prevImgShortcut ) {
				NavigateImages( -1 );
				return true;
			} else if ( keyEvent->key() == nextImgShortcut ) {
				NavigateImages( 1 );
				return true;
			}
		}
	}
	return QMainWindow::eventFilter( obj, event );
}

bool MainWindow::VerifyRenderDir() {
	// Check if export path exists and is a directory.
	QString path{ QDir::cleanPath( saveDirEntry->text().trimmed() ) };

	// Split path into components to check for invalid characters.
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
		// Ask to create the directory if it doesn't exist.
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

bool MainWindow::ManageSceneMainSettings() {
	scene->settings.colorDepth = static_cast<unsigned>(colorDepthSpin->value());
	scene->settings.BGColor = Color( bgColor.redF(), bgColor.greenF(), bgColor.blueF() );
	scene->settings.overrideBGColor = overrideBGColorBtn->isChecked();

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

	scene->settings.saveDir = saveDirEntry->text().toStdString();
	scene->SetRenderScene( sceneFileEntry->text().toStdString() );
	scene->settings.shadowBias = static_cast<float>(shadowBiasSpin->value());
	scene->settings.refractBias = static_cast<float>(refractBiasSpin->value());
	scene->settings.pathDepth = pathDepthSpin->value();
	scene->settings.renderMode = static_cast<RenderMode>(renderModeCombo->currentIndex());
	scene->settings.ignoreBackface = ignoreBackfaceBtn->isChecked();
	scene->settings.accTreeMaxDepth = static_cast<unsigned>(accTreeMaxDepthSpin->value());
	scene->settings.maxAABBTriangleCount = static_cast<unsigned>(maxAABBTriCountSpin->value());
	scene->settings.outputSRGB = sRGBBtn->isChecked();
	scene->settings.antialiasing = static_cast<Antialiasing>(antialiasCombo->currentIndex());

	if ( sceneFile.suffix().toLower() == "crtscene" ) {
		scene->ParseSceneFile();
	} else if ( sceneFile.suffix().toLower() == "obj" ) {
		QMessageBox::warning( this, "Feature In Development",
			"The OBJ file parser is still in development and currently can't be used." );
		return false;
		//scene.ParseObjFile();
	}

	if ( custWidthBtn->isChecked() )
		scene->settings.renderWidth = static_cast<unsigned>(resWidthSpin->value());
	if ( custHeightBtn->isChecked() )
		scene->settings.renderHeight = static_cast<unsigned>(resHeightSpin->value());
	if ( custBucketSizeBtn->isChecked() )
		scene->settings.bucketSize = static_cast<unsigned>(bucketSizeSpin->value());

	return true;
}
