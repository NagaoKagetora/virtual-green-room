#pragma once
#ifndef _VGR_CORE_BASIC_INDICATOR_HPP
#define _VGR_CORE_BASIC_INDICATOR_HPP

#include <osg/Camera>
#include <osg/Geode>
#include <osg/Geometry>



class Handler;
class Indicator;
class RectangleIndicator;
class SegmentIndicator;



class Indicator : public osg::Camera {

public:
	virtual void setGeometry(osg::Vec2 start, osg::Vec2 stop) { }
	virtual void setViewport(int width, int height) {
		this->setProjectionMatrixAsOrtho2D(0, (double)width, 0, (double)height);
	}
};



class RectangleIndicator : public Indicator {

private:
	osg::ref_ptr<osg::Vec3Array>		m_vertices;
	osg::ref_ptr<osg::Vec4Array>		m_borderColors;
	osg::ref_ptr<osg::DrawArrays>		m_borderPrimitives;
	osg::ref_ptr<osg::Geometry>			m_borderGeometry;
	osg::ref_ptr<osg::Vec4Array>		m_fillColors;
	osg::ref_ptr<osg::DrawArrays>		m_fillPrimitives;
	osg::ref_ptr<osg::Geometry>			m_fillGeometry;
	osg::ref_ptr<osg::Geode>			m_geode;

public:
	RectangleIndicator();

public:
	void setBorderColor(osg::Vec4 color);
	void setFillColor(osg::Vec4 color);

public:
	virtual void setGeometry(osg::Vec2 start, osg::Vec2 stop);
};



class SegmentIndicator : public Indicator {

protected:
	osg::ref_ptr<osg::Vec3Array>		m_vertices;
	osg::ref_ptr<osg::Vec4Array>		m_backgroundColors;
	osg::ref_ptr<osg::DrawArrays>		m_backgroundLinePrimitives;
	osg::ref_ptr<osg::DrawArrays>		m_backgroundPointPrimitives;
	osg::ref_ptr<osg::Geometry>			m_backgroundGeometry;
	osg::ref_ptr<osg::Vec4Array>		m_foregroundColors;
	osg::ref_ptr<osg::DrawArrays>		m_foregroundLinePrimitives;
	osg::ref_ptr<osg::DrawArrays>		m_foregroundPointPrimitives;
	osg::ref_ptr<osg::Geometry>			m_foregroundGeometry;
	osg::ref_ptr<osg::Geode>			m_geode;

public:
	SegmentIndicator();

public:
	void setBackgroundColor(osg::Vec4 color);
	void setStartPointColor(osg::Vec4 color);
	void setStopPointColor(osg::Vec4 color);
	void setLineColor(osg::Vec4 color);

public:
	virtual void setGeometry(osg::Vec2 start, osg::Vec2 stop);
};



#endif /* _VGR_CORE_BASIC_INDICATOR_HPP */
