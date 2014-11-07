#pragma once
#ifndef _VGR_CORE_BASIC_SPRITE_COMPASS_HPP
#define _VGR_CORE_BASIC_SPRITE_COMPASS_HPP

#include <osg/Camera>



class Compass;
class CompassUpdateCallback;



class Compass : public osg::Camera {

public:
	Compass();
};



class CompassUpdateCallback : public osg::Camera::DrawCallback {

private:
	osg::Camera							*m_sceneCamera;
	osg::Camera							*m_compassCamera;

public:
	CompassUpdateCallback(osg::Camera *sceneCamera, osg::Camera *compassCamera) :
			m_sceneCamera(sceneCamera), m_compassCamera(compassCamera) { }

public:
	virtual void operator ()(osg::RenderInfo &info) const;
};



#endif /* _VGR_CORE_BASIC_SPRITE_COMPASS_HPP */
