#include "Context.hpp"
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/Node>
#include <osg/NodeVisitor>
#include <core/basic/Model.hpp>
#include <core/basic/Configuration.hpp>

#pragma warning (push)
#pragma warning (disable:4482)	/* nonstandard extension used: enum 'enum' used in qualified name */



class ViewMatrixUniformCallback;
class ProjectionMatrixUniformCallback;
class ViewportMatrixUniformCallback;



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



Context::RenderSettings::RenderSettings() {
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
	this->m_vertexMode				= new osg::Uniform("mode.vertex", false);
	this->m_edgeMode				= new osg::Uniform("mode.edge", false);
	this->m_faceMode				= new osg::Uniform("mode.face", true);
}

void Context::RenderSettings::updateSettings() {
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

void Context::regist(Scene *scene) {
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
	states->addUniform(this->m_renderSettings.pointSize());
	states->addUniform(this->m_renderSettings.pointColor());
	states->addUniform(this->m_renderSettings.lineWidth());
	states->addUniform(this->m_renderSettings.lineColor());
	states->addUniform(this->m_renderSettings.lightIntensity());
	states->addUniform(this->m_renderSettings.frontMaterialAmbient());
	states->addUniform(this->m_renderSettings.frontMaterialDiffuse());
	states->addUniform(this->m_renderSettings.frontMaterialSpecular());
	states->addUniform(this->m_renderSettings.frontMaterialShininess());
	states->addUniform(this->m_renderSettings.backMaterialAmbient());
	states->addUniform(this->m_renderSettings.backMaterialDiffuse());
	states->addUniform(this->m_renderSettings.backMaterialSpecular());
	states->addUniform(this->m_renderSettings.backMaterialShininess());
	states->addUniform(this->m_renderSettings.effectMaterialAmbient());
	states->addUniform(this->m_renderSettings.effectMaterialDiffuse());
	states->addUniform(this->m_renderSettings.effectMaterialSpecular());
	states->addUniform(this->m_renderSettings.effectMaterialShininess());
	states->addUniform(this->m_renderSettings.vertexMode());
	states->addUniform(this->m_renderSettings.edgeMode());
	states->addUniform(this->m_renderSettings.faceMode());
}



#pragma warning (pop)
