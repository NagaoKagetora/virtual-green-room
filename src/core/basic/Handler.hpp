#pragma once
#ifndef _VGR_CORE_BASIC_HANDLER_HPP
#define _VGR_CORE_BASIC_HANDLER_HPP

#include <osgGA/GUIEventHandler>
#include <common/global.hpp>
#include <core/basic/Operation.hpp>
#include <core/basic/Indicator.hpp>

#pragma warning (push)
#pragma warning (disable:4482)	/* nonstandard extension used: enum 'enum' used in qualified name */



class Handler;
class EmptyHandler;
class RectangleHandler;
class SegmentHandler;
class PickupHandler;



class Handler : public osgGA::GUIEventHandler {

public:
	Handler() { }
	virtual ~Handler() { }

public:
	virtual osg::Vec2 start() const { return (osg::Vec2(0.0f, 0.0f)); }
	virtual osg::Vec2 stop() const { return (osg::Vec2(0.0f, 0.0f)); }
	virtual Operation *operation() const { return (NULL); }
	virtual void setOperation(Operation *operation) { }
	virtual Indicator *indicator() const { return (NULL); }
	virtual void setIndicator(Indicator *indicator) { }
	virtual void setViewport(int width, int height) { }
	virtual bool operate() { return (false); }
	virtual void dispose() { }
};



class EmptyHandler : public Handler {

protected:
	osg::ref_ptr<Operation>				m_operation;

public:
	EmptyHandler() : Handler(), m_operation(new Operation()) { }
	explicit EmptyHandler(Operation *operation) : Handler(), m_operation(operation) { }

private:
	EmptyHandler(const EmptyHandler &handler) { }

private:
	EmptyHandler &operator =(const EmptyHandler &handler) { return (*this); }

public:
	virtual Operation *operation() const { return (this->m_operation.get()); }
	virtual void setOperation(Operation *operation) { this->m_operation = operation; }
	virtual void setViewport(int width, int height);

protected:
	virtual bool handle(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa, osg::Object *object, osg::NodeVisitor *visitor) {
		return (false);
	}
};



class RectangleHandler : public Handler {

protected:
	enum PositionMask {
		None = -1,
		Outside = 0,
		Top = 0x1,
		Left = 0x2,
		Bottom = 0x4,
		Right = 0x8,
		Center = Top | Left | Bottom | Right
	};

protected:
	osg::ref_ptr<Operation>				m_operation;
	osg::ref_ptr<RectangleIndicator>	m_indicator;
	osg::Vec2							m_start;
	osg::Vec2							m_stop;
	osg::Vec2							m_beginPos;
	osg::Vec2							m_lastPos;
	int									m_mask;

protected:
	static const int					RESPONSIVE_BAND_WIDTH = 20;

public:
	RectangleHandler() :
			Handler(), m_operation(new Operation()), m_indicator(new RectangleIndicator()),
			m_start(0.0f, 0.0f), m_stop(0.0f, 0.0f), m_beginPos(0.0f, 0.0f), m_lastPos(0.0f, 0.0f), m_mask(PositionMask::None) { }
	explicit RectangleHandler(Operation *operation) :
			Handler(), m_operation(operation), m_indicator(new RectangleIndicator()),
			m_start(0.0f, 0.0f), m_stop(0.0f, 0.0f), m_beginPos(0.0f, 0.0f), m_lastPos(0.0f, 0.0f), m_mask(PositionMask::None) { }

private:
	RectangleHandler(const RectangleHandler &handler) { }

private:
	RectangleHandler &operator =(const RectangleHandler &handler) { return (*this); }

public:
	virtual osg::Vec2 start() const { return (this->m_start); }
	virtual osg::Vec2 stop() const { return (this->m_stop); }
	virtual Operation *operation() const { return (this->m_operation.get()); }
	virtual void setOperation(Operation *operation) { this->m_operation = operation; }
	virtual Indicator *indicator() const { return (this->m_indicator.get()); }
	virtual void setViewport(int width, int height);

protected:
	virtual bool handle(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa, osg::Object *object, osg::NodeVisitor *visitor);

private:
	int computePositionMask(osg::Vec2 pos) const;
};



class SegmentHandler : public Handler {

protected:
	enum PositionMask {
		None = -1,
		Outside = 0,
		Start = 0x1,
		Stop = 0x2,
		Midpoint = 0x4,
	};

protected:
	osg::ref_ptr<Operation>				m_operation;
	osg::ref_ptr<SegmentIndicator>		m_indicator;
	osg::Vec2							m_start;
	osg::Vec2							m_stop;
	osg::Vec2							m_beginPos;
	osg::Vec2							m_lastPos;
	int									m_mask;

protected:
	static const int					RESPONSIVE_BAND_WIDTH = 20;
	
public:
	SegmentHandler() :
			Handler(), m_operation(new Operation()), m_indicator(new SegmentIndicator()),
			m_start(0.0f, 0.0f), m_stop(0.0f, 0.0f), m_beginPos(0.0f, 0.0f), m_lastPos(0.0f, 0.0f), m_mask(PositionMask::None) { }
	explicit SegmentHandler(Operation *operation) :
			Handler(), m_operation(operation), m_indicator(new SegmentIndicator()),
			m_start(0.0f, 0.0f), m_stop(0.0f, 0.0f), m_beginPos(0.0f, 0.0f), m_lastPos(0.0f, 0.0f), m_mask(PositionMask::None) { }

private:
	SegmentHandler(const SegmentHandler &handler) { }

private:
	SegmentHandler &operator =(const SegmentHandler &hanlder) { return (*this); }

public:
	virtual osg::Vec2 start() const { return (this->m_start); }
	virtual osg::Vec2 stop() const { return (this->m_stop); }
	virtual Operation *operation() const { return (this->m_operation.get()); }
	virtual void setOperation(Operation *operation) { this->m_operation = operation; }
	virtual Indicator *indicator() const { return (this->m_indicator.get()); }
	virtual void setViewport(int width, int height);

protected:
	virtual bool handle(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa, osg::Object *object, osg::NodeVisitor *visitor);

private:
	int computePositionMask(osg::Vec2 pos) const;
};



class PickupHandler : public Handler {

protected:
	osg::ref_ptr<Operation>				m_operation;
	osg::ref_ptr<RectangleIndicator>	m_indicator;
	osg::Vec2							m_start;
	osg::Vec2							m_stop;
	osg::Vec2							m_beginPos;
	osg::Vec2							m_lastPos;

public:
	PickupHandler() :
			Handler(), m_operation(new Operation()), m_indicator(new RectangleIndicator()),
			m_start(0.0f, 0.0f), m_stop(0.0f, 0.0f), m_beginPos(0.0f, 0.0f), m_lastPos(0.0f, 0.0f) { }
	explicit PickupHandler(Operation *operation) :
			Handler(), m_operation(operation), m_indicator(new RectangleIndicator()),
			m_start(0.0f, 0.0f), m_stop(0.0f, 0.0f), m_beginPos(0.0f, 0.0f), m_lastPos(0.0f, 0.0f) { }

private:
	PickupHandler(const PickupHandler &handler) { }

private:
	PickupHandler &operator =(const PickupHandler &handler) { return (*this); }

public:
	virtual osg::Vec2 start() const { return (this->m_start); }
	virtual osg::Vec2 stop() const { return (this->m_stop); }
	virtual Operation *operation() const { return (this->m_operation.get()); }
	virtual void setOperation(Operation *operation) { this->m_operation = operation; }
	virtual Indicator *indicator() const { return (this->m_indicator.get()); }
	virtual void setViewport(int width, int height);

protected:
	virtual bool handle(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa, osg::Object *object, osg::NodeVisitor *visitor);
};



#pragma warning (pop)

#endif /* _VGR_CORE_BASIC_HANDLER_HPP */
