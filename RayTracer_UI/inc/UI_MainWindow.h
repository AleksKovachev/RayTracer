#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtWidgets/QMainWindow>

#include <chrono> // high_resolution_clock, microseconds

#include "RenderType.h" // RenderType
#include "Scene.h"
#include "UI_Main.h"

template <typename T>
class QFutureWatcher;
class Render;

class MainWindow : public QMainWindow, public Ui::RayTracer {
	Q_OBJECT
public:
	explicit MainWindow( QWidget* parent = nullptr );
	~MainWindow() override;

	// Verifies the output directory and creates it if needed.
	// @return True if the directory is valid, false otherwise.
	bool VerifyRenderDir();

	// Gets scene data from UI and fills in the scene object. Initiates scene parsing.
	// @return True if the scene was successfully filled, false otherwise.
	bool ManageSceneMainSettings();

	Scene* scene = nullptr;
private slots:
	// Creates a new scene and prepares it for rendering. Calls rendering concurrently.
	void OnRenderClicked();

	// Updates bucket size to fit into chosen resolution.
	void UpdateBucketSizeMax();

	// A handler for the Image Viewer shortcuts.
	void HandleKeyPress( int key );

	// Open file dialog when "Open Image" button is clicked.
	void OpenImageBtnClicked();

	// Open file dialog when "Scene File" button is clicked.
	void OpenSceneBtnClicked();

	// Open file dialog when "Render To" button is clicked.
	void OpenRenderToBtnClicked();

	// Manages camera animation widgets placement, removal, visibility, and cosmetics.
	void OnCameraActionChanged();

	// Enables/Disables camera initial position widgets.
	void HandleSceneCameraPositionButtonClicked();

	// Updates UI and manages pointers after a render is finished.
	// @param[in-out] watcher: The future watcher used to track the render.
	// @param[in] render: The render object used to render the scene.
	void SetupPostRenderWatcher( QFutureWatcher<void>*, const Render* );
	
	// Runs the render process, static or animation, and takes render time.
	// @param[in-out] render: The render object used to render the scene.
	// @param[in-out] fileName: The name of the file to save the rendered image to.
	// @param[in-out] camera: The camera object used to render the scene.
	// @param[in] pivot: The point that the camera will orbit around.
	// @param[in] actionCoords: The relative coordinates / angles for the move / rotation.
	void RunRender(
		Render* const, std::string* const, Camera* const, const FVector3, const FVector3 );

	// Handles the antialiasing type selection and sets up the UI accordingly.
	void HandleAntialias();

	// Applies FXAA to the currently opened image in the viewport.
	void ApplyFXAAToOpenedImage();
signals:
	// Emitted when a render of a frame is finished when rendering animation.
	// @param[in] frame: The frame number that was rendered.
	// @param[in-out] imagePath: The absolute path to the rendered image.
	void FrameRendered( int, QString* );
private:
	std::chrono::high_resolution_clock::time_point timerStart;
	std::chrono::high_resolution_clock::time_point timerEnd;
	double execDuration;
	QColor bgColor;
	QStringList imageFiles;
	int currentImageIndex = -1;
	QString currentDirectory;
	static constexpr Qt::Key prevImgShortcut = Qt::Key_Comma;
	static constexpr Qt::Key nextImgShortcut = Qt::Key_Period;
	std::vector<QWidget*> cameraWidgets{};

	// Manages the color of the button for picking a color.
	void SetupColorPicker();

	// Loads all .ppm files in the provided directory.
	// @param[in] imagePath: The path to the directory to load images from.
	void LoadDirectoryContents( const QString& );

	// Displays the nex / previous image in the list. Manages wrapping when end is reached.
	// @param[in] offset: The offset to apply to the current image index.
	void NavigateImages( int );

	// Collects and removes from screen all camera animation-related widgets.
	void SetupCameraWidgets();

	// Sets up the camera initial position and the move / rotation vector for the animation.
	// @param[out] initialPos: The initial camera position.
	// @param[out] actionCoords: The move / rotation vector.
	void SetupAnimationCoordinates( FVector3&, FVector3& );

	// Handles key-press events to disable shortcuts whe some widgets are in focus.
	// @param[in] watched: The object that is being watched for events.
	// @param[in] event: The event that is being processed.
	bool eventFilter( QObject*, QEvent* ) override;
};

#endif // UI_MAINWINDOW_H