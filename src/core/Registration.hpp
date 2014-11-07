#pragma once
#ifndef _VGR_CORE_REGISTRATION_HPP
#define _VGR_CORE_REGISTRATION_HPP

#include <osgViewer/View>
#include <common/global.hpp>
#include <core/basic/Operation.hpp>
#include <core/basic/Handler.hpp>
#include <core/basic/Function.hpp>

#pragma warning (push)
#pragma warning (disable:4482)	/* nonstandard extension used: enum 'enum' used in qualified name */



class RegistrationOperation;
class ThreePointRegistrationOperation;
class ThreePointRegistrationHandler;
class LeastSquaresRegistrationOperation;
class LeastSquaresRegistrationHandler;
class IterativeRegistrationOperation;
class IterativeRegistrationHandler;
class RegistrationSettings;
class RegistrationFunction;



class RegistrationOperation : public Operation {

protected:
	StandardScene						*m_source;
	StandardScene						*m_target;

public:
	RegistrationOperation() : m_source(NULL), m_target(NULL) { }
	RegistrationOperation(StandardScene *source, StandardScene *target) : m_source(source), m_target(target) { }
};



class ThreePointRegistrationOperation : public RegistrationOperation {

private:
	class Selection : public osg::Group {

	public:
		StandardScene					*m_scene;
		int								m_origin;
		int								m_start;
		int								m_stop;
		osg::ref_ptr<osg::Vec3Array>	m_vertices;
		osg::ref_ptr<osg::Vec4Array>	m_backgroundColors;
		osg::ref_ptr<osg::DrawArrays>	m_backgroundPrimitives;
		osg::ref_ptr<osg::Geometry>		m_backgroundGeometry;
		osg::ref_ptr<osg::Vec4Array>	m_foregroundColors;
		osg::ref_ptr<osg::DrawArrays>	m_foregroundPrimitives;
		osg::ref_ptr<osg::Geometry>		m_foregroundGeometry;
		osg::ref_ptr<osg::Geode>		m_geode;

	public:
		Selection(StandardScene *scene);

	public:
		int origin() const { return (this->m_origin); }
		int start() const { return (this->m_start); }
		int stop() const { return (this->m_stop); }

	public:
		bool add(int index);
		bool remove(int index);
		bool ready() const { return (!(this->m_origin < 0) && !(this->m_start < 0) && !(this->m_stop < 0)); }
		bool empty() const { return ((this->m_origin < 0) && (this->m_start < 0) && (this->m_stop < 0)); }
		void updateSelection();

	public:
		void setViewport(int width, int height) { }
		void dispose();
	};

private:
	osg::ref_ptr<Selection>				m_selection;

public:
	ThreePointRegistrationOperation() :
			RegistrationOperation(), m_selection(new Selection(NULL)) { }
	ThreePointRegistrationOperation(StandardScene *source, StandardScene *target) :
			RegistrationOperation(source, target), m_selection(new Selection(target)) { }

public:
	virtual void operator ()(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa, const Handler &handler);

public:
	virtual void setViewport(int width, int height) {
		if (this->m_selection) this->m_selection->setViewport(width, height);
	}
	virtual void dispose() {
		if (this->m_selection) this->m_selection->dispose();
	}
	virtual bool operate();
};



class ThreePointRegistrationHandler : public PickupHandler {

public:
	ThreePointRegistrationHandler() :
			PickupHandler(new ThreePointRegistrationOperation()) { }
	ThreePointRegistrationHandler(StandardScene *source, StandardScene *target) :
			PickupHandler(new ThreePointRegistrationOperation(source, target)) { }

public:
	virtual bool operate() {
		if (this->operation()) return (this->operation()->operate());
		return (false);
	}
	virtual void dispose() { if (this->operation()) this->operation()->dispose(); }

protected:
	virtual bool handle(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa, osg::Object *object, osg::NodeVisitor *visitor) {
		if (!dynamic_cast<osgViewer::View *>(&aa)) return (false);

		if (ea.getModKeyMask() == osgGA::GUIEventAdapter::ModKeyMask::MODKEY_SHIFT) {
			this->m_indicator->setBorderColor(osg::Vec4(0.0f, 0.0f, 1.0f, 1.0f));
			this->m_indicator->setFillColor(osg::Vec4(0.0f, 0.0f, 1.0f, 0.2f));
		}
		if (ea.getModKeyMask() == osgGA::GUIEventAdapter::ModKeyMask::MODKEY_ALT) {
			this->m_indicator->setBorderColor(osg::Vec4(1.0f, 0.0f, 0.0f, 1.0f));
			this->m_indicator->setFillColor(osg::Vec4(1.0f, 0.0f, 0.0f, 0.2f));
		}

		return (PickupHandler::handle(ea, aa, object, visitor));
	}
};



class LeastSquaresRegistrationOperation : public RegistrationOperation {

private:
	typedef std::set<size_t>			Selection;

private:
	Selection							m_selection;
	osg::Vec2							m_start;
	osg::Vec2							m_stop;

public:
	LeastSquaresRegistrationOperation() :
			RegistrationOperation(), m_selection(), m_start(0.0f, 0.0f), m_stop(0.0f, 0.0f) { }
	LeastSquaresRegistrationOperation(StandardScene *source, StandardScene *target) :
			RegistrationOperation(source, target), m_selection(), m_start(0.0f, 0.0f), m_stop(0.0f, 0.0f) { }

public:
	virtual void operator ()(const Handler &handler) {
		this->m_start = handler.start();
		this->m_stop = handler.stop();
	}
	virtual void operator ()(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa, const Handler &handler) {
		this->operator ()(handler);
	}

public:
	virtual bool operate();
};



class LeastSquaresRegistrationHandler : public RectangleHandler {

public:
	LeastSquaresRegistrationHandler() :
			RectangleHandler(new LeastSquaresRegistrationOperation()) {
		this->m_indicator->setBorderColor(osg::Vec4(1.0f, 1.0f, 0.0f, 1.0f));
		this->m_indicator->setFillColor(osg::Vec4(1.0f, 1.0f, 0.0f, 0.2f));
	}
	LeastSquaresRegistrationHandler(StandardScene *source, StandardScene *target) :
			RectangleHandler(new LeastSquaresRegistrationOperation(source, target)) {
		this->m_indicator->setBorderColor(osg::Vec4(1.0f, 1.0f, 0.0f, 1.0f));
		this->m_indicator->setFillColor(osg::Vec4(1.0f, 1.0f, 0.0f, 0.2f));
	}

public:
	virtual bool operate() {
		if (this->operation()) return (this->operation()->operate());
		return (false);
	}
	virtual void dispose() { if (this->operation()) this->operation()->dispose(); }

protected:
	virtual bool handle(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa, osg::Object *object, osg::NodeVisitor *visitor) {
		if (!dynamic_cast<osgViewer::View *>(&aa)) return (false);

		return (RectangleHandler::handle(ea, aa, object, visitor));
	}
};



class IterativeRegistrationOperation : public RegistrationOperation {

private:
	typedef std::set<size_t>			Selection;

private:
	Selection							m_selection;
	osg::Vec2							m_start;
	osg::Vec2							m_stop;

public:
	IterativeRegistrationOperation() :
			RegistrationOperation(), m_selection(), m_start(0.0f, 0.0f), m_stop(0.0f, 0.0f)  { }
	IterativeRegistrationOperation(StandardScene *source, StandardScene *target) :
			RegistrationOperation(source, target), m_selection(), m_start(0.0f, 0.0f), m_stop(0.0f, 0.0f)  { }

public:
	virtual void operator ()(const Handler &handler) {
		this->m_start = handler.start();
		this->m_stop = handler.stop();
	}
	virtual void operator ()(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa, const Handler &handler) {
		this->operator ()(handler);
	}

public:
	virtual bool operate();
};



class IterativeRegistrationHandler : public RectangleHandler {

public:
	IterativeRegistrationHandler() :
			RectangleHandler(new IterativeRegistrationOperation()) {
		this->m_indicator->setBorderColor(osg::Vec4(1.0f, 1.0f, 0.0f, 1.0f));
		this->m_indicator->setFillColor(osg::Vec4(1.0f, 1.0f, 0.0f, 0.2f));
	}
	IterativeRegistrationHandler(StandardScene *source, StandardScene *target) :
			RectangleHandler(new IterativeRegistrationOperation(source, target)) {
		this->m_indicator->setBorderColor(osg::Vec4(1.0f, 1.0f, 0.0f, 1.0f));
		this->m_indicator->setFillColor(osg::Vec4(1.0f, 1.0f, 0.0f, 0.2f));
	}

public:
	virtual bool operate() {
		if (this->operation()) return (this->operation()->operate());
		return (false);
	}
	virtual void dispose() { if (this->operation()) this->operation()->dispose(); }

protected:
	virtual bool handle(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa, osg::Object *object, osg::NodeVisitor *visitor) {
		if (!dynamic_cast<osgViewer::View *>(&aa)) return (false);

		return (RectangleHandler::handle(ea, aa, object, visitor));
	}
};



class RegistrationSettings : public Settings {

public:
	enum Mode {
		ThreePoint,
		LeastSquares,
		Iterative
	};

public:
	int									mode;
	int									source;
	int									target;
};



class RegistrationFunction : public Function {

private:
	osg::ref_ptr<Handler>						m_handler;
	std::vector<StandardScene *>				m_scenes;
	std::vector<osg::ref_ptr<osg::Uniform> >	m_activated;

public:
	RegistrationFunction();

public:
	virtual osg::ref_ptr<Handler> handler() { return (this->m_handler); }
	virtual void reset();
	virtual void bind(std::vector<StandardScene *> scenes);
	virtual void activate(const Settings &settings);
	virtual void modify(const Settings &settings);
	virtual void execute(const Settings &settings);
};



#pragma warning (pop)

#endif /* _VGR_CORE_REGISTRATION_HPP */
