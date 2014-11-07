#pragma once
#ifndef _VGR_CORE_COLORIZATION_HPP
#define _VGR_CORE_COLORIZATION_HPP

#include <osgViewer/View>
#include <common/global.hpp>
#include <core/basic/sprite/Hud.hpp>
#include <core/basic/sprite/Rect.hpp>
#include <core/basic/sprite/Text.hpp>
#include <core/basic/sprite/Transform.hpp>
#include <core/basic/Handler.hpp>
#include <core/basic/Function.hpp>

#pragma warning (push)
#pragma warning (disable:4482)	/* nonstandard extension used: enum 'enum' used in qualified name */



class ColorizationOperation;
class NComponentColorizationOperation;
class XComponentColorizationOperation;
class YComponentColorizationOperation;
class ZComponentColorizationOperation;
class NDeviationColorizationOperation;
class CurvatureColorizationOperation;
class ColorizationHandler;
class ColorizationSettings;
class ColorizationResults;
class ColorizationFunction;



class ColorizationOperation : public Operation {

protected:
	class Colorbar : public Hud {

	private:
		StandardScene					*m_scene;
		osg::ref_ptr<osg::Vec3Array>	m_barVertices;
		osg::ref_ptr<osg::Vec4Array>	m_barColors;
		osg::ref_ptr<osg::DrawArrays>	m_barPrimitives;
		osg::ref_ptr<osg::Geometry>		m_barGeometry;
		osg::ref_ptr<osg::Vec3Array>	m_toleranceVertices;
		osg::ref_ptr<osg::Vec4Array>	m_toleranceColors;
		osg::ref_ptr<osg::DrawArrays>	m_tolerancePrimitives;
		osg::ref_ptr<osg::Geometry>		m_toleranceGeometry;
		osg::ref_ptr<osg::Geode>		m_barGeode;
		osg::ref_ptr<Transform2D>		m_barTransform;
		osg::ref_ptr<Rect>				m_frameRect;
		osg::ref_ptr<Transform2D>		m_frameTransform;
		osg::ref_ptr<Text>				m_upperBoundText;
		osg::ref_ptr<Text>				m_lowerBoundText;

	public:
		Colorbar(StandardScene *scene);

	public:
		void updateBar(int slices);
		void updateBound(real_t bound);
		void updateTolerance(real_t tolerance);

	public:
		void setViewport(int width, int height);
		void dispose();
	};

protected:
	StandardScene						*m_source;
	StandardScene						*m_target;
	osg::ref_ptr<osg::Uniform>			m_start;
	osg::ref_ptr<osg::Uniform>			m_stop;
	osg::ref_ptr<osg::Uniform>			m_peak;
	osg::ref_ptr<osg::Uniform>			m_resolution;
	osg::ref_ptr<osg::Uniform>			m_range;
	osg::ref_ptr<osg::Uniform>			m_tolerance;
	osg::ref_ptr<Colorbar>				m_colorbar;

public:
	ColorizationOperation() :
			Operation(), m_source(NULL), m_target(NULL),
			m_start(new osg::Uniform("start", osg::Vec2(0.0f, 0.0f))),
			m_stop(new osg::Uniform("stop", osg::Vec2(0.0f, 0.0f))),
			m_peak(new osg::Uniform("peak", 0.0f)),
			m_resolution(new osg::Uniform("resolution", 0.0f)),
			m_range(new osg::Uniform("range", 0.0f)),
			m_tolerance(new osg::Uniform("tolerance", 0.0f)),
			m_colorbar(new Colorbar(NULL)) { }
	ColorizationOperation(StandardScene *source, StandardScene *target, osg::ref_ptr<osg::Uniform> start, osg::ref_ptr<osg::Uniform> stop, osg::ref_ptr<osg::Uniform> peak,
			osg::ref_ptr<osg::Uniform> resolution, osg::ref_ptr<osg::Uniform> range, osg::ref_ptr<osg::Uniform> tolerance) :
					Operation(), m_source(source), m_target(target), m_start(start), m_stop(stop), m_peak(peak),
					m_resolution(resolution), m_range(range), m_tolerance(tolerance), m_colorbar(new Colorbar(target)) { }

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

public:
	void setParameters(real_t resolution, real_t range, real_t tolerance);

public:
	virtual void setViewport(int width, int height) {
		if (this->m_colorbar) this->m_colorbar->setViewport(width, height);
	}
	virtual void dispose() {
		if (this->m_colorbar) this->m_colorbar->dispose();
	}
};



class NComponentColorizationOperation : public ColorizationOperation {

public:
	NComponentColorizationOperation() : ColorizationOperation() { }
	NComponentColorizationOperation(StandardScene *source, StandardScene *target, osg::ref_ptr<osg::Uniform> start, osg::ref_ptr<osg::Uniform> stop, osg::ref_ptr<osg::Uniform> peak,
			osg::ref_ptr<osg::Uniform> resolution, osg::ref_ptr<osg::Uniform> range, osg::ref_ptr<osg::Uniform> tolerance) :
					ColorizationOperation(source, target, start, stop, peak, resolution, range, tolerance) { }

protected:
	virtual bool operate();
};



class XComponentColorizationOperation : public ColorizationOperation {

public:
	XComponentColorizationOperation() : ColorizationOperation() { }
	XComponentColorizationOperation(StandardScene *source, StandardScene *target, osg::ref_ptr<osg::Uniform> start, osg::ref_ptr<osg::Uniform> stop, osg::ref_ptr<osg::Uniform> peak,
			osg::ref_ptr<osg::Uniform> resolution, osg::ref_ptr<osg::Uniform> range, osg::ref_ptr<osg::Uniform> tolerance) :
					ColorizationOperation(source, target, start, stop, peak, resolution, range, tolerance) { }

protected:
	virtual bool operate();
};



class YComponentColorizationOperation : public ColorizationOperation {

public:
	YComponentColorizationOperation() : ColorizationOperation() { }
	YComponentColorizationOperation(StandardScene *source, StandardScene *target, osg::ref_ptr<osg::Uniform> start, osg::ref_ptr<osg::Uniform> stop, osg::ref_ptr<osg::Uniform> peak,
			osg::ref_ptr<osg::Uniform> resolution, osg::ref_ptr<osg::Uniform> range, osg::ref_ptr<osg::Uniform> tolerance) :
					ColorizationOperation(source, target, start, stop, peak, resolution, range, tolerance) { }

protected:
	virtual bool operate();
};



class ZComponentColorizationOperation : public ColorizationOperation {

public:
	ZComponentColorizationOperation() : ColorizationOperation() { }
	ZComponentColorizationOperation(StandardScene *source, StandardScene *target, osg::ref_ptr<osg::Uniform> start, osg::ref_ptr<osg::Uniform> stop, osg::ref_ptr<osg::Uniform> peak,
			osg::ref_ptr<osg::Uniform> resolution, osg::ref_ptr<osg::Uniform> range, osg::ref_ptr<osg::Uniform> tolerance) :
					ColorizationOperation(source, target, start, stop, peak, resolution, range, tolerance) { }

protected:
	virtual bool operate();
};



class NDeviationColorizationOperation : public ColorizationOperation {

public:
	NDeviationColorizationOperation() : ColorizationOperation() { }
	NDeviationColorizationOperation(StandardScene *source, StandardScene *target, osg::ref_ptr<osg::Uniform> start, osg::ref_ptr<osg::Uniform> stop, osg::ref_ptr<osg::Uniform> peak,
			osg::ref_ptr<osg::Uniform> resolution, osg::ref_ptr<osg::Uniform> range, osg::ref_ptr<osg::Uniform> tolerance) :
					ColorizationOperation(source, target, start, stop, peak, resolution, range, tolerance) { }

protected:
	virtual bool operate();
};



class CurvatureColorizationOperation : public ColorizationOperation {

public:
	CurvatureColorizationOperation() : ColorizationOperation() { }
	CurvatureColorizationOperation(StandardScene *source, StandardScene *target, osg::ref_ptr<osg::Uniform> start, osg::ref_ptr<osg::Uniform> stop, osg::ref_ptr<osg::Uniform> peak,
			osg::ref_ptr<osg::Uniform> resolution, osg::ref_ptr<osg::Uniform> range, osg::ref_ptr<osg::Uniform> tolerance) :
					ColorizationOperation(source, target, start, stop, peak, resolution, range, tolerance) { }

protected:
	virtual bool operate();
};



class ColorizationHandler : public RectangleHandler {

public:
	ColorizationHandler() :
			RectangleHandler(new ColorizationOperation()) {
		this->m_indicator->setBorderColor(osg::Vec4(1.0f, 1.0f, 0.0f, 1.0f));
		this->m_indicator->setFillColor(osg::Vec4(0.0f, 0.0f, 0.0f, 0.0f));
	}
	ColorizationHandler(StandardScene *source, StandardScene *target, osg::ref_ptr<osg::Uniform> start, osg::ref_ptr<osg::Uniform> stop, osg::ref_ptr<osg::Uniform> peak,
			osg::ref_ptr<osg::Uniform> resolution, osg::ref_ptr<osg::Uniform> range, osg::ref_ptr<osg::Uniform> tolerance) :
					RectangleHandler(new ColorizationOperation(source, target, peak, start, stop, resolution, range, tolerance)) {
		this->m_indicator->setBorderColor(osg::Vec4(1.0f, 1.0f, 0.0f, 1.0f));
		this->m_indicator->setFillColor(osg::Vec4(0.0f, 0.0f, 0.0f, 0.0f));
	}

public:
	void setParameters(real_t resolution, real_t range, real_t tolerance);

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



class ColorizationSettings : public Settings {

public:
	enum Mode {
		NComponent,
		XComponent,
		YComponent,
		ZComponent,
		NDeviation,
		Curvature
	};

public:
	int									mode;
	int									source;
	int									target;
	double								resolution;
	double								range;
	double								tolerance;
};



class ColorizationResults : public Results {

public:
	double								peak;
};



class ColorizationFunction : public Function {

private:
	osg::ref_ptr<Handler>						m_handler;
	std::vector<StandardScene *>				m_scenes;
	std::vector<osg::ref_ptr<osg::Uniform> >	m_activated;
	osg::ref_ptr<osg::Uniform>					m_start;
	osg::ref_ptr<osg::Uniform>					m_stop;
	osg::ref_ptr<osg::Uniform>					m_peak;
	osg::ref_ptr<osg::Uniform>					m_resolution;
	osg::ref_ptr<osg::Uniform>					m_range;
	osg::ref_ptr<osg::Uniform>					m_tolerance;

public:
	ColorizationFunction();

public:
	virtual osg::ref_ptr<Handler> handler() { return (this->m_handler); }
	virtual void reset();
	virtual void bind(std::vector<StandardScene *> scenes);
	virtual void activate(const Settings &settings);
	virtual void modify(const Settings &settings);
	virtual void update(const Settings &settings);
	virtual void execute(const Settings &settings);
	virtual const Results &getResults() const;
};



#pragma warning (pop)

#endif /* _VGR_CORE_COLORIZATION_HPP */
