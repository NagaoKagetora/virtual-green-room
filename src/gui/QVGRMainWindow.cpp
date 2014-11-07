#include "QVGRMainWindow.h"
#include <common/global.hpp>
#include <common/algo/util.hpp>
#include <common/util/logger.hpp>
#include <QtCore/QDateTime>
#include <QtCore/QFile>
#include <QtCore/QState>
#include <QtCore/QStateMachine>
#include <QtGui/QAction>
#include <QtGui/QActionGroup>
#include <QtGui/QDockWidget>
#include <QtGui/QFileDialog>
#include <QtGui/QGroupBox>
#include <QtGui/QLabel>
#include <QtGui/QLayout>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QSplitter>
#include <QtGui/QStatusBar>
#include <QtGui/QTabWidget>
#include <QtGui/QToolBar>
#include <core/basic/Configuration.hpp>
#include <gui/QVGRManager.h>
#include <gui/module/QVGRRegistrationModule.h>
#include <gui/module/QVGRReflectionModule.h>
#include <gui/module/QVGRSectionModule.h>
#include <gui/module/QVGRColorizationModule.h>
#include <gui/QVGRViewerWidget.h>
#include <gui/QVGRExplorerWidget.h>
#include <gui/QVGRSnapshotWidget.h>
#include <gui/QVGRLoggerWidget.h>
#include <gui/QVGRCreateProjectDialog.h>
#include <gui/QVGRConfigDialog.h>
#include <gui/QVGRInfoDialog.h>

using namespace vgr;

#pragma warning (push)
#pragma warning (disable:4482)	/* nonstandard extension used: enum 'enum' used in qualified name */



QVGRCentralWindow::QVGRCentralWindow(QWidget *parent, Qt::WFlags flags) : QMainWindow(parent, flags) {
	
	this->setFont(QFont(QString("Arial"), 9));
	this->setWindowIcon(QIcon(QString(":/resource/logo/logo.png")));
	this->setWindowTitle(tr("GM Virtual Green Room"));

	// Widgets.
	this->m_manager = new QVGRManager();
	this->m_registrationModule = new QVGRRegistrationModule(tr("Registration"), this);
	this->m_reflectionModule = new QVGRReflectionModule(tr("Reflection"), this);
	this->m_sectionModule = new QVGRSectionModule(tr("Section"), this);
	this->m_colorizationModule = new QVGRColorizationModule(tr("Colorization"), this);
	this->m_modules = new QVGRModuleGroup();
	this->m_modules->append(this->m_registrationModule);
	this->m_modules->append(this->m_reflectionModule);
	this->m_modules->append(this->m_sectionModule);
	this->m_modules->append(this->m_colorizationModule);
	this->m_viewWidget = new QVGRViewerWidget(this->m_manager, this);
	this->m_viewWidget->regist(this->m_registrationModule);
	this->m_viewWidget->regist(this->m_reflectionModule);
	this->m_viewWidget->regist(this->m_sectionModule);
	this->m_viewWidget->regist(this->m_colorizationModule);
	this->m_explorerWidget = new QVGRExplorerWidget(this->m_manager, this);
	this->m_snapshotWidget = new QVGRSnapshotWidget(this);
	this->m_loggerWidget = new QVGRLoggerWidget(this);
	this->m_sideTabWidget = new QTabWidget();
	this->m_sideTabWidget->setMaximumWidth(300);
	this->m_sideTabWidget->addTab(this->m_explorerWidget, tr("Explorer"));
	this->m_sideTabWidget->addTab(this->m_snapshotWidget, tr("Snapshot"));
	this->m_sideTabWidget->setVisible(false);
	this->m_centralSplitter = new QSplitter(Qt::Orientation::Horizontal);
	this->m_centralSplitter->addWidget(this->m_sideTabWidget);
	this->m_centralSplitter->addWidget(this->m_viewWidget);

	// Actions.
	this->m_createProjectAction = new QAction(QIcon(QString(":/resource/toolbar/main_create.png")), tr("Create Project"), this);
	this->m_openProjectAction = new QAction(QIcon(QString(":/resource/toolbar/main_open.png")), tr("Open Project"), this);
	this->m_saveProjectAction = new QAction(QIcon(QString(":/resource/toolbar/main_save.png")), tr("Save Project"), this);
	this->m_snapshotAction = new QAction(QIcon(QString(":/resource/toolbar/main_snapshot.png")), tr("Snapshot"), this);
	this->m_configAction = new QAction(QIcon(QString(":/resource/toolbar/main_config.png")), tr("Config"), this);
	this->m_renderVertexAction = new QAction(QIcon(QString(":/resource/toolbar/render_vertex.png")), tr("Vertex"), this);
	this->m_renderVertexAction->setCheckable(true);
	this->m_renderEdgeAction = new QAction(QIcon(QString(":/resource/toolbar/render_edge.png")), tr("Edge"), this);
	this->m_renderEdgeAction->setCheckable(true);
	this->m_renderFaceAction = new QAction(QIcon(QString(":/resource/toolbar/render_face.png")), tr("Face"), this);
	this->m_renderFaceAction->setCheckable(true);
	this->m_renderFaceAction->setChecked(true);
	this->m_renderCaptionAction = new QAction(QIcon(QString(":/resource/toolbar/render_caption.png")), tr("Caption"), this);
	this->m_renderCaptionAction->setCheckable(true);
	this->m_renderCaptionAction->setChecked(true);
	this->m_renderCompassAction = new QAction(QIcon(QString(":/resource/toolbar/render_compass.png")), tr("Compass"), this);
	this->m_renderCompassAction->setCheckable(true);
	this->m_renderCompassAction->setChecked(true);
	this->m_renderActionGroup = new QActionGroup(this);
	this->m_renderActionGroup->setExclusive(false);
	this->m_renderActionGroup->addAction(this->m_renderVertexAction);
	this->m_renderActionGroup->addAction(this->m_renderEdgeAction);
	this->m_renderActionGroup->addAction(this->m_renderFaceAction);
	this->m_renderActionGroup->addAction(this->m_renderCaptionAction);
	this->m_renderActionGroup->addAction(this->m_renderCompassAction);
	this->m_viewportProjectionModeAction = new QAction(QIcon(QString(":/resource/toolbar/viewport_projection_perspective.png")), tr("Perspective Projection"), this);
	this->m_viewportHomeAction = new QAction(QIcon(QString(":/resource/toolbar/viewport_home.png")), tr("Home"), this);
	this->m_viewportLeftRotateAction = new QAction(QIcon(QString(":/resource/toolbar/viewport_left_rotate.png")), tr("Left Rotate"), this);
	this->m_viewportRightRotateAction = new QAction(QIcon(QString(":/resource/toolbar/viewport_right_rotate.png")), tr("Right Rotate"), this);
	this->m_viewportFrontAction = new QAction(QIcon(QString(":/resource/toolbar/viewport_front.png")), tr("Front"), this);
	this->m_viewportBackAction = new QAction(QIcon(QString(":/resource/toolbar/viewport_back.png")), tr("Back"), this);
	this->m_viewportLeftAction = new QAction(QIcon(QString(":/resource/toolbar/viewport_left.png")), tr("Left"), this);
	this->m_viewportRightAction = new QAction(QIcon(QString(":/resource/toolbar/viewport_right.png")), tr("Right"), this);
	this->m_viewportTopAction = new QAction(QIcon(QString(":/resource/toolbar/viewport_top.png")), tr("Top"), this);
	this->m_viewportBottomAction = new QAction(QIcon(QString(":/resource/toolbar/viewport_bottom.png")), tr("Bottom"), this);
	this->m_viewportActionGroup = new QActionGroup(this);
	this->m_viewportActionGroup->addAction(this->m_viewportProjectionModeAction);
	this->m_viewportActionGroup->addAction(this->m_viewportHomeAction);
	this->m_viewportActionGroup->addAction(this->m_viewportLeftRotateAction);
	this->m_viewportActionGroup->addAction(this->m_viewportRightRotateAction);
	this->m_viewportActionGroup->addAction(this->m_viewportFrontAction);
	this->m_viewportActionGroup->addAction(this->m_viewportBackAction);
	this->m_viewportActionGroup->addAction(this->m_viewportLeftAction);
	this->m_viewportActionGroup->addAction(this->m_viewportRightAction);
	this->m_viewportActionGroup->addAction(this->m_viewportTopAction);
	this->m_viewportActionGroup->addAction(this->m_viewportBottomAction);
	this->m_manipulatorRotationLockAction = new QAction(QIcon(QString(":/resource/toolbar/manipulator_rotation_lock.png")), tr("Rotation Lock"), this);
	this->m_manipulatorRotationLockAction->setCheckable(true);
	this->m_manipulatorTranslationLockAction = new QAction(QIcon(QString(":/resource/toolbar/manipulator_translation_lock.png")), tr("Translation Lock"), this);
	this->m_manipulatorTranslationLockAction->setCheckable(true);
	this->m_manipulatorZoomingLockAction = new QAction(QIcon(QString(":/resource/toolbar/manipulator_zooming_lock.png")), tr("Zooming Lock"), this);
	this->m_manipulatorZoomingLockAction->setCheckable(true);
	this->m_manipulatorActionGroup = new QActionGroup(this);
	this->m_manipulatorActionGroup->setExclusive(false);
	this->m_manipulatorActionGroup->addAction(this->m_manipulatorRotationLockAction);
	this->m_manipulatorActionGroup->addAction(this->m_manipulatorTranslationLockAction);
	this->m_manipulatorActionGroup->addAction(this->m_manipulatorZoomingLockAction);
	this->m_viewSideAction = new QAction(QIcon(QString(":/resource/toolbar/view_side.png")), tr("Side View"), this);
	this->m_viewSideAction->setCheckable(true);
	this->m_viewLogAction = new QAction(QIcon(QString(":/resource/toolbar/view_log.png")), tr("Log View"), this);
	this->m_infoAction = new QAction(QIcon(QString(":/resource/toolbar/info.png")), tr("Info"), this);

	// Toolbars.
	QToolBar *mainToolBar = this->addToolBar(tr("Main"));
	mainToolBar->addAction(this->m_createProjectAction);
	mainToolBar->addAction(this->m_openProjectAction);
	mainToolBar->addAction(this->m_saveProjectAction);
	mainToolBar->addSeparator();
	mainToolBar->addAction(this->m_snapshotAction);
	mainToolBar->addSeparator();
	mainToolBar->addAction(this->m_configAction);
	QToolBar *renderToolBar = this->addToolBar(tr("Render"));
	renderToolBar->addActions(this->m_renderActionGroup->actions());
	QToolBar *viewportToolBar = this->addToolBar(tr("Viewport"));
	viewportToolBar->addActions(this->m_viewportActionGroup->actions());
	QToolBar *manipulatorToolBar = this->addToolBar(tr("Manipulator"));
	manipulatorToolBar->addActions(this->m_manipulatorActionGroup->actions());
	QToolBar *viewToolBar = this->addToolBar(tr("View"));
	viewToolBar->addAction(this->m_viewSideAction);
	viewToolBar->addAction(this->m_viewLogAction);
	QToolBar *infoToolBar = this->addToolBar(tr("Info"));
	infoToolBar->addAction(this->m_infoAction);
	this->addToolBar(Qt::ToolBarArea::RightToolBarArea, this->m_registrationModule);
	this->addToolBar(Qt::ToolBarArea::RightToolBarArea, this->m_reflectionModule);
	this->addToolBar(Qt::ToolBarArea::RightToolBarArea, this->m_sectionModule);
	this->addToolBar(Qt::ToolBarArea::RightToolBarArea, this->m_colorizationModule);
	QStatusBar *statusBar = this->statusBar();
	statusBar->addWidget(this->m_loggerWidget->statusWidget());
	statusBar->addPermanentWidget(this->m_loggerWidget->statusPermanentWidget());
	this->m_loggerWidget->setHostWidget(statusBar);

	// Layouts.
	QHBoxLayout *layout = new QHBoxLayout();
	layout->setContentsMargins(1, 1, 1, 1);
	layout->addWidget(this->m_centralSplitter);
	QWidget *layoutWidget = new QWidget();
	layoutWidget->setLayout(layout);
	this->setCentralWidget(layoutWidget);

	// Signals and slots.
	connect(this->m_manager, SIGNAL(loadingStarted()), this, SLOT(onLoadingStarted()));
	connect(this->m_manager, SIGNAL(loadingFinished()), this, SLOT(onLoadingFinished()));
	connect(this->m_createProjectAction, SIGNAL(triggered(bool)), this, SLOT(createProject()));
	connect(this->m_openProjectAction, SIGNAL(triggered(bool)), this, SLOT(openProject()));
	connect(this->m_saveProjectAction, SIGNAL(triggered(bool)), this, SLOT(saveProject()));
	connect(this->m_snapshotAction, SIGNAL(triggered(bool)), this, SLOT(snapshot()));
	connect(this->m_configAction, SIGNAL(triggered(bool)), this, SLOT(config()));
	connect(this->m_renderActionGroup, SIGNAL(triggered(QAction *)), this, SLOT(changeRender(QAction *)));
	connect(this->m_viewportActionGroup, SIGNAL(triggered(QAction *)), this, SLOT(changeViewport(QAction *)));
	connect(this->m_manipulatorActionGroup, SIGNAL(triggered(QAction *)), this, SLOT(changeManipulator(QAction *)));
	connect(this->m_viewSideAction, SIGNAL(triggered(bool)), this, SLOT(onViewSideActionTriggered()));
	connect(this->m_viewLogAction, SIGNAL(triggered(bool)), this, SLOT(onViewLogActionTriggered()));
	connect(this->m_infoAction, SIGNAL(triggered(bool)), this, SLOT(onInfoActionTriggered()));
	connect(this->m_viewWidget, SIGNAL(taskStarted()), this, SLOT(onTaskStarted()));
	connect(this->m_viewWidget, SIGNAL(taskFinished()), this, SLOT(onTaskFinished()));
	connect(this->m_viewWidget, SIGNAL(filesDropped(const QStringList &)), this, SLOT(openFiles(const QStringList &)));
	connect(this->m_explorerWidget, SIGNAL(modelChanged(int)), this->m_viewWidget, SLOT(refresh()));
	connect(this->m_registrationModule, SIGNAL(activated(QString)), this, SLOT(onModulesActivatedChanged()));
	connect(this->m_reflectionModule, SIGNAL(activated(QString)), this, SLOT(onModulesActivatedChanged()));
	connect(this->m_sectionModule, SIGNAL(activated(QString)), this, SLOT(onModulesActivatedChanged()));
	connect(this->m_colorizationModule, SIGNAL(activated(QString)), this, SLOT(onModulesActivatedChanged()));

	_VGR_SYSTEM_MSG("Ready");
}

QVGRCentralWindow::~QVGRCentralWindow() {
	if (!this->m_viewWidget) return;
	this->m_viewWidget->dispose();
}

void QVGRCentralWindow::createProject() {
	QVGRCreateProjectDialog *dialog = new QVGRCreateProjectDialog(this);
	if (dialog->exec()) {
		this->m_manager->clear();
		this->m_manager->addItems(dialog->fileNames());
	}
}

void QVGRCentralWindow::openProject() {
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open Project"),
			Configuration::instance().applicationConfiguration().recentDirectory(), tr("Project File (*.vproj)"));
	if (fileName.length()) {
		QFileInfo fileInfo(fileName);
		Configuration::instance().applicationConfiguration().setRecentDirectory(fileInfo.absoluteDir().absolutePath());
		this->m_manager->clear();
		this->m_manager->loadProject(fileInfo.absoluteFilePath());
	}
}

void QVGRCentralWindow::openFiles(const QStringList &fileNames) {
	if (fileNames.empty()) return;

	// Handles opening project file.
	QFileInfo fileInfo(fileNames.front());
	if (fileInfo.suffix().toLower() == QString("vproj")) {
		this->m_manager->clear();
		this->m_manager->loadProject(fileInfo.absoluteFilePath());
		return;
	}

	// Handles opening model files.
	QStringList formats;
	formats << QString("nas") << QString("obj") << QString("stl") << QString("ply");
	QStringList models;
	foreach (const QString &fileName, fileNames) {
		QFileInfo info(fileName);
		if (!formats.contains(info.suffix().toLower())) {
			_VGR_SYSTEM_WRN(boost::format("Ignore file <%1%> due to unsupported mesh format <%2%>.") % fileName.toStdString() % info.suffix().toStdString());
			continue;
		}
		models << fileName;
	}
	if (models.length()) {
		this->m_manager->clear();
		this->m_manager->addItems(models);
	}
}

void QVGRCentralWindow::saveProject() {
	QString fileName = QFileDialog::getSaveFileName(this, tr("Save Project"),
			Configuration::instance().applicationConfiguration().recentDirectory(), tr("Project File (*.vproj)"));
	if (fileName.length()) {
		QFileInfo fileInfo(fileName);
		Configuration::instance().applicationConfiguration().setRecentDirectory(fileInfo.absoluteDir().absolutePath());
		this->m_manager->saveProject(fileInfo.absoluteFilePath());
	}
}

void QVGRCentralWindow::snapshot() {
	if (!this->m_manager->size()) return;

	foreach (Scene *scene, this->m_viewWidget->scenes()) {
		if (!scene || scene->caption().empty()) continue;
		this->m_snapshotWidget->addSnapshot(scene->screen().widget()->grabFrameBuffer(), QString::fromStdString(scene->caption()));
	}
}

void QVGRCentralWindow::config() {
	QVGRConfigDialog dialog(this);
	dialog.loadSettings();
	if (dialog.exec()) {
		dialog.updateSettings();
		this->m_viewWidget->updateSettings();
	}
}

void QVGRCentralWindow::changeRender(QAction *action) {
	QVGRRenderingSettings &settings(this->m_viewWidget->settings());
	settings.vertexMode()->set(this->m_renderVertexAction->isChecked());
	settings.vertexMode()->dirty();
	settings.edgeMode()->set(this->m_renderEdgeAction->isChecked());
	settings.edgeMode()->dirty();
	settings.faceMode()->set(this->m_renderFaceAction->isChecked());
	settings.faceMode()->dirty();
	foreach (StandardScene *scene, this->m_viewWidget->standards()) {
		scene->sprite()->setCaptionEnabled(this->m_renderCaptionAction->isChecked());
		scene->sprite()->setCompassEnabled(this->m_renderCompassAction->isChecked());
	}
	this->m_viewWidget->refresh();
}

void QVGRCentralWindow::changeViewport(QAction *action) {
	StandardScene::Manipulator *manipulator = StandardScene::sharedManipulator();
	if (action == this->m_viewportProjectionModeAction) {
		if (StandardScene::sharedManipulator()->mode() == StandardScene::Manipulator::Perspective) {
			this->m_viewportProjectionModeAction->setIcon(QIcon(QString(":/resource/toolbar/viewport_projection_orthography.png")));
			this->m_viewportProjectionModeAction->setToolTip(tr("Orthographic Projection"));
			StandardScene::sharedManipulator()->changeMode(StandardScene::Manipulator::Orthographic);
			this->m_viewWidget->settings().orthographicMode()->set(true);
		} else {
			this->m_viewportProjectionModeAction->setIcon(QIcon(QString(":/resource/toolbar/viewport_projection_perspective.png")));
			this->m_viewportProjectionModeAction->setToolTip(tr("Perspective Projection"));
			StandardScene::sharedManipulator()->changeMode(StandardScene::Manipulator::Perspective);
			this->m_viewWidget->settings().orthographicMode()->set(false);
		}
	} else if (action == this->m_viewportHomeAction) {
		foreach (const StandardScene *scene, this->m_viewWidget->standards()) {
			if (scene->ready()) StandardScene::sharedManipulator()->fitScene(scene);
		}
	} else if (action == this->m_viewportLeftRotateAction) {
		osg::Vec3 axis = osg::Matrixd::inverse(osg::Matrixd(manipulator->getRotation())) * osg::Vec3(0.0f, 0.0f, 1.0f);
		osg::Quat quat = manipulator->getRotation() * osg::Quat(-osg::PI_2, axis);
		manipulator->setRotation(quat);
	} else if (action == this->m_viewportRightRotateAction) {
		osg::Vec3 axis = osg::Matrixd::inverse(osg::Matrixd(manipulator->getRotation())) * osg::Vec3(0.0f, 0.0f, 1.0f);
		osg::Quat quat = manipulator->getRotation() * osg::Quat(osg::PI_2, axis);
		manipulator->setRotation(quat);
	} else if (action == this->m_viewportFrontAction) {
		manipulator->setRotation(osg::Quat());
	} else if (action == this->m_viewportBackAction) {
		manipulator->setRotation(osg::Quat(osg::PI, osg::Vec3(0.0f, 1.0f, 0.0f)));
	} else if (action == this->m_viewportLeftAction) {
		manipulator->setRotation(osg::Quat(-osg::PI_2, osg::Vec3(0.0f, 1.0f, 0.0f)));
	} else if (action == this->m_viewportRightAction) {
		manipulator->setRotation(osg::Quat(osg::PI_2, osg::Vec3(0.0f, 1.0f, 0.0f)));
	} else if (action == this->m_viewportTopAction) {
		manipulator->setRotation(osg::Quat(-osg::PI_2, osg::Vec3(1.0f, 0.0f, 0.0f)));
	} else if (action == this->m_viewportBottomAction) {
		manipulator->setRotation(osg::Quat(osg::PI_2, osg::Vec3(1.0f, 0.0f, 0.0f)));
	}
	this->m_viewWidget->refresh();
	foreach (QVGRModule *module, this->m_viewWidget->modules()) {
		if (module && module->getSettings().activated) {
			module->getFunction().update(module->getSettings());
		}
	}
	this->m_viewWidget->refresh();
}

void QVGRCentralWindow::changeManipulator(QAction *action) {
	StandardScene::Manipulator *manipulator = StandardScene::sharedManipulator();
	if (action == this->m_manipulatorRotationLockAction) {
		manipulator->setRotationEnabled(!this->m_manipulatorRotationLockAction->isChecked());
	}
	if (action == this->m_manipulatorTranslationLockAction) {
		manipulator->setTranslationEnabled(!this->m_manipulatorTranslationLockAction->isChecked());
	}
	if (action == this->m_manipulatorZoomingLockAction) {
		manipulator->setZoomingEnabled(!this->m_manipulatorZoomingLockAction->isChecked());
	}
}

void QVGRCentralWindow::onLoadingStarted() {
	this->m_createProjectAction->setEnabled(false);
	this->m_openProjectAction->setEnabled(false);
	this->m_saveProjectAction->setEnabled(false);
	this->m_renderActionGroup->setEnabled(false);
	this->m_viewportActionGroup->setEnabled(false);
	this->m_modules->setEnabled(false);
}

void QVGRCentralWindow::onLoadingFinished() {
	this->m_viewWidget->displayProject();
	this->m_createProjectAction->setEnabled(true);
	this->m_openProjectAction->setEnabled(true);
	this->m_saveProjectAction->setEnabled(true);
	this->m_renderActionGroup->setEnabled(true);
	this->m_viewportActionGroup->setEnabled(true);
	this->m_modules->setEnabled(true);
	this->m_manipulatorRotationLockAction->setChecked(false);
	this->m_manipulatorTranslationLockAction->setChecked(false);
	this->m_manipulatorZoomingLockAction->setChecked(false);
}

void QVGRCentralWindow::onModulesActivatedChanged() {
	bool deactivated = !this->m_modules->activated();
	this->m_createProjectAction->setEnabled(deactivated);
	this->m_openProjectAction->setEnabled(deactivated);
	this->m_saveProjectAction->setEnabled(deactivated);
	this->m_explorerWidget->setAllowDrags(deactivated);
	this->m_viewWidget->setScenesAcceptDrops(deactivated);
}

void QVGRCentralWindow::onTaskStarted() {
	this->m_modules->setEnabled(false);
}

void QVGRCentralWindow::onTaskFinished() {
	this->m_modules->setEnabled(true);
	this->m_loggerWidget->blink();
}

void QVGRCentralWindow::onViewSideActionTriggered() {
	this->m_sideTabWidget->setVisible(this->m_viewSideAction->isChecked());
}

void QVGRCentralWindow::onViewLogActionTriggered() {
	this->m_loggerWidget->show();
}

void QVGRCentralWindow::onInfoActionTriggered() {
	QVGRInfoDialog dialog(this);
	dialog.exec();
}



QVGRMainWindow::QVGRMainWindow(QWidget *parent, Qt::WFlags flags) : QMainWindow(parent, flags) {

	this->setFont(QFont(QString("Arial"), 9));
	this->setWindowIcon(QIcon(QString(":/resource/logo/logo.png")));
	this->setWindowTitle(tr("GM Virtual Green Room"));

	this->m_titleIconLabel = new QLabel();
	this->m_titleIconLabel->setPixmap(QPixmap(QString(":/resource/logo/logo.png")).scaled(this->iconSize(), Qt::AspectRatioMode::IgnoreAspectRatio, Qt::TransformationMode::SmoothTransformation));
	this->m_titleTextLabel = new QLabel(tr("Virtual Green Room"));
	//this->m_titleTextLabel->setFixedHeight(16);
	this->m_titleTextLabel->setObjectName(tr("TitleTextLabel"));
	this->m_menuBar = new QMenuBar();
	QMenu *fileMenu = new QMenu(tr("File"));
	fileMenu->addAction(tr("Create Project"));
	fileMenu->addAction(tr("Open Project"));
	fileMenu->addAction(tr("Save Project"));
	this->m_menuBar->addMenu(fileMenu);
	this->m_menuBar->addMenu(tr("Edit"));
	this->m_menuBar->addMenu(tr("View"));
	this->m_menuBar->addMenu(tr("Help"));
	this->m_minimizePushButton = new QPushButton(QIcon(QString(":/resource/handle/minus_medium.png")), QString());
	this->m_minimizePushButton->setObjectName(QString("MinimizePushButton"));
	this->m_maximizePushButton = new QPushButton(QIcon(QString(":/resource/handle/float_medium.png")), QString());
	this->m_maximizePushButton->setObjectName(QString("MaximizePushButton"));
	this->m_closePushButton = new QPushButton(QIcon(QString(":/resource/handle/close_medium.png")), QString());
	this->m_closePushButton->setObjectName(QString("ClosePushButton"));
	this->m_centralWindow = new QVGRCentralWindow(this, Qt::WindowType::FramelessWindowHint);

	//this->setMenuBar(m_menuBar);
	QHBoxLayout *buttonsLayout = new QHBoxLayout();
	buttonsLayout->addWidget(this->m_minimizePushButton);
	buttonsLayout->addWidget(this->m_maximizePushButton);
	buttonsLayout->addWidget(this->m_closePushButton);
	buttonsLayout->setSpacing(0);
	buttonsLayout->setContentsMargins(10, 0, 10, 0);
	QWidget *buttonsLayoutWidget = new QWidget();
	buttonsLayoutWidget->setLayout(buttonsLayout);
	QHBoxLayout *titleLayout = new QHBoxLayout();
	titleLayout->addWidget(this->m_titleIconLabel);
	titleLayout->addWidget(this->m_titleTextLabel);
	titleLayout->addWidget(this->m_menuBar);
	titleLayout->addWidget(buttonsLayoutWidget);
	titleLayout->setContentsMargins(0, 0, 0, 0);
	QWidget *titleLayoutWidget = new QWidget();
	titleLayoutWidget->setLayout(titleLayout);
	QVBoxLayout *layout = new QVBoxLayout();
	layout->setContentsMargins(1, 1, 1, 0);
	layout->addWidget(titleLayoutWidget);
	layout->addWidget(this->m_centralWindow);
	layout->setSpacing(1);
	QWidget *layoutWidget = new QWidget();
	layoutWidget->setLayout(layout);

	this->setCentralWidget(layoutWidget);

	connect(this->m_minimizePushButton, SIGNAL(clicked()), this, SLOT(showMinimized()));
	connect(this->m_maximizePushButton, SIGNAL(clicked()), this, SLOT(showMaximized()));
	connect(this->m_closePushButton, SIGNAL(clicked()), this, SLOT(close()));
}



#pragma warning (pop)
