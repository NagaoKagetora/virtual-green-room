#pragma once
#ifndef _VGR_CORE_SECTION_HPP
#define _VGR_CORE_SECTION_HPP

#include <osgViewer/View>
#include <common/global.hpp>
#include <core/basic/Scene.hpp>
#include <core/basic/Handler.hpp>
#include <core/basic/Function.hpp>
#include <core/algorithm/section/Curve.hpp>
#include <core/section/SectionScene.hpp>

#pragma warning (push)
#pragma warning (disable:4482)	/* nonstandard extension used: enum 'enum' used in qualified name */



class SectionOperation;
class RawProfileSectionOperation;
class LocalEstimationProfileSectionOperation;
class LocalEstimationSlopeSectionOperation;
class LocalEstimationCurvatureSectionOperation;
class LocalOptimizedProfileSectionOperation;
class LocalOptimizedSlopeSectionOperation;
class LocalOptimizedCurvatureSectionOperation;
class GlobalEstimationProfileSectionOperation;
class GlobalEstimationSlopeSectionOperation;
class GlobalEstimationCurvatureSectionOperation;
class SplineFittingProfileSectionOperation;
class SplineFittingSlopeSectionOperation;
class SplineFittingCurvatureSectionOperation;
class SimplifiedPolylineProfileSectionOperation;
class SimplifiedPolylineSlopeSectionOperation;
class SimplifiedPolylineCurvatureSectionOperation;
class SimplifiedSplineProfileSectionOperation;
class SimplifiedSplineSlopeSectionOperation;
class SimplifiedSplineCurvatureSectionOperation;
class SectionHandler;
class SectionSettings;
class SectionFunction;



class SectionOperation : public Operation {

protected:
	StandardScene						*m_source;
	StandardScene						*m_target;
	SectionScene						*m_primary;
	SectionScene						*m_secondary;
	Curve								m_originalSourceCurve;
	Curve								m_originalTargetCurve;
	Curve								m_sourceCurve;
	Curve								m_targetCurve;
	Curve								m_deviationCurve;
	bool								m_culling;
	real_t								m_acceptance;
	real_t								m_rejection;

public:
	SectionOperation() : Operation(), m_source(NULL), m_target(NULL), m_primary(NULL), m_secondary(NULL), m_culling(false), m_acceptance(0), m_rejection(0) { }
	SectionOperation(StandardScene *source, StandardScene *target, SectionScene *primary, SectionScene *secondary) :
			Operation(), m_source(source), m_target(target), m_primary(primary), m_secondary(secondary), m_culling(false), m_acceptance(0), m_rejection(0) { }

public:
	void setParameters(bool culling, real_t acceptance, real_t rejection) {
		this->m_culling = culling;
		this->m_acceptance = acceptance;
		this->m_rejection = rejection;
	}

public:
	virtual void operator ()(const Handler &handler) {
		this->extract(handler.start(), handler.stop());
	}
	virtual void operator ()(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa, const Handler &handler) {
		this->operator ()(handler);
	}

protected:
	virtual void buildPrimary() { }
	virtual void buildSecondary();

private:
	void extract(osg::Vec2 start, osg::Vec2 stop);
};



class RawProfileSectionOperation : public SectionOperation {

public:
	RawProfileSectionOperation() : SectionOperation() { }
	RawProfileSectionOperation(StandardScene *source, StandardScene *target, SectionScene *primary, SectionScene *secondary) :
			SectionOperation(source, target, primary, secondary) { }

protected:
	virtual void buildPrimary();
};



class LocalEstimationProfileSectionOperation : public SectionOperation {

public:
	LocalEstimationProfileSectionOperation() : SectionOperation() { }
	LocalEstimationProfileSectionOperation(StandardScene *source, StandardScene *target, SectionScene *primary, SectionScene *secondary) :
			SectionOperation(source, target, primary, secondary) { }

protected:
	virtual void buildPrimary();
};



class LocalEstimationSlopeSectionOperation : public SectionOperation {

public:
	LocalEstimationSlopeSectionOperation() : SectionOperation() { }
	LocalEstimationSlopeSectionOperation(StandardScene *source, StandardScene *target, SectionScene *primary, SectionScene *secondary) :
			SectionOperation(source, target, primary, secondary) { }

protected:
	virtual void buildPrimary();
};



class LocalEstimationCurvatureSectionOperation : public SectionOperation {

public:
	LocalEstimationCurvatureSectionOperation() : SectionOperation() { }
	LocalEstimationCurvatureSectionOperation(StandardScene *source, StandardScene *target, SectionScene *primary, SectionScene *secondary) :
			SectionOperation(source, target, primary, secondary) { }

protected:
	virtual void buildPrimary();
};



class LocalOptimizedProfileSectionOperation : public SectionOperation {

public:
	LocalOptimizedProfileSectionOperation() : SectionOperation() { }
	LocalOptimizedProfileSectionOperation(StandardScene *source, StandardScene *target, SectionScene *primary, SectionScene *secondary) :
			SectionOperation(source, target, primary, secondary) { }

protected:
	virtual void buildPrimary();
};



class LocalOptimizedSlopeSectionOperation : public SectionOperation {

public:
	LocalOptimizedSlopeSectionOperation() : SectionOperation() { }
	LocalOptimizedSlopeSectionOperation(StandardScene *source, StandardScene *target, SectionScene *primary, SectionScene *secondary) :
			SectionOperation(source, target, primary, secondary) { }

protected:
	virtual void buildPrimary();
};



class LocalOptimizedCurvatureSectionOperation : public SectionOperation {

public:
	LocalOptimizedCurvatureSectionOperation() : SectionOperation() { }
	LocalOptimizedCurvatureSectionOperation(StandardScene *source, StandardScene *target, SectionScene *primary, SectionScene *secondary) :
			SectionOperation(source, target, primary, secondary) { }

protected:
	virtual void buildPrimary();
};



class GlobalEstimationProfileSectionOperation : public SectionOperation {

public:
	GlobalEstimationProfileSectionOperation() : SectionOperation() { }
	GlobalEstimationProfileSectionOperation(StandardScene *source, StandardScene *target, SectionScene *primary, SectionScene *secondary) :
			SectionOperation(source, target, primary, secondary) { }

protected:
	virtual void buildPrimary();
};



class GlobalEstimationSlopeSectionOperation : public SectionOperation {

public:
	GlobalEstimationSlopeSectionOperation() : SectionOperation() { }
	GlobalEstimationSlopeSectionOperation(StandardScene *source, StandardScene *target, SectionScene *primary, SectionScene *secondary) :
			SectionOperation(source, target, primary, secondary) { }

protected:
	virtual void buildPrimary();
};



class GlobalEstimationCurvatureSectionOperation : public SectionOperation {

public:
	GlobalEstimationCurvatureSectionOperation() : SectionOperation() { }
	GlobalEstimationCurvatureSectionOperation(StandardScene *source, StandardScene *target, SectionScene *primary, SectionScene *secondary) :
			SectionOperation(source, target, primary, secondary) { }

protected:
	virtual void buildPrimary();
};



class SplineFittingProfileSectionOperation : public SectionOperation {

public:
	SplineFittingProfileSectionOperation() : SectionOperation() { }
	SplineFittingProfileSectionOperation(StandardScene *source, StandardScene *target, SectionScene *primary, SectionScene *secondary) :
			SectionOperation(source, target, primary, secondary) { }

protected:
	virtual void buildPrimary();
};



class SplineFittingSlopeSectionOperation : public SectionOperation {

public:
	SplineFittingSlopeSectionOperation() : SectionOperation() { }
	SplineFittingSlopeSectionOperation(StandardScene *source, StandardScene *target, SectionScene *primary, SectionScene *secondary) :
			SectionOperation(source, target, primary, secondary) { }

protected:
	virtual void buildPrimary();
};



class SplineFittingCurvatureSectionOperation : public SectionOperation {

public:
	SplineFittingCurvatureSectionOperation() : SectionOperation() { }
	SplineFittingCurvatureSectionOperation(StandardScene *source, StandardScene *target, SectionScene *primary, SectionScene *secondary) :
			SectionOperation(source, target, primary, secondary) { }

protected:
	virtual void buildPrimary();
};



class SimplifiedPolylineProfileSectionOperation : public SectionOperation {

public:
	SimplifiedPolylineProfileSectionOperation() : SectionOperation() { }
	SimplifiedPolylineProfileSectionOperation(StandardScene *source, StandardScene *target, SectionScene *primary, SectionScene *secondary) :
			SectionOperation(source, target, primary, secondary) { }

protected:
	virtual void buildPrimary();
};



class SimplifiedPolylineSlopeSectionOperation : public SectionOperation {

public:
	SimplifiedPolylineSlopeSectionOperation() : SectionOperation() { }
	SimplifiedPolylineSlopeSectionOperation(StandardScene *source, StandardScene *target, SectionScene *primary, SectionScene *secondary) :
			SectionOperation(source, target, primary, secondary) { }

protected:
	virtual void buildPrimary();
};



class SimplifiedPolylineCurvatureSectionOperation : public SectionOperation {

public:
	SimplifiedPolylineCurvatureSectionOperation() : SectionOperation() { }
	SimplifiedPolylineCurvatureSectionOperation(StandardScene *source, StandardScene *target, SectionScene *primary, SectionScene *secondary) :
			SectionOperation(source, target, primary, secondary) { }

protected:
	virtual void buildPrimary();
};



class SimplifiedSplineProfileSectionOperation : public SectionOperation {

public:
	SimplifiedSplineProfileSectionOperation() : SectionOperation() { }
	SimplifiedSplineProfileSectionOperation(StandardScene *source, StandardScene *target, SectionScene *primary, SectionScene *secondary) :
			SectionOperation(source, target, primary, secondary) { }

protected:
	virtual void buildPrimary();
};



class SimplifiedSplineSlopeSectionOperation : public SectionOperation {

public:
	SimplifiedSplineSlopeSectionOperation() : SectionOperation() { }
	SimplifiedSplineSlopeSectionOperation(StandardScene *source, StandardScene *target, SectionScene *primary, SectionScene *secondary) :
			SectionOperation(source, target, primary, secondary) { }

protected:
	virtual void buildPrimary();
};



class SimplifiedSplineCurvatureSectionOperation : public SectionOperation {

public:
	SimplifiedSplineCurvatureSectionOperation() : SectionOperation() { }
	SimplifiedSplineCurvatureSectionOperation(StandardScene *source, StandardScene *target, SectionScene *primary, SectionScene *secondary) :
			SectionOperation(source, target, primary, secondary) { }

protected:
	virtual void buildPrimary();
};



class SectionHandler : public SegmentHandler {

public:
	SectionHandler() :
			SegmentHandler(new SectionOperation()) {
		this->m_indicator->setBackgroundColor(osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f));
		this->m_indicator->setLineColor(osg::Vec4(0.5f, 0.5f, 0.5f, 1.0f));
		this->m_indicator->setStartPointColor(osg::Vec4(0.5f, 0.5f, 1.0f, 1.0f));
		this->m_indicator->setStopPointColor(osg::Vec4(1.0f, 0.5f, 0.5f, 1.0f));
	}
	SectionHandler(StandardScene *source, StandardScene *target, SectionScene *primary, SectionScene *secondary) :
			SegmentHandler(new SectionOperation(source, target, primary, secondary)) {
		this->m_indicator->setBackgroundColor(osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f));
		this->m_indicator->setLineColor(osg::Vec4(0.5f, 0.5f, 0.5f, 1.0f));
		this->m_indicator->setStartPointColor(osg::Vec4(0.5f, 0.5f, 1.0f, 1.0f));
		this->m_indicator->setStopPointColor(osg::Vec4(1.0f, 0.5f, 0.5f, 1.0f));
	}

public:
	void setParameters(bool culling, real_t acceptance, real_t rejection);

public:
	virtual bool operate() {
		this->m_operation->operator ()(*this);
		return (true);
	}

protected:
	virtual bool handle(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa, osg::Object *object, osg::NodeVisitor *visitor) {
		if (!dynamic_cast<osgViewer::View *>(&aa)) return (false);

		if (ea.getModKeyMask() & osgGA::GUIEventAdapter::ModKeyMask::MODKEY_ALT) {
			return (SegmentHandler::handle(ea, aa, object, visitor));
		} else {
			if (ea.getButtonMask() || (ea.getEventType() & osgGA::GUIEventAdapter::EventType::SCROLL)) {
				this->m_operation->operator ()(*this);
			}
			return (false);
		}
	}
};



class SectionSettings : public Settings {

public:
	enum Mode {
		RawProfile,
		LocalEstimatedProfile,
		LocalEstimatedSlope,
		LocalEstimatedCurvature,
		LocalOptimizedProfile,
		LocalOptimizedSlope,
		LocalOptimizedCurvature,
		GlobalEstimatedProfile,
		GlobalEstimatedSlope,
		GlobalEstimatedCurvature,
		SplineFittingProfile,
		SplineFittingSlope,
		SplineFittingCurvature,
		SimplifiedPolylineProfile,
		SimplifiedPolylineSlope,
		SimplifiedPolylineCurvature,
		SimplifiedSplineProfile,
		SimplifiedSplineSlope,
		SimplifiedSplineCurvature
	};

public:
	int									mode;
	int									source;
	int									target;
	bool								culling;
	double								acceptance;
	double								rejection;
};



class SectionFunction : public Function {

private:
	osg::ref_ptr<Handler>						m_handler;
	std::vector<StandardScene *>				m_scenes;
	std::vector<osg::ref_ptr<osg::Uniform> >	m_activated;
	SectionScene								*m_primaryScene;
	SectionScene								*m_secondaryScene;

public:
	SectionFunction();

public:
	virtual osg::ref_ptr<Handler> handler() { return (this->m_handler); }
	virtual void reset();
	virtual void bind(std::vector<StandardScene *> scenes);
	virtual std::vector<Scene *> views();
	virtual std::vector<Scene *> layout(const Settings &settings);
	virtual void activate(const Settings &settings);
	virtual void modify(const Settings &settings);
	virtual void update(const Settings &settings);
};



#pragma warning (pop)

#endif /* _VGR_CORE_SECTION_HPP */
