#pragma once
#ifndef _VGR_CORE_REFLECTION_HPP
#define _VGR_CORE_REFLECTION_HPP

#include <common/global.hpp>
#include <osgViewer/View>
#include <core/basic/Handler.hpp>
#include <core/basic/Function.hpp>

#pragma warning (push)
#pragma warning (disable:4482)	/* nonstandard extension used: enum 'enum' used in qualified name */



class ReflectionOperation;
class ReflectionHandler;
class ReflectionSettings;
class ReflectionFunction;



class ReflectionOperation : public Operation {

private:
	osg::ref_ptr<osg::Uniform>			m_start;
	osg::ref_ptr<osg::Uniform>			m_stop;

public:
	ReflectionOperation() :
			Operation(), m_start(new osg::Uniform("start", osg::Vec2(0.0f, 0.0f))), m_stop(new osg::Uniform("stop", osg::Vec2(0.0f, 0.0f))) { }
	ReflectionOperation(osg::ref_ptr<osg::Uniform> start, osg::ref_ptr<osg::Uniform> stop) :
			Operation(), m_start(start), m_stop(stop) { }

public:
	virtual void operator ()(const Handler &handler) {
		this->m_start->set(handler.start());
		this->m_start->dirty();
		this->m_stop->set(handler.stop());
		this->m_stop->dirty();
	}
	virtual void operator ()(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa, const Handler &handler) {
		this->operator ()(handler);
	}
};



class ReflectionHandler : public RectangleHandler {

public:
	ReflectionHandler() :
			RectangleHandler(new ReflectionOperation()) {
		this->m_indicator->setBorderColor(osg::Vec4(1.0f, 1.0f, 0.0f, 1.0f));
		this->m_indicator->setFillColor(osg::Vec4(0.0f, 0.0f, 0.0f, 0.0f));
	}
	ReflectionHandler(osg::ref_ptr<osg::Uniform> start, osg::ref_ptr<osg::Uniform> stop) :
			RectangleHandler(new ReflectionOperation(start, stop)) {
		this->m_indicator->setBorderColor(osg::Vec4(1.0f, 1.0f, 0.0f, 1.0f));
		this->m_indicator->setFillColor(osg::Vec4(0.0f, 0.0f, 0.0f, 0.0f));
	}

protected:
	virtual bool handle(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa, osg::Object *object, osg::NodeVisitor *visitor) {
		if (!dynamic_cast<osgViewer::View *>(&aa)) return (false);

		return (RectangleHandler::handle(ea, aa, object, visitor));
	}
};



class ReflectionSettings : public Settings {

public:
	int									mode;
	int									scale;
	int									rotation;
};



class ReflectionFunction : public Function {

private:
	osg::ref_ptr<Handler>						m_handler;
	std::vector<StandardScene *>				m_scenes;
	std::vector<osg::ref_ptr<osg::Uniform> >	m_activated;
	osg::ref_ptr<osg::Uniform>					m_start;
	osg::ref_ptr<osg::Uniform>					m_stop;
	osg::ref_ptr<osg::Uniform>					m_mode;
	osg::ref_ptr<osg::Uniform>					m_scale;
	osg::ref_ptr<osg::Uniform>					m_rotation;

public:
	ReflectionFunction();

public:
	virtual osg::ref_ptr<Handler> handler() { return (this->m_handler); }
	virtual void reset();
	virtual void bind(std::vector<StandardScene *> scenes);
	virtual void activate(const Settings &settings);
	virtual void modify(const Settings &settings);
	virtual void update(const Settings &settings);
};



#pragma warning (pop)

#endif /* _VGR_CORE_REFLECTION_HPP */
