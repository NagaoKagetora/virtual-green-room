#pragma once
#ifndef _VGR_CORE_BASIC_SPRITE_RECT_HPP
#define _VGR_CORE_BASIC_SPRITE_RECT_HPP

#include <osg/Geometry>
#include <osg/Geode>



class Rect;



class Rect : public osg::Geode {

private:
	osg::ref_ptr<osg::Vec3Array>		m_vertices;
	osg::ref_ptr<osg::Vec4Array>		m_fillColors;
	osg::ref_ptr<osg::DrawArrays>		m_fillPrimitives;
	osg::ref_ptr<osg::Geometry>			m_fillGeometry;
	osg::ref_ptr<osg::Vec4Array>		m_borderColors;
	osg::ref_ptr<osg::DrawArrays>		m_borderPrimitives;
	osg::ref_ptr<osg::Geometry>			m_borderGeometry;

public:
	Rect();

public:
	void setFillColor(osg::Vec4 color);
	void setBorderColor(osg::Vec4 color);
};



#endif /* _VGR_CORE_BASIC_SPRITE_RECT_HPP */
