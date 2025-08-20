#ifndef CUSTOMWIDGETS_H
#define CUSTOMWIDGETS_H

#include <QDoubleSpinBox>
#include <QWidget>
#include <QGraphicsView>
#include <QFileSystemWatcher>


class ImageViewer : public QGraphicsView {
	Q_OBJECT
public:
	QSize currentImageSize;
	float zoomFactor = 1.f;
	explicit ImageViewer( QWidget* parent = nullptr );
	void DisplayImage( const QString& path );
	const QString GetCurrentImagePath() const;
	const QString GetCurrentImagePathDirname() const;

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
	const float zoomIncrement = 0.1f; // 10% zoom per wheel step
};


class SmartDoubleSpinBox : public QDoubleSpinBox {
	Q_OBJECT
public:
	explicit SmartDoubleSpinBox( QWidget* parent = nullptr ) : QDoubleSpinBox( parent ) {}

	QString textFromValue( double value ) const override {
		// Show minimal decimal places needed, but allow full precision
		QString text = QDoubleSpinBox::textFromValue( value );

		// Trim trailing zeros and optional decimal point
		if ( text.contains( '.' ) ) {
			while ( text.endsWith( '0' ) ) {
				text.chop( 1 );
			}
			if ( text.endsWith( '.' ) ) {
				text.chop( 1 );
			}
		}
		return text;
	}
};

#endif // CUSTOMWIDGETS_H