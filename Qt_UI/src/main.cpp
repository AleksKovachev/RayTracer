#include "UI_MainWindow.h"
#include <QtWidgets/QApplication>

// TODO: Add initial camera rotation for camera animation controls.

int main( int argc, char* argv[] ) {

	QApplication app( argc, argv );
	MainWindow mainWindow;
	mainWindow.show();

	return app.exec();
}