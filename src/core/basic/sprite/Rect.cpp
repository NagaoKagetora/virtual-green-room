#include "Rect.hpp"

#pragma warning (push)
#pragma warning (disable:4482)	/* nonstandard extension used: enum 'enum' used in qualified name */



Rect::Rect() : osg::Geode() {

	this->m_vertices = new osg::Vec3Array();
	this->m_vertices->push_back(osg::Vec3(0.0f, 0.0f, 0.0f));
	this->m_vertices->push_back(osg::Vec3(1.0f, 0.0f, 0.0f));
	this->m_vertices->push_back(osg::Vec3(1.0f, 1.0f, 0.0f));
	this->m_vertices->push_back(osg::Vec3(0.0f, 1.0f, 0.0f));
	this->m_fillColors = new osg::Vec4Array();
	this->m_fillColors->push_back(osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f));
	this->m_fillPrimitives = new osg::DrawArrays(osg::PrimitiveSet::Mode::QUADS, 0, 4);
	this->m_fillGeometry = new osg::Geometry();
	this->m_fillGeometry->setVertexArray(this->m_vertices);
	this->m_fillGeometry->setColorArray(this->m_fillColors);
	this->m_fillGeometry->setColorBinding(osg::Geometry::AttributeBinding::BIND_OVERALL);
	this->m_fillGeometry->addPrimitiveSet(this->m_fillPrimitives);
	this->m_borderColors = new osg::Vec4Array();
	this->m_borderColors->push_back(osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f));
	this->m_borderPrimitives = new osg::DrawArrays(osg::PrimitiveSet::Mode::LINE_LOOP, 0, 4);
	this->m_borderGeometry = new osg::Geometry();
	this->m_borderGeometry->setVertexArray(this->m_vertices);
	this->m_borderGeometry->setColorArray(this->m_borderColors);
	this->m_borderGeometry->setColorBinding(osg::Geometry::AttributeBinding::BIND_OVERALL);
	this->m_borderGeometry->addPrimitiveSet(this->m_borderPrimitives);
	this->addDrawable(this->m_fillGeometry);
	this->addDrawable(this->m_borderGeometry);
}

void Rect::setFillColor(osg::Vec4 color) {
	this->m_fillColors->at(0) = color;
	this->m_fillColors->dirty();
	this->m_fillGeometry->dirtyDisplayList();
	this->m_fillGeometry->dirtyBound();
	this->dirtyBound();
}

void Rect::setBorderColor(osg::Vec4 color) {
	this->m_borderColors->at(0) = color;
	this->m_borderColors->dirty();
	this->m_borderGeometry->dirtyDisplayList();
	this->m_borderGeometry->dirtyBound();
	this->dirtyBound();
}



#pragma warning (pop)
