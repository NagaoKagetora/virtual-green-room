#include "Hud.hpp"

#pragma warning (push)
#pragma warning (disable:4482)	/* nonstandard extension used: enum 'enum' used in qualified name */



Hud::Hud() : osg::Camera() {
	this->setReferenceFrame(osg::Transform::ReferenceFrame::ABSOLUTE_RF);
	this->setRenderOrder(osg::Camera::RenderOrder::POST_RENDER);
	this->setProjectionMatrixAsOrtho2D(0.0, 0.0, 0.0, 0.0);
	this->setViewMatrix(osg::Matrix::identity());
	this->setClearMask(GL_DEPTH_BUFFER_BIT);
	this->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::Values::OFF);
	this->getOrCreateStateSet()->setMode(GL_DEPTH_TEST, osg::StateAttribute::Values::OFF);
	this->getOrCreateStateSet()->setMode(GL_BLEND, osg::StateAttribute::Values::ON);
}



#pragma warning (pop)
