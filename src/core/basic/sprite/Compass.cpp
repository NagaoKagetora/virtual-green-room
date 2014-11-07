#include "Compass.hpp"
#include <osgDB/ReadFile>

#pragma warning (push)
#pragma warning (disable:4482)	/* nonstandard extension used: enum 'enum' used in qualified name */



Compass::Compass() {
	this->setReferenceFrame(osg::Transform::ReferenceFrame::ABSOLUTE_RF);
	this->setRenderOrder(osg::Camera::RenderOrder::POST_RENDER);
	this->setProjectionMatrixAsPerspective(15.0, 1.0, 1.0, 10000.0);
	this->setViewMatrixAsLookAt(osg::Vec3d(0.0, 0.0, 10.0), osg::Vec3d(0.0, 0.0, 0.0), osg::Vec3d(0.0, 1.0, 0.0));
	this->setViewport(0, 0, 100, 100);
	this->setClearMask(GL_DEPTH_BUFFER_BIT);
	this->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::Values::OFF);
	this->addChild(osgDB::readNodeFile("./runtime/compass/compass.obj"));
}



void CompassUpdateCallback::operator ()(osg::RenderInfo &info) const {
	osg::Quat rotation;
	this->m_sceneCamera->getViewMatrix().decompose(osg::Vec3d(), rotation, osg::Vec3d(), osg::Quat());
	this->m_compassCamera->setViewMatrix(osg::Matrixd::rotate(rotation) * osg::Matrixd::lookAt(osg::Vec3d(0.0, 0.0, 10.0), osg::Vec3d(0.0, 0.0, 0.0), osg::Vec3d(0.0, 1.0, 0.0)));
}



#pragma warning (pop)
