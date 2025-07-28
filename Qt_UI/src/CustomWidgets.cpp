#include "CustomWidgets.h"

#include <QGraphicsPixmapItem>
#include <QFileInfo>
#include <Qt3DInput/Qt3DInput>


ImageViewer::ImageViewer( QWidget* parent )
	: QGraphicsView( parent ), qScene( new QGraphicsScene( this ) ) {
	setScene( qScene );
	setRenderHint( QPainter::Antialiasing );
	setDragMode( QGraphicsView::ScrollHandDrag );

	qFileWatcher = new QFileSystemWatcher( this );
	connect( qFileWatcher, &QFileSystemWatcher::fileChanged,
		this, &ImageViewer::ReloadImage );
}

void ImageViewer::DisplayImage( const QString& path ) {
	if ( currentImagePath == path )
		return;

	if ( !currentImagePath.isEmpty() ) {
		qFileWatcher->removePath( currentImagePath );
	}

	currentImagePath = path;
	qFileWatcher->addPath( path );
	ReloadImage();
}

void ImageViewer::wheelEvent( QWheelEvent* event ) {
	if ( scene()->items().isEmpty() ) {
		QGraphicsView::wheelEvent( event );
		return;
	}

	// Get mouse position before zoom
	QPointF oldPos = mapToScene( event->position().toPoint() );

	// Determine zoom direction
	float factor = (event->angleDelta().y() > 0)
		? (1.f + zoomIncrement) : (1.f - zoomIncrement);

	// Limit zoom range
	float newZoom = zoomFactor * factor;
	float minZoom = 0.1f;
	float maxZoom = 20.f;
	if ( newZoom < minZoom || newZoom > maxZoom ) {
		return;
	}
	zoomFactor = newZoom;

	// Perform the zoom
	scale( factor, factor );

	// Get mouse position after zoom
	QPointF newPos = mapToScene( event->position().toPoint() );

	// Adjust scrollbars to keep mouse position stable
	QPointF delta = newPos - oldPos;
	translate( delta.x(), delta.y() );

	event->accept();
}

void ImageViewer::ReloadImage() {
	qScene->clear();

	QFileInfo fileInfo( currentImagePath );
	if ( !fileInfo.exists() ) {
		qWarning() << "Image file not found:" << currentImagePath;
		return;
	}

	QPixmap pixmap( currentImagePath );
	if ( pixmap.isNull() ) {
		qWarning() << "Failed to load image:" << currentImagePath;
		return;
	}

	qScene->addItem( new QGraphicsPixmapItem( pixmap ) );
	setSceneRect( pixmap.rect() );
	fitInView( qScene->itemsBoundingRect(), Qt::KeepAspectRatio );

	// Re-add the path in case it was overwritten (which removes it from watcher)
	if ( !qFileWatcher->files().contains( currentImagePath ) ) {
		qFileWatcher->addPath( currentImagePath );
	}

	emit ImageUpdated();
}

void ImageViewer::mouseDoubleClickEvent( QMouseEvent* event ) {
	Q_UNUSED( event );
	fitInView( scene()->itemsBoundingRect(), Qt::KeepAspectRatio );
	zoomFactor = 1.f;
}

void ImageViewer::keyPressEvent( QKeyEvent* event ) {
	emit KeyPressed( event->key() );  // Forward to Main Window
	QGraphicsView::keyPressEvent( event );
}