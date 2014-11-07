#include "QVGRViewerWidget.h"
#include <QtGui/QGridLayout>
#include <QtGui/QResizeEvent>
#include <osgGA/TrackballManipulator>
#include <osgViewer/CompositeViewer>
#include <common/3rdparty/osg/GraphicsWindow.hpp>
#include <core/basic/Configuration.hpp>

#pragma warning (push)
#pragma warning (disable:4482)	/* nonstandard extension used: enum 'enum' used in qualified name */



class ViewMatrixUniformCallback;
class ProjectionMatrixUniformCallback;
class ViewportMatrixUniformCallback;
class ResizeCallback;



class ViewMatrixUniformCallback : public osg::Uniform::Callback {

private:
	osg::Camera							*m_camera;

public:
	ViewMatrixUniformCallback(osg::Camera *camera) : m_camera(camera) { }

public:
	virtual void operator ()(osg::Uniform *uniform, osg::NodeVisitor *visitor) {
		if (uniform) uniform->set(this->m_camera->getViewMatrix());
		uniform->dirty();
	}
};



class ProjectionMatrixUniformCallback : public osg::Uniform::Callback {

private:
	osg::Camera							*m_camera;

public:
	ProjectionMatrixUniformCallback(osg::Camera *camera) : m_camera(camera) { }

public:
	virtual void operator ()(osg::Uniform *uniform, osg::NodeVisitor *visitor) {
		if (uniform) uniform->set(this->m_camera->getProjectionMatrix());
		uniform->dirty();
	}
};



class ViewportMatrixUniformCallback : public osg::Uniform::Callback {

private:
	osg::Camera							*m_camera;

public:
	ViewportMatrixUniformCallback(osg::Camera *camera) : m_camera(camera) { }

public:
	virtual void operator ()(osg::Uniform *uniform, osg::NodeVisitor *visitor) {
		if (uniform) uniform->set(this->m_camera->getViewport()->computeWindowMatrix());
		uniform->dirty();
	}
};



class ResizeCallback : public Scene::Screen::Widget::ResizeCallback {

private:
	Scene								*m_scene;

public:
	ResizeCallback(Scene *scene) : m_scene(scene) { }

public:
	virtual void operator ()(QResizeEvent *event) {
		this->m_scene->setViewport(event->size().width(), event->size().height());
	}
};



QVGRRenderingSettings::QVGRRenderingSettings() {
	this->m_pointSize				= new osg::Uniform("point.size", (float)Configuration::instance().sceneConfiguration().pointSize());
	this->m_pointColor				= new osg::Uniform("point.color", Converter::toVec3(Configuration::instance().sceneConfiguration().pointColor()));
	this->m_lineWidth				= new osg::Uniform("line.width", (float)Configuration::instance().sceneConfiguration().lineWidth());
	this->m_lineColor				= new osg::Uniform("line.color", Converter::toVec3(Configuration::instance().sceneConfiguration().lineColor()));
	this->m_lightIntensity			= new osg::Uniform("light.intensity", Converter::toVec3(Configuration::instance().sceneConfiguration().lightIntensity()));
	this->m_frontMaterialAmbient	= new osg::Uniform("frontMaterial.ambient", Converter::toVec3(Configuration::instance().sceneConfiguration().frontMaterialAmbient()));
	this->m_frontMaterialDiffuse	= new osg::Uniform("frontMaterial.diffuse", Converter::toVec3(Configuration::instance().sceneConfiguration().frontMaterialDiffuse()));
	this->m_frontMaterialSpecular	= new osg::Uniform("frontMaterial.specular", Converter::toVec3(Configuration::instance().sceneConfiguration().frontMaterialSpecular()));
	this->m_frontMaterialShininess	= new osg::Uniform("frontMaterial.shininess", (float)Configuration::instance().sceneConfiguration().frontMaterialShininess());
	this->m_backMaterialAmbient		= new osg::Uniform("backMaterial.ambient", Converter::toVec3(Configuration::instance().sceneConfiguration().backMaterialAmbient()));
	this->m_backMaterialDiffuse		= new osg::Uniform("backMaterial.diffuse", Converter::toVec3(Configuration::instance().sceneConfiguration().backMaterialDiffuse()));
	this->m_backMaterialSpecular	= new osg::Uniform("backMaterial.specular", Converter::toVec3(Configuration::instance().sceneConfiguration().backMaterialSpecular()));
	this->m_backMaterialShininess	= new osg::Uniform("backMaterial.shininess", (float)Configuration::instance().sceneConfiguration().backMaterialShininess());
	this->m_effectMaterialAmbient	= new osg::Uniform("effectMaterial.ambient", Converter::toVec3(Configuration::instance().sceneConfiguration().effectMaterialAmbient()));
	this->m_effectMaterialDiffuse	= new osg::Uniform("effectMaterial.diffuse", Converter::toVec3(Configuration::instance().sceneConfiguration().effectMaterialDiffuse()));
	this->m_effectMaterialSpecular	= new osg::Uniform("effectMaterial.specular", Converter::toVec3(Configuration::instance().sceneConfiguration().effectMaterialSpecular()));
	this->m_effectMaterialShininess	= new osg::Uniform("effectMaterial.shininess", (float)Configuration::instance().sceneConfiguration().effectMaterialShininess());
	this->m_orthographicMode		= new osg::Uniform("mode.orthographic", false);
	this->m_vertexMode				= new osg::Uniform("mode.vertex", false);
	this->m_edgeMode				= new osg::Uniform("mode.edge", false);
	this->m_faceMode				= new osg::Uniform("mode.face", true);
}

void QVGRRenderingSettings::refresh() {
	this->m_pointSize->set((float)Configuration::instance().sceneConfiguration().pointSize());
	this->m_pointColor->set(Converter::toVec3(Configuration::instance().sceneConfiguration().pointColor()));
	this->m_lineWidth->set((float)Configuration::instance().sceneConfiguration().lineWidth());
	this->m_lineColor->set(Converter::toVec3(Configuration::instance().sceneConfiguration().lineColor()));
	this->m_lightIntensity->set(Converter::toVec3(Configuration::instance().sceneConfiguration().lightIntensity()));
	this->m_frontMaterialAmbient->set(Converter::toVec3(Configuration::instance().sceneConfiguration().frontMaterialAmbient()));
	this->m_frontMaterialDiffuse->set(Converter::toVec3(Configuration::instance().sceneConfiguration().frontMaterialDiffuse()));
	this->m_frontMaterialSpecular->set(Converter::toVec3(Configuration::instance().sceneConfiguration().frontMaterialSpecular()));
	this->m_frontMaterialShininess->set((float)Configuration::instance().sceneConfiguration().frontMaterialShininess());
	this->m_backMaterialAmbient->set(Converter::toVec3(Configuration::instance().sceneConfiguration().backMaterialAmbient()));
	this->m_backMaterialDiffuse->set(Converter::toVec3(Configuration::instance().sceneConfiguration().backMaterialDiffuse()));
	this->m_backMaterialSpecular->set(Converter::toVec3(Configuration::instance().sceneConfiguration().backMaterialSpecular()));
	this->m_backMaterialShininess->set((float)Configuration::instance().sceneConfiguration().backMaterialShininess());
	this->m_effectMaterialAmbient->set(Converter::toVec3(Configuration::instance().sceneConfiguration().effectMaterialAmbient()));
	this->m_effectMaterialDiffuse->set(Converter::toVec3(Configuration::instance().sceneConfiguration().effectMaterialDiffuse()));
	this->m_effectMaterialSpecular->set(Converter::toVec3(Configuration::instance().sceneConfiguration().effectMaterialSpecular()));
	this->m_effectMaterialShininess->set((float)Configuration::instance().sceneConfiguration().effectMaterialShininess());
}

void QVGRRenderingSettings::regist(StandardScene *scene) {
	if (!scene || !scene->content() || !scene->content()->node()) return;

	Scene::Screen::Camera *camera = scene->screen().camera();
	Scene::Screen::View *view = scene->screen().view();
	osg::StateSet *states = scene->content()->node()->getOrCreateStateSet();
	osg::ref_ptr<osg::Uniform> modelViewMatrix = new osg::Uniform("modelViewMatrix", camera->getViewMatrix());
	osg::ref_ptr<osg::Uniform> projectionMatrix = new osg::Uniform("projectionMatrix", camera->getProjectionMatrix());
	osg::ref_ptr<osg::Uniform> viewportMatrix = new osg::Uniform("viewportMatrix", camera->getViewport()->computeWindowMatrix());
	modelViewMatrix->setUpdateCallback(new ViewMatrixUniformCallback(camera));
	projectionMatrix->setUpdateCallback(new ProjectionMatrixUniformCallback(camera));
	viewportMatrix->setUpdateCallback(new ViewportMatrixUniformCallback(camera));
	states->addUniform(modelViewMatrix);
	states->addUniform(projectionMatrix);
	states->addUniform(viewportMatrix);
	states->addUniform(this->m_pointSize);
	states->addUniform(this->m_pointColor);
	states->addUniform(this->m_lineWidth);
	states->addUniform(this->m_lineColor);
	states->addUniform(this->m_lightIntensity);
	states->addUniform(this->m_frontMaterialAmbient);
	states->addUniform(this->m_frontMaterialDiffuse);
	states->addUniform(this->m_frontMaterialSpecular);
	states->addUniform(this->m_frontMaterialShininess);
	states->addUniform(this->m_backMaterialAmbient);
	states->addUniform(this->m_backMaterialDiffuse);
	states->addUniform(this->m_backMaterialSpecular);
	states->addUniform(this->m_backMaterialShininess);
	states->addUniform(this->m_effectMaterialAmbient);
	states->addUniform(this->m_effectMaterialDiffuse);
	states->addUniform(this->m_effectMaterialSpecular);
	states->addUniform(this->m_effectMaterialShininess);
	states->addUniform(this->m_orthographicMode);
	states->addUniform(this->m_vertexMode);
	states->addUniform(this->m_edgeMode);
	states->addUniform(this->m_faceMode);
}



size_t QVGRViewerWidget::ScenesCount = 4;

QVGRViewerWidget::QVGRViewerWidget(QVGRManager *manager, QWidget *parent) : QWidget(parent), m_manager(manager) {

	this->setThreadingModel(osgViewer::CompositeViewer::SingleThreaded);

	this->m_standards = QVector<StandardScene *>();
	this->m_scenes = QVector<Scene *>();
	this->m_layout = new QGridLayout();
	this->m_layout->setContentsMargins(1, 0, 1, 0);
	this->m_thread = new QVGRExecutionThread();
	
	this->setLayout(this->m_layout);
	this->addScenes();
	this->setupLayout();
	this->setAcceptDrops(true);

	connect(this->m_thread, SIGNAL(started()), this, SIGNAL(taskStarted()));
	connect(this->m_thread, SIGNAL(finished()), this, SIGNAL(taskFinished()));
	connect(this, SIGNAL(taskStarted()), this, SLOT(refresh()));
	connect(this, SIGNAL(taskFinished()), this, SLOT(refresh()));
}

void QVGRViewerWidget::setScenesAcceptDrops(bool on) {
	foreach (StandardScene *scene, this->m_standards) {
		scene->screen().widget()->setAcceptDrops(on);
	}
}

void QVGRViewerWidget::regist(QVGRModule *module) {
	this->m_modules.insert(module->name(), module);
	foreach (Scene *scene, module->getFunction().views()) {
		if (scene) this->addScene(scene);
	}
	module->getFunction().bind(this->m_standards.toStdVector());
	connect(module, SIGNAL(activated(QString)), this, SLOT(activateModule(QString)));
	connect(module, SIGNAL(modified(QString)), this, SLOT(modifyModule(QString)));
	connect(module, SIGNAL(updated(QString)), this, SLOT(updateModule(QString)));
	connect(module, SIGNAL(executionStarted(QString)), this, SLOT(executeModule(QString)));
}

void QVGRViewerWidget::dispose() {
	this->clearLayout();
	this->removeScenes();
	foreach (QVGRModule *module, this->m_modules) {
		if (!module) continue;
		foreach (Scene *scene, module->getFunction().views()) {
			if (scene) this->removeScene(scene);
		}
	}
}

void QVGRViewerWidget::displayProject() {

	// Disposes old layout, scenes and function bindings.
	foreach (QVGRModule *module, this->m_modules) {
		if (!module) continue;
		module->getFunction().reset();
	}

	// Create new layout, scenes, and function bindings.
	for (size_t i = 0, size = this->m_manager->size(); i < QVGRViewerWidget::ScenesCount; ++i) {
		StandardScene *scene = this->m_standards[i];
		scene->clearModel();
		if (i < size) {
			scene->setupModel(this->m_manager->item(i));
			StandardScene::sharedManipulator()->fitScene(scene);
		}
	}

	StandardScene::sharedManipulator()->reset();
	this->refresh();
}

void QVGRViewerWidget::updateSettings() {
	this->m_settings.refresh();
	foreach (StandardScene *scene, this->m_standards) {
		if (!scene) continue;
		scene->screen().camera()->setClearColor(Converter::toVec4(Configuration::instance().sceneConfiguration().sceneBackgroundColor()));
	}
	foreach (QVGRModule *module, this->m_modules) {
		if (!module) continue;
		foreach (Scene *scene, module->getFunction().views()) {
			if (!scene) continue;
			scene->screen().camera()->setClearColor(Converter::toVec4(Configuration::instance().sceneConfiguration().sceneBackgroundColor()));
		}
	}
	this->refresh();
}

void QVGRViewerWidget::paintEvent(QPaintEvent *event) {
	this->frame();
}

void QVGRViewerWidget::resizeEvent(QResizeEvent *event) {
	this->resizeLayout();
}

void QVGRViewerWidget::resizeLayout() {

	/* To be effective based on the assumption that all widgets share the same
	 * dimensions. Finds the first visible widget to flush the viewport of the
	 * context.
	 */
	if (this->m_scenes.empty()) return;
	Scene *scene = NULL;
	for (size_t i = 0; i < this->m_scenes.size(); ++i) if (scene = this->m_scenes[i]) break;
	foreach (QVGRModule *module, this->m_modules.values()) {
		module->getFunction().setViewport(scene->width(), scene->height());
	}
}

void QVGRViewerWidget::clearLayout() {
	for (size_t i = 0; i < this->m_scenes.size(); ++i) {
		if (this->m_scenes[i]) this->removeWindow(this->m_scenes[i]);
	}
	this->resizeLayout();
	this->refresh();
}

void QVGRViewerWidget::setupLayout() {
	for (size_t i = 0; i < this->m_scenes.size(); ++i) {
		if (this->m_scenes[i]) this->addWindow(this->m_scenes[i], i >> 1, i & 1);
	}
	this->resizeLayout();
	this->refresh();
}

void QVGRViewerWidget::addScenes() {
	this->m_standards.resize(QVGRViewerWidget::ScenesCount);
	this->m_scenes.resize(QVGRViewerWidget::ScenesCount);
	for (size_t i = 0; i < QVGRViewerWidget::ScenesCount; ++i) {
		StandardScene *scene = new StandardScene();
		this->addScene(scene);
		this->m_standards[i] = scene;
		this->m_scenes[i] = scene;
		this->m_settings.regist(scene);
		connect(scene->screen().widget(), SIGNAL(itemDropped(QWidget *, int)), this, SLOT(onItemDropped(QWidget *, int)));
		connect(scene->screen().widget(), SIGNAL(filesDropped(const QStringList &)), this, SIGNAL(filesDropped(const QStringList &)));
	}
}

void QVGRViewerWidget::removeScenes() {
	for (size_t i = 0; i < QVGRViewerWidget::ScenesCount; ++i) {
		StandardScene *scene = this->m_standards[i];
		this->removeScene(scene);
		delete scene;
	}
	this->m_standards.clear();
	this->m_scenes.clear();
}

void QVGRViewerWidget::addScene(Scene *scene) {
	if (!scene) return;
	this->addView(scene->screen().view());
	scene->screen().widget()->setViewer(this);
	scene->screen().widget()->setWidget(this);
	scene->screen().widget()->setResizeCallback(new ResizeCallback(scene));
}

void QVGRViewerWidget::removeScene(Scene *scene) {
	if (!scene) return;
	this->removeView(scene->screen().view());
}

void QVGRViewerWidget::addWindow(Scene *scene, int row, int column) {
	if (!scene || !this->m_layout) return;
	this->m_layout->addWidget(scene->screen().widget(), row, column);
	scene->screen().widget()->show();
}

void QVGRViewerWidget::removeWindow(Scene *scene) {
	if (!scene || !this->m_layout) return;
	scene->screen().widget()->hide();
	this->m_layout->removeWidget(scene->screen().widget());
}

void QVGRViewerWidget::refresh() {
	this->frame();
	this->update();
}

void QVGRViewerWidget::activateModule(QString name) {
	QVGRModule *module = this->m_modules[name];
	if (!module) return;
	module->getFunction().activate(module->getSettings());
	std::vector<Scene *> layout = module->getFunction().layout(module->getSettings());
	if (!layout.empty()) {
		this->clearLayout();
		this->m_scenes = QVector<Scene *>::fromStdVector(layout);
		this->setupLayout();
	}
	this->refresh();
}

void QVGRViewerWidget::modifyModule(QString name) {
	QVGRModule *module = this->m_modules[name];
	if (!module) return;
	module->getFunction().modify(module->getSettings());
	std::vector<Scene *> layout = module->getFunction().layout(module->getSettings());
	if (!layout.empty()) {
		this->clearLayout();
		this->m_scenes = QVector<Scene *>::fromStdVector(layout);
		this->setupLayout();
	}
	this->refresh();
}

void QVGRViewerWidget::updateModule(QString name) {
	QVGRModule *module = this->m_modules[name];
	if (!module) return;
	module->getFunction().update(module->getSettings());
	this->refresh();
}

void QVGRViewerWidget::executeModule(QString name) {
	QVGRModule *module = this->m_modules[name];
	if (!module) return;
	this->m_thread->setModule(module);
	this->m_thread->start();
}

void QVGRViewerWidget::onItemDropped(QWidget *widget, int index) {
	GraphWidget *graphWidget = qobject_cast<GraphWidget *>(widget);
	if (!graphWidget) return;

	foreach (StandardScene *scene, this->m_standards) {
		if (scene->screen().widget() == graphWidget) {
			scene->clearModel();
			scene->setupModel(this->m_manager->item(index));
			break;
		}
	}
}



#pragma warning (pop)
