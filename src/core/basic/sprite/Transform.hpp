#pragma once
#ifndef _VGR_CORE_BASIC_SPRITE_TRANSFORM_HPP
#define _VGR_CORE_BASIC_SPRITE_TRANSFORM_HPP

#include <osg/PositionAttitudeTransform>



class Transform2D;
class Transform3D;



class Transform2D : public osg::PositionAttitudeTransform {

public:
	Transform2D() : osg::PositionAttitudeTransform() { }

public:
	using osg::PositionAttitudeTransform::setPosition;
	void setPosition(osg::Vec2 position) { this->setPosition(position.x(), position.y()); }
	void setPosition(float x, float y) { this->setPosition(osg::Vec3d((double)x, (double)y, 0.0)); }
	void setSize(osg::Vec2 position) { this->setSize(position.x(), position.y()); }
	void setSize(float x, float y) { this->setScale(osg::Vec3d((double)x, (double)y, 0.0)); }
};



class Transform3D : public osg::PositionAttitudeTransform {

public:
	Transform3D() : osg::PositionAttitudeTransform() { }
};



#endif /* _VGR_CORE_BASIC_SPRITE_TRANSFORM_HPP */
