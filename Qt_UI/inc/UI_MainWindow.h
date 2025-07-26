#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtWidgets/QMainWindow>

#include <chrono> // high_resolution_clock, microseconds

#include "Scene.h"
#include "UI_Main.h"


class MainWindow : public QMainWindow, public Ui::RayTracer {
	Q_OBJECT
public:
	explicit MainWindow( QWidget* parent = nullptr );
	~MainWindow() override;

	bool VerifyRenderDir();
	bool ManageSceneSettings();

	Scene* scene = nullptr;
private slots:
	void OnRenderClicked();
	void UpdateBucketSizeMax();
	void HandleKeyPress( int key );
private:
	std::chrono::high_resolution_clock::time_point timerStart;
	std::chrono::high_resolution_clock::time_point timerEnd;
	double execDuration;
	QColor bgColor;
	class ImageViewer* imageViewer = nullptr;
	QStringList imageFiles;
	int currentImageIndex = -1;
	QString currentDirectory;

	void LoadDirectoryContents( const QString& imagePath );
	void NavigateImages( int offset );
	bool eventFilter( QObject* watched, QEvent* event ) override;
};

#endif // UI_MAINWINDOW_H