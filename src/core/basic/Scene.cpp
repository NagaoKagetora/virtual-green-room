#include "Scene.hpp"
#include <QtCore/QString>
#include <common/3rdparty/boost.hpp>
#include <core/basic/Model.hpp>
#include <core/basic/Configuration.hpp>

#pragma warning (push)
#pragma warning (disable:4482)	/* nonstandard extension used: enum 'enum' used in qualified name */



class AttributeBinding;



class AttributeBinding : public osg::NodeVisitor {

public:
	typedef std::pair<size_t, std::string>		Alias;

private:
	Alias								m_vertex;
	Alias								m_normal;
	Alias								m_color;
	Alias								m_secondaryColor;
	Alias								m_fogCoord;
	Alias								m_multiTexCoord[8];

public:
	AttributeBinding() : osg::NodeVisitor(osg::NodeVisitor::TRAVERSE_ALL_CHILDREN) {
		this->m_vertex = Alias(0, std::string("osg_Vertex"));
		this->m_normal = Alias(2, std::string("osg_Normal"));
		this->m_color = Alias(3, std::string("osg_Color"));
		this->m_secondaryColor = Alias(4, std::string("osg_SecondaryColor"));
		this->m_fogCoord = Alias(5, std::string("osg_FogCoord"));
		this->m_multiTexCoord[0] = Alias(8, std::string("osg_MultiTexCoord0"));
		this->m_multiTexCoord[1] = Alias(9, std::string("osg_MultiTexCoord1"));
		this->m_multiTexCoord[2] = Alias(10, std::string("osg_MultiTexCoord2"));
		this->m_multiTexCoord[3] = Alias(11, std::string("osg_MultiTexCoord3"));
		this->m_multiTexCoord[4] = Alias(12, std::string("osg_MultiTexCoord4"));
		this->m_multiTexCoord[5] = Alias(13, std::string("osg_MultiTexCoord5"));
		this->m_multiTexCoord[6] = Alias(14, std::string("osg_MultiTexCoord6"));
		this->m_multiTexCoord[7] = Alias(15, std::string("osg_MultiTexCoord7"));
	}

public:
	void apply(osg::Geode &geode) {
		for (size_t i = 0; i < geode.getNumDrawables(); ++i) {
			osg::Geometry *geometry = geode.getDrawable(i)->asGeometry();
			if (geometry) this->apply(*geometry);
		}
	}

private:
	void apply(osg::Geometry &geometry) {
		if (geometry.getVertexArray()) this->bind(this->m_vertex, geometry, geometry.getVertexArray(), osg::Geometry::BIND_PER_VERTEX);
		if (geometry.getNormalArray()) this->bind(this->m_normal, geometry, geometry.getNormalArray(), geometry.getNormalBinding());
		if (geometry.getColorArray()) this->bind(this->m_color, geometry, geometry.getColorArray(), geometry.getColorBinding());
		if (geometry.getSecondaryColorArray()) this->bind(this->m_secondaryColor, geometry, geometry.getSecondaryColorArray(), geometry.getSecondaryColorBinding());
		if (geometry.getFogCoordArray()) this->bind(this->m_fogCoord, geometry, geometry.getFogCoordArray(), geometry.getFogCoordBinding());
		size_t numTexCoords = std::min<size_t>(8, geometry.getNumTexCoordArrays());
		for (size_t i = 0; i < numTexCoords; ++i) {
			if (geometry.getTexCoordArray(i)) this->bind(this->m_multiTexCoord[i], geometry, geometry.getTexCoordArray(i), osg::Geometry::BIND_PER_VERTEX);
		}
	}
	void bind(const Alias &alias, osg::Geometry &geometry, osg::Array *array, osg::Geometry::AttributeBinding binding) {
		geometry.setVertexAttribArray(alias.first, array);
		geometry.setVertexAttribBinding(alias.first, binding);
		array->setName(alias.second);
	}
};



Scene::Screen::Screen() {
	osg::ref_ptr<osg::DisplaySettings> settings = osg::DisplaySettings::instance();
	osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits();
	traits->x = 0;
	traits->y = 0;
	traits->width = 100;
	traits->height = 100;
	traits->windowName = std::string();
	traits->windowDecoration = false;
	traits->doubleBuffer = true;
	traits->alpha = settings->getMinimumNumAlphaBits();
	traits->stencil = settings->getMinimumNumStencilBits();
	traits->sampleBuffers = settings->getMultiSamples();
	traits->samples = settings->getNumMultiSamples();
	this->m_window = new Window(traits);
	this->m_widget = this->m_window->graphWidget();
	this->m_camera = new Camera();
	this->m_camera->setGraphicsContext(this->m_window);
	this->m_camera->setClearColor(Converter::toVec4(Configuration::instance().sceneConfiguration().sceneBackgroundColor()));

	/* Disables small feature culling to display a scene with its bounding box
	 * volume has zero sizes (e.g. scene that only has a single point or coincident
	 * points) in a correct way.
	 */
	this->m_camera->setCullingMode(this->m_camera->getCullingMode() & ~osg::CullSettings::CullingModeValues::SMALL_FEATURE_CULLING);
	this->m_camera->setViewport(traits->x, traits->y, traits->width, traits->height);
	this->m_camera->setProjectionMatrixAsPerspective(30.0, 1.0, 1.0, 10000.0);
	this->m_view = new View();
	this->m_view->setCamera(this->m_camera);
}

void Scene::Screen::setViewport(int width, int height) {
	this->m_camera->setViewport(0, 0, width, height);
}



StandardScene::Program::Program() {
	this->m_vertShader = new osg::Shader(osg::Shader::VERTEX);
	this->m_vertShader->loadShaderSourceFromFile(std::string("./runtime/shader/composite.vert"));
	this->m_fragShader = new osg::Shader(osg::Shader::FRAGMENT);
	this->m_fragShader->loadShaderSourceFromFile(std::string("./runtime/shader/composite.frag"));
	this->m_geomShader = new osg::Shader(osg::Shader::GEOMETRY);
	this->m_geomShader->loadShaderSourceFromFile(std::string("./runtime/shader/composite.geom"));
	this->m_program = new osg::Program();
	this->m_program->addShader(this->m_vertShader);
	this->m_program->addShader(this->m_geomShader);
	this->m_program->addShader(this->m_fragShader);
	this->m_program->addBindFragDataLocation(std::string("osg_FragColor"), 0);
}



int StandardScene::Sprite::CurrentSerial = 0;

StandardScene::Sprite::Sprite() {
	this->init();
	this->setupModel(NULL);
}

StandardScene::Sprite::Sprite(Model *model) {
	this->init();
	this->setupModel(model);
}

void StandardScene::Sprite::setupModel(Model *model) {
	this->clearModel();
	if (!model) return;
	this->m_nameText->setText(model->name().toStdString());
	this->m_textHud->dirtyBound();
	this->m_node->dirtyBound();
}

void StandardScene::Sprite::clearModel() {
	this->m_nameText->setText(std::string());
	this->m_textHud->dirtyBound();
	this->m_indicator->removeChildren(0, this->m_indicator->getNumChildren());
	this->m_indicator->dirtyBound();
	this->m_decorator->removeChildren(0, this->m_decorator->getNumChildren());
	this->m_decorator->dirtyBound();
	this->m_node->dirtyBound();
}

void StandardScene::Sprite::setCaptionEnabled(bool enabled) {
	this->m_fixture->setValue(0, enabled);
	this->m_fixture->dirtyBound();
	this->m_node->dirtyBound();
}

void StandardScene::Sprite::setCompassEnabled(bool enabled) {
	this->m_fixture->setValue(1, enabled);
	this->m_fixture->dirtyBound();
	this->m_node->dirtyBound();
}

void StandardScene::Sprite::addIndicator(osg::Node *indicator) {
	this->m_indicator->addChild(indicator);
	this->m_indicator->dirtyBound();
	this->m_node->dirtyBound();
}

void StandardScene::Sprite::addIndicator(const std::string &name, osg::Node *indicator) {
	indicator->setName(name);
	this->m_indicator->addChild(indicator);
	this->m_indicator->dirtyBound();
	this->m_node->dirtyBound();
}

void StandardScene::Sprite::removeIndicator(osg::Node *indicator) {
	this->m_indicator->removeChild(indicator);
	this->m_indicator->dirtyBound();
	this->m_node->dirtyBound();
}

void StandardScene::Sprite::removeIndicator(const std::string &name) {
	for (size_t i = 0; i < this->m_indicator->getNumChildren(); ++i) {
		if (this->m_indicator->getChild(i)->getName() == name) {
			this->m_indicator->removeChild(i);
			this->m_indicator->dirtyBound();
			this->m_node->dirtyBound();
			break;
		}
	}
}

void StandardScene::Sprite::addDecorator(osg::Node *decorator) {
	this->m_decorator->addChild(decorator);
	this->m_decorator->dirtyBound();
	this->m_node->dirtyBound();
}

void StandardScene::Sprite::addDecorator(const std::string &name, osg::Node *decorator) {
	decorator->setName(name);
	this->m_decorator->addChild(decorator);
	this->m_decorator->dirtyBound();
	this->m_node->dirtyBound();
}

void StandardScene::Sprite::removeDecorator(osg::Node *decorator) {
	this->m_decorator->removeChild(decorator);
	this->m_decorator->dirtyBound();
	this->m_node->dirtyBound();
}

void StandardScene::Sprite::removeDecorator(const std::string &name) {
	for (size_t i = 0; i < this->m_decorator->getNumChildren(); ++i) {
		if (this->m_decorator->getChild(i)->getName() == name) {
			this->m_decorator->removeChild(i);
			this->m_decorator->dirtyBound();
			this->m_node->dirtyBound();
			break;
		}
	}
}

void StandardScene::Sprite::init() {

	this->m_serialText = new Text();
	this->m_serialText->setFont(Configuration::instance().spriteConfiguration().largeFontName().toStdString());
	this->m_serialText->setFontSize(Configuration::instance().spriteConfiguration().largeFontSize());
	this->m_serialText->setColor(Converter::toVec4(Configuration::instance().spriteConfiguration().largeFontColor()));
	this->m_serialText->setText(++StandardScene::Sprite::CurrentSerial);
	this->m_nameText = new Text();
	this->m_nameText->setFont(Configuration::instance().spriteConfiguration().smallFontName().toStdString());
	this->m_nameText->setFontSize(Configuration::instance().spriteConfiguration().smallFontSize());
	this->m_nameText->setColor(Converter::toVec4(Configuration::instance().spriteConfiguration().smallFontColor()));
	this->m_textHud = new Hud();
	this->m_textHud->addChild(this->m_serialText);
	this->m_textHud->addChild(this->m_nameText);
	this->m_compass = new Compass();
	this->m_fixture = new osg::Switch();
	this->m_fixture->setNodeMask(StandardScene::NodeMask::Visible);
	this->m_fixture->addChild(this->m_textHud);
	this->m_fixture->addChild(this->m_compass);
	this->m_fixture->setAllChildrenOn();
	this->m_indicator = new osg::Group();
	this->m_indicator->setNodeMask(StandardScene::NodeMask::Visible);
	this->m_decorator = new osg::Group();
	this->m_decorator->setNodeMask(StandardScene::NodeMask::Visible);
	this->m_node = new osg::Group();
	this->m_node->setNodeMask(StandardScene::NodeMask::Visible);
	this->m_node->addChild(this->m_fixture);
	this->m_node->addChild(this->m_indicator);
	this->m_node->addChild(this->m_decorator);
}

void StandardScene::Sprite::setViewport(int width, int height) {
	this->m_serialText->setPosition(20.0f, height - 40.0f);
	this->m_nameText->setPosition(50.0f, height - 24.0f);
	this->m_textHud->setProjectionMatrixAsOrtho2D(0, width, 0, height);
	this->m_textHud->dirtyBound();
	this->m_node->dirtyBound();
}



StandardScene::Content::Content() {
	this->init();
	this->setupModel(NULL);
}

StandardScene::Content::Content(Model *model) {
	this->init();
	this->setupModel(model);
}

void StandardScene::Content::setupModel(Model *model) {
	this->clearModel();
	if (!model) return;
	this->m_vertex->addChild(model->vertexNode());
	this->m_vertex->dirtyBound();
	this->m_edge->addChild(model->edgeNode());
	this->m_edge->dirtyBound();
	this->m_face->addChild(model->faceNode());
	this->m_face->dirtyBound();
	this->m_node->dirtyBound();
}

void StandardScene::Content::clearModel() {
	this->m_vertex->removeChildren(0, this->m_vertex->getNumChildren());
	this->m_vertex->dirtyBound();
	this->m_edge->removeChildren(0, this->m_edge->getNumChildren());
	this->m_edge->dirtyBound();
	this->m_face->removeChildren(0, this->m_face->getNumChildren());
	this->m_face->dirtyBound();
	this->m_node->dirtyBound();
}

void StandardScene::Content::init() {
	this->m_vertex = new osg::Group();
	this->m_vertex->setNodeMask(StandardScene::NodeMask::Pickable);
	this->m_edge = new osg::Group();
	this->m_edge->setNodeMask(StandardScene::NodeMask::Pickable);
	this->m_face = new osg::Group();
	this->m_face->setNodeMask(StandardScene::NodeMask::All);
	this->m_node = new osg::Group();
	this->m_node->addChild(this->m_vertex);
	this->m_node->addChild(this->m_edge);
	this->m_node->addChild(this->m_face);
}



void StandardScene::Manipulator::changeMode(ProjectionMode mode) {
	if (this->m_mode == mode) return;
	if (this->m_cameras.empty()) return;
	this->m_mode = mode;
	if (this->m_mode == ProjectionMode::Orthographic) {
		this->updateOrthography();
	} else {
		this->updatePerspective();
	}
}

void StandardScene::Manipulator::reset() {
	this->m_rotationEnabled = true;
	this->m_translationEnabled = true;
	this->m_zoomingEnabled = true;
	this->setAllowThrow(false);
}

void StandardScene::Manipulator::updateOrthography() {
	if (this->m_mode == ProjectionMode::Perspective) return;
	if (this->m_cameras.empty()) return;
	real_t ratio = this->m_cameras.back()->getViewport()->aspectRatio();
	real_t height = this->getDistance() * std::tan(0.5 * this->m_fov * VGR_RAD_PER_DGR);
	real_t width = height * ratio;
	for (size_t i = 0; i < this->m_cameras.size(); ++i) {
		this->m_cameras[i]->setProjectionMatrixAsOrtho(-width, width, -height, height, -VGR_REAL_MAX, VGR_REAL_MAX);
	}
}

void StandardScene::Manipulator::updatePerspective() {
	if (this->m_mode == ProjectionMode::Orthographic) return;
	if (this->m_cameras.empty()) return;
	real_t ratio = this->m_cameras.back()->getViewport()->aspectRatio();
	for (size_t i = 0; i < this->m_cameras.size(); ++i) {
		this->m_cameras[i]->setProjectionMatrixAsPerspective(this->m_fov, ratio, 1.0, 10000.0);
	}
}

void StandardScene::Manipulator::fitScene(const StandardScene *scene) {
	if (!scene) return;
	const osg::BoundingSphere &bound(scene->content()->node()->getBound());
	real_t distance = bound.radius() / std::tan(0.5 * this->m_fov * VGR_RAD_PER_DGR);
	this->setByMatrix(osg::Matrixd::identity());
	this->setTransformation(bound.center() + osg::Vec3(0.0f, 0.0f, distance), bound.center(), osg::Vec3(0.0f, 1.0f, 0.0f));
}

void StandardScene::Manipulator::setTransformation(const osg::Vec3d &eye, const osg::Quat &rotation) {
	TrackballManipulator::setTransformation(eye, rotation);
	if (this->m_mode == ProjectionMode::Orthographic) this->updateOrthography();
}

void StandardScene::Manipulator::setTransformation(const osg::Vec3d &eye, const osg::Vec3d &center, const osg::Vec3d &up) {
	TrackballManipulator::setTransformation(eye, center, up);
	if (this->m_mode == ProjectionMode::Orthographic) this->updateOrthography();
}

bool StandardScene::Manipulator::handle(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &us) {
	if (ea.getModKeyMask()) return (false);
	if (!this->m_rotationEnabled && (ea.getButtonMask() == osgGA::GUIEventAdapter::MouseButtonMask::LEFT_MOUSE_BUTTON)) return (false);
	if (!this->m_translationEnabled && (ea.getButtonMask() == osgGA::GUIEventAdapter::MouseButtonMask::MIDDLE_MOUSE_BUTTON)) return (false);
	if (!this->m_translationEnabled && (ea.getButtonMask() == (osgGA::GUIEventAdapter::MouseButtonMask::LEFT_MOUSE_BUTTON | osgGA::GUIEventAdapter::MouseButtonMask::RIGHT_MOUSE_BUTTON))) return (false);
	if (!this->m_zoomingEnabled && (ea.getButtonMask() == osgGA::GUIEventAdapter::MouseButtonMask::RIGHT_MOUSE_BUTTON)) return (false);
	if (!this->m_zoomingEnabled && (ea.getEventType() == osgGA::GUIEventAdapter::EventType::SCROLL)) return (false);
	
	if (this->m_mode == ProjectionMode::Orthographic) {
		if (ea.getEventType() == osgGA::GUIEventAdapter::EventType::SCROLL) {
			bool result = osgGA::TrackballManipulator::handle(ea, us);
			if (result) {
				this->updateOrthography();
				return (true);
			}
			return (false);
		} else if ((ea.getEventType() == osgGA::GUIEventAdapter::EventType::DRAG) && (ea.getButtonMask() == osgGA::GUIEventAdapter::MouseButtonMask::RIGHT_MOUSE_BUTTON)) {
			bool result = osgGA::TrackballManipulator::handle(ea, us);
			if (result) {
				this->updateOrthography();
				return (true);
			}
			return (false);
		}
	}

	return (osgGA::TrackballManipulator::handle(ea, us));
}



int StandardScene::CurrentSerial = 0;

StandardScene::StandardScene() : Scene(), m_serial(++StandardScene::CurrentSerial), m_caption(), m_ready(false), m_model(NULL) {
	this->m_program = new Program();
	this->m_content = new Content();
	this->m_content->node()->accept(AttributeBinding());
	this->m_content->node()->getOrCreateStateSet()->setAttributeAndModes(this->m_program->program());
	this->m_sprite = new Sprite();
	this->m_root = new osg::Group();
	this->m_root->addChild(this->m_content->node());
	this->m_root->addChild(this->m_sprite->node());
	this->screen().camera()->setCullMask(StandardScene::NodeMask::Visible);
	this->screen().camera()->setPostDrawCallback(new CompassUpdateCallback(this->screen().camera(), this->m_sprite->compass()));
	this->screen().view()->setSceneData(this->m_root);
	this->screen().view()->setCameraManipulator(StandardScene::sharedManipulator());
	StandardScene::sharedManipulator()->addCamera(this->screen().camera());
}

void StandardScene::setupModel(Model *model) {
	if (!model) return;

	this->m_model = model;
	this->m_content->setupModel(model);
	this->m_sprite->setupModel(model);
	this->m_root->dirtyBound();
	this->m_caption = model->name().toStdString();
	this->m_ready = true;
}

void StandardScene::clearModel() {
	if (!this->m_model) return;

	this->m_model = NULL;
	this->m_content->clearModel();
	this->m_sprite->clearModel();
	this->m_root->dirtyBound();
	this->m_caption = std::string();
	this->m_ready = false;
}

void StandardScene::addHandler(Handler *handler) {
	if (!handler || !this->ready()) return;
	this->m_screen.view()->addEventHandler(handler);
	this->m_sprite->addIndicator(handler->indicator());
}

void StandardScene::removeHandler(Handler *handler) {
	if (!handler || !this->ready()) return;
	this->m_sprite->removeIndicator(handler->indicator());
	this->m_screen.view()->removeEventHandler(handler);
}

void StandardScene::setViewport(int width, int height) {
	if (width < 0) width = this->width();
	if (height < 0) height = this->height();

	this->m_content->setViewport(width, height);
	this->m_sprite->setViewport(width, height);

	Scene::setViewport(width, height);
}



#pragma warning (pop)
