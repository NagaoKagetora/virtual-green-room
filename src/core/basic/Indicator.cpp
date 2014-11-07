#include "Indicator.hpp"
#include <common/global.hpp>
#include <osg/Point>
#include <osg/LineWidth>

#pragma warning (push)
#pragma warning (disable:4482)	/* nonstandard extension used: enum 'enum' used in qualified name */



RectangleIndicator::RectangleIndicator() : Indicator() {

	this->m_vertices = new osg::Vec3Array();
	this->m_vertices->insert(this->m_vertices->begin(), 4, osg::Vec3(-VGR_FLOAT32_MAX, -VGR_FLOAT32_MAX, 0.0f));
	this->m_borderColors = new osg::Vec4Array();
	this->m_borderColors->push_back(osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f));
	this->m_borderPrimitives = new osg::DrawArrays(osg::PrimitiveSet::Mode::LINE_LOOP, 0, 4);
	this->m_borderGeometry = new osg::Geometry();
	this->m_borderGeometry->setVertexArray(this->m_vertices);
	this->m_borderGeometry->setColorArray(this->m_borderColors);
	this->m_borderGeometry->setColorBinding(osg::Geometry::AttributeBinding::BIND_OVERALL);
	this->m_borderGeometry->addPrimitiveSet(this->m_borderPrimitives);
	this->m_fillColors = new osg::Vec4Array();
	this->m_fillColors->push_back(osg::Vec4(0.0f, 0.0f, 0.0f, 0.0f));
	this->m_fillPrimitives = new osg::DrawArrays(osg::PrimitiveSet::Mode::QUADS, 0, 4);
	this->m_fillGeometry = new osg::Geometry();
	this->m_fillGeometry->setVertexArray(this->m_vertices);
	this->m_fillGeometry->setColorArray(this->m_fillColors);
	this->m_fillGeometry->setColorBinding(osg::Geometry::AttributeBinding::BIND_OVERALL);
	this->m_fillGeometry->addPrimitiveSet(this->m_fillPrimitives);
	this->m_geode = new osg::Geode();
	this->m_geode->addDrawable(this->m_borderGeometry);
	this->m_geode->addDrawable(this->m_fillGeometry);
	this->m_geode->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::Values::OFF);
	this->m_geode->getOrCreateStateSet()->setMode(GL_DEPTH_TEST, osg::StateAttribute::Values::OFF);
	this->m_geode->getOrCreateStateSet()->setMode(GL_BLEND, osg::StateAttribute::Values::ON);
	
	this->addChild(this->m_geode);
	this->setReferenceFrame(osg::Transform::ReferenceFrame::ABSOLUTE_RF);
	this->setRenderOrder(osg::Camera::RenderOrder::POST_RENDER);
	this->setProjectionMatrixAsOrtho2D(0, 0, 0, 0);
	this->setViewMatrix(osg::Matrix::identity());
	this->setClearMask(GL_DEPTH_BUFFER_BIT);
}

void RectangleIndicator::setBorderColor(osg::Vec4 color) {
	this->m_borderColors->at(0) = color;
	this->m_borderColors->dirty();
	this->m_borderGeometry->dirtyDisplayList();
	this->m_borderGeometry->dirtyBound();
	this->m_geode->dirtyBound();
}

void RectangleIndicator::setFillColor(osg::Vec4 color) {
	this->m_fillColors->at(0) = color;
	this->m_fillColors->dirty();
	this->m_fillGeometry->dirtyDisplayList();
	this->m_fillGeometry->dirtyBound();
	this->m_geode->dirtyBound();
}

void RectangleIndicator::setGeometry(osg::Vec2 start, osg::Vec2 stop) {
	osg::Vec2 min(std::min<float>(start.x(), stop.x()), std::min<float>(start.y(), stop.y()));
	osg::Vec2 max(std::max<float>(start.x(), stop.x()), std::max<float>(start.y(), stop.y()));
	this->m_vertices->at(0) = osg::Vec3(min.x(), min.y(), 0.0f);
	this->m_vertices->at(1) = osg::Vec3(max.x(), min.y(), 0.0f);
	this->m_vertices->at(2) = osg::Vec3(max.x(), max.y(), 0.0f);
	this->m_vertices->at(3) = osg::Vec3(min.x(), max.y(), 0.0f);
	this->m_vertices->dirty();
	this->m_borderGeometry->dirtyDisplayList();
	this->m_borderGeometry->dirtyBound();
	this->m_fillGeometry->dirtyDisplayList();
	this->m_fillGeometry->dirtyBound();
	this->m_geode->dirtyBound();
}



SegmentIndicator::SegmentIndicator() : Indicator() {

	this->m_vertices = new osg::Vec3Array();
	this->m_vertices->insert(this->m_vertices->begin(), 4, osg::Vec3(-VGR_FLOAT32_MAX, -VGR_FLOAT32_MAX, 0.0f));
	this->m_backgroundColors = new osg::Vec4Array();
	this->m_backgroundColors->push_back(osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f));
	this->m_backgroundLinePrimitives = new osg::DrawArrays(osg::PrimitiveSet::Mode::LINES, 0, 2);
	this->m_backgroundPointPrimitives = new osg::DrawArrays(osg::PrimitiveSet::Mode::POINTS, 2, 2);
	this->m_backgroundGeometry = new osg::Geometry();
	this->m_backgroundGeometry->setVertexArray(this->m_vertices);
	this->m_backgroundGeometry->setColorArray(this->m_backgroundColors);
	this->m_backgroundGeometry->setColorBinding(osg::Geometry::AttributeBinding::BIND_OVERALL);
	this->m_backgroundGeometry->addPrimitiveSet(this->m_backgroundLinePrimitives);
	this->m_backgroundGeometry->addPrimitiveSet(this->m_backgroundPointPrimitives);
	this->m_backgroundGeometry->getOrCreateStateSet()->setAttribute(new osg::Point(10.0f), osg::StateAttribute::Values::ON);
	this->m_backgroundGeometry->getOrCreateStateSet()->setAttribute(new osg::LineWidth(6.0f), osg::StateAttribute::Values::ON);
	this->m_foregroundColors = new osg::Vec4Array();
	this->m_foregroundColors->push_back(osg::Vec4(0.5f, 0.5f, 0.5f, 1.0f));
	this->m_foregroundColors->push_back(osg::Vec4(0.5f, 0.5f, 0.5f, 1.0f));
	this->m_foregroundColors->push_back(osg::Vec4(0.0f, 0.0f, 0.1f, 1.0f));
	this->m_foregroundColors->push_back(osg::Vec4(1.0f, 0.0f, 0.0f, 1.0f));
	this->m_foregroundLinePrimitives = new osg::DrawArrays(osg::PrimitiveSet::Mode::LINES, 0, 2);
	this->m_foregroundPointPrimitives = new osg::DrawArrays(osg::PrimitiveSet::Mode::POINTS, 2, 2);
	this->m_foregroundGeometry = new osg::Geometry();
	this->m_foregroundGeometry->setVertexArray(this->m_vertices);
	this->m_foregroundGeometry->setColorArray(this->m_foregroundColors);
	this->m_foregroundGeometry->setColorBinding(osg::Geometry::AttributeBinding::BIND_PER_VERTEX);
	this->m_foregroundGeometry->addPrimitiveSet(this->m_foregroundLinePrimitives);
	this->m_foregroundGeometry->addPrimitiveSet(this->m_foregroundPointPrimitives);
	this->m_foregroundGeometry->getOrCreateStateSet()->setAttribute(new osg::Point(6.0f), osg::StateAttribute::Values::ON);
	this->m_foregroundGeometry->getOrCreateStateSet()->setAttribute(new osg::LineWidth(2.0f), osg::StateAttribute::Values::ON);
	this->m_geode = new osg::Geode();
	this->m_geode->addDrawable(this->m_backgroundGeometry);
	this->m_geode->addDrawable(this->m_foregroundGeometry);
	this->m_geode->getOrCreateStateSet()->setMode(GL_LINE_SMOOTH, osg::StateAttribute::Values::ON);
	this->m_geode->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::Values::OFF);
	this->m_geode->getOrCreateStateSet()->setMode(GL_DEPTH_TEST, osg::StateAttribute::Values::OFF);
	this->m_geode->getOrCreateStateSet()->setMode(GL_BLEND, osg::StateAttribute::Values::ON);
	
	this->addChild(this->m_geode);
	this->setReferenceFrame(osg::Transform::ReferenceFrame::ABSOLUTE_RF);
	this->setRenderOrder(osg::Camera::RenderOrder::POST_RENDER);
	this->setProjectionMatrixAsOrtho2D(0, 0, 0, 0);
	this->setViewMatrix(osg::Matrix::identity());
	this->setClearMask(GL_DEPTH_BUFFER_BIT);
}

void SegmentIndicator::setBackgroundColor(osg::Vec4 color) {
	this->m_backgroundColors->at(0) = color;
	this->m_backgroundColors->dirty();
	this->m_backgroundGeometry->dirtyDisplayList();
	this->m_backgroundGeometry->dirtyBound();
	this->m_geode->dirtyBound();
}

void SegmentIndicator::setLineColor(osg::Vec4 color) {
	this->m_foregroundColors->at(0) = color;
	this->m_foregroundColors->at(1) = color;
	this->m_foregroundColors->dirty();
	this->m_foregroundGeometry->dirtyDisplayList();
	this->m_foregroundGeometry->dirtyBound();
	this->m_geode->dirtyBound();
}

void SegmentIndicator::setStartPointColor(osg::Vec4 color) {
	this->m_foregroundColors->at(2) = color;
	this->m_foregroundColors->dirty();
	this->m_foregroundGeometry->dirtyDisplayList();
	this->m_foregroundGeometry->dirtyBound();
	this->m_geode->dirtyBound();
}

void SegmentIndicator::setStopPointColor(osg::Vec4 color) {
	this->m_foregroundColors->at(3) = color;
	this->m_foregroundColors->dirty();
	this->m_foregroundGeometry->dirtyDisplayList();
	this->m_foregroundGeometry->dirtyBound();
	this->m_geode->dirtyBound();
}

void SegmentIndicator::setGeometry(osg::Vec2 start, osg::Vec2 stop) {
	this->m_vertices->at(0) = osg::Vec3(start, 0.0f);
	this->m_vertices->at(1) = osg::Vec3(stop, 0.0f);
	this->m_vertices->at(2) = osg::Vec3(start, 0.0f);
	this->m_vertices->at(3) = osg::Vec3(stop, 0.0f);
	this->m_vertices->dirty();
	this->m_backgroundGeometry->dirtyDisplayList();
	this->m_backgroundGeometry->dirtyBound();
	this->m_foregroundGeometry->dirtyDisplayList();
	this->m_foregroundGeometry->dirtyBound();
	this->m_geode->dirtyBound();
}



#pragma warning (pop)
