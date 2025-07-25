#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtWidgets/QMainWindow>

#include "UI_Main.h"

class MainWindow : public QMainWindow, public Ui::RayTracer {
	Q_OBJECT
public:
	explicit MainWindow( QWidget* parent = nullptr )
		: QMainWindow( parent ) {
		setupUi( this );
	}
	~MainWindow() override = default;
};

#endif // UI_MAINWINDOW_H