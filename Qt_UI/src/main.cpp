#include "UI_MainWindow.h"
#include <QtWidgets/QApplication>

int main( int argc, char* argv[] ) {

	QApplication app( argc, argv );
	MainWindow mainWindow;
	mainWindow.show();

	return app.exec();
}