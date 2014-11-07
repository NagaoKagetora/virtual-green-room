#ifndef _VGR_GUI_QVGRMAINWINDOW_H
#define _VGR_GUI_QVGRMAINWINDOW_H

#include <QtGui/QMainWindow>
#include <common/global.hpp>



class QAction;
class QActionGroup;
class QDockWidget;
class QGroupBox;
class QLabel;
class QMenuBar;
class QPushButton;
class QSplitter;
class QTabWidget;
class QVGRManager;
class QVGRViewerWidget;
class QVGRRegistrationModule;
class QVGRReflectionModule;
class QVGRSectionModule;
class QVGRColorizationModule;
class QVGRModuleGroup;
class QVGRExplorerWidget;
class QVGRSnapshotWidget;
class QVGRLoggerWidget;
class QVGRCentralWindow;
class QVGRMainWindow;



class QVGRCentralWindow : public QMainWindow {

	Q_OBJECT

private:
	QVGRManager							*m_manager;
	QVGRRegistrationModule				*m_registrationModule;
	QVGRReflectionModule				*m_reflectionModule;
	QVGRSectionModule					*m_sectionModule;
	QVGRColorizationModule				*m_colorizationModule;
	QVGRModuleGroup						*m_modules;
	QVGRViewerWidget					*m_viewWidget;
	QVGRExplorerWidget					*m_explorerWidget;
	QVGRSnapshotWidget					*m_snapshotWidget;
	QVGRLoggerWidget					*m_loggerWidget;
	QTabWidget							*m_sideTabWidget;
	QSplitter							*m_centralSplitter;
	QAction								*m_createProjectAction;
	QAction								*m_openProjectAction;
	QAction								*m_saveProjectAction;
	QAction								*m_snapshotAction;
	QAction								*m_configAction;
	QAction								*m_renderVertexAction;
	QAction								*m_renderEdgeAction;
	QAction								*m_renderFaceAction;
	QAction								*m_renderCaptionAction;
	QAction								*m_renderCompassAction;
	QActionGroup						*m_renderActionGroup;
	QAction								*m_viewportProjectionModeAction;
	QAction								*m_viewportHomeAction;
	QAction								*m_viewportLeftRotateAction;
	QAction								*m_viewportRightRotateAction;
	QAction								*m_viewportFrontAction;
	QAction								*m_viewportBackAction;
	QAction								*m_viewportLeftAction;
	QAction								*m_viewportRightAction;
	QAction								*m_viewportTopAction;
	QAction								*m_viewportBottomAction;
	QActionGroup						*m_viewportActionGroup;
	QAction								*m_manipulatorRotationLockAction;
	QAction								*m_manipulatorTranslationLockAction;
	QAction								*m_manipulatorZoomingLockAction;
	QActionGroup						*m_manipulatorActionGroup;
	QAction								*m_viewSideAction;
	QAction								*m_viewLogAction;
	QAction								*m_infoAction;

public:
	QVGRCentralWindow(QWidget *parent = 0, Qt::WFlags flags = 0);
	~QVGRCentralWindow();

protected slots:
	void createProject();
	void openProject();
	void openFiles(const QStringList &fileNames);
	void saveProject();
	void snapshot();
	void config();
	void changeRender(QAction *action);
	void changeViewport(QAction *action);
	void changeManipulator(QAction *action);

private slots:
	void onLoadingStarted();
	void onLoadingFinished();
	void onModulesActivatedChanged();
	void onTaskStarted();
	void onTaskFinished();
	void onViewSideActionTriggered();
	void onViewLogActionTriggered();
	void onInfoActionTriggered();
};



class QVGRMainWindow : public QMainWindow {

	Q_OBJECT

private:
	QLabel								*m_titleIconLabel;
	QLabel								*m_titleTextLabel;
	QMenuBar							*m_menuBar;
	QPushButton							*m_minimizePushButton;
	QPushButton							*m_maximizePushButton;
	QPushButton							*m_closePushButton;
	QVGRCentralWindow					*m_centralWindow;

public:
	QVGRMainWindow(QWidget *parent = 0, Qt::WFlags flags = 0);
	~QVGRMainWindow() { }
};



#endif /* _VGR_GUI_QVGRMAINWINDOW_H */
