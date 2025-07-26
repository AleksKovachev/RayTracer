#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H

#include <QWidget>
#include <QGraphicsView>
#include <QFileSystemWatcher>


class ImageViewer : public QGraphicsView {
	Q_OBJECT
public:
	explicit ImageViewer( QWidget* parent = nullptr );
	void DisplayImage( const QString& path );

protected:
	void wheelEvent( QWheelEvent* event ) override;
	void mouseDoubleClickEvent( QMouseEvent* event ) override;
	void keyPressEvent( QKeyEvent* event ) override;

signals:
	void ImageUpdated();
	void KeyPressed( int key );

private slots:
	void ReloadImage();

private:
	QGraphicsScene* qScene;
	QFileSystemWatcher* qFileWatcher;
	QString currentImagePath;
	float zoomFactor = 1.f;
	const float zoomIncrement = 0.1f; // 10% zoom per wheel step
};

#endif // IMAGEVIEWER_H