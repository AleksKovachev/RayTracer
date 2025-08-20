#include "UI_MainWindow.h"
#include <QtWidgets/QApplication>

// TODO: Add initial camera rotation for camera animation controls.
// TODO: Make it so that the ImageViewer shows each new bucket when it's rendered.
// TODO: Implement dran n' drop for the Image viewer.


int main( int argc, char* argv[] ) {

	QApplication app( argc, argv );
	MainWindow mainWindow;
	mainWindow.show();

	return app.exec();
}
