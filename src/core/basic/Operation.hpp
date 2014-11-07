#pragma once
#ifndef _VGR_CORE_BASIC_OPERATION_HPP
#define _VGR_CORE_BASIC_OPERATION_HPP

#include <osgGA/GUIEventAdapter>
#include <osgGA/GUIActionAdapter>



class Handler;
class Operation;



class Operation : public osg::Referenced {

public:
	virtual ~Operation() { }

public:
	virtual void operator ()(const Handler &handler) { }
	virtual void operator ()(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa, const Handler &handler) { }

public:
	virtual void setViewport(int width, int height) { }
	virtual void dispose() { }
	virtual bool operate() { return (false); }
};



#endif /* _VGR_CORE_BASIC_OPERATION_HPP */
