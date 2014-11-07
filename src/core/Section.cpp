#include "Section.hpp"
#include <QtCore/QString>
#include <osgUtil/PlaneIntersector>
#include <common/3rdparty/eigen.hpp>
#include <common/algo/util.hpp>
#include <core/basic/Configuration.hpp>
#include <core/basic/Model.hpp>
#include <core/algorithm/section/DeviationSectionProvider.hpp>
#include <core/algorithm/section/LocalEstimationProfileSectionProvider.hpp>
#include <core/algorithm/section/LocalEstimationSlopeSectionProvider.hpp>
#include <core/algorithm/section/LocalEstimationCurvatureSectionProvider.hpp>
#include <core/algorithm/section/GlobalEstimationProfileSectionProvider.hpp>
#include <core/algorithm/section/GlobalEstimationSlopeSectionProvider.hpp>
#include <core/algorithm/section/GlobalEstimationCurvatureSectionProvider.hpp>
#include <core/algorithm/section/SimplifiedPolylineSectionProvider.hpp>
#include <core/algorithm/section/SplineFittingSectionProvider.hpp>

#pragma warning (push)
#pragma warning (disable:4482)	/* nonstandard extension used: enum 'enum' used in qualified name */



void SectionOperation::extract(osg::Vec2 start, osg::Vec2 stop) {
	if (!this->m_source || !this->m_source->ready()) return;
	if (!this->m_target || !this->m_target->ready()) return;

	if (start == stop) return;

	// Calculates section plane.
	osg::Vec3 point = osg::Vec3((start + stop) / 2.0f, 0.0f);
	osg::Vec3 direct = osg::Vec3(stop - start, 0.0f);
	direct.normalize();
	osg::Vec3 normal = (direct ^ osg::Vec3(0.0f, 0.0f, 1.0f));
	normal.normalize();
	osg::Plane plane(normal, point);
	osg::Polytope polytope;
	polytope.add(osg::Plane(direct, osg::Vec3(start, 0.0f)));
	polytope.add(osg::Plane(-direct, osg::Vec3(stop, 0.0f)));

	osg::Vec2 orientation = stop - start;
	orientation.normalize();
	float angle = std::atan2(orientation.y(), orientation.x());
	StandardScene::Manipulator *manipulator = StandardScene::sharedManipulator();
	osg::Vec3 axisX = osg::Matrixd::inverse(osg::Matrixd(manipulator->getRotation())) * osg::Vec3(1.0f, 0.0f, 0.0f);
	osg::Vec3 axisZ = osg::Matrixd::inverse(osg::Matrixd(manipulator->getRotation())) * osg::Vec3(0.0f, 0.0f, 1.0f);
	osg::Quat quat = (manipulator->getRotation() * osg::Quat(osg::PI_2, axisX) * osg::Quat(angle, axisZ)).inverse();

	// Extracts and draws section polylines.
	this->m_originalSourceCurve.intersect(this->m_source, plane, polytope, quat, this->m_culling);
	this->m_originalTargetCurve.intersect(this->m_target, plane, polytope, quat, this->m_culling);
	if (!this->m_originalSourceCurve.valid() || !this->m_originalTargetCurve.valid()) return;

	this->buildPrimary();
	this->buildSecondary();
}

void SectionOperation::buildSecondary() {
	DeviationSectionProvider()(this->m_targetCurve, this->m_sourceCurve, this->m_deviationCurve);
	this->m_secondary->updateCaption(std::string("Deviation"));
	this->m_secondary->updateScene(this->m_deviationCurve, this->m_acceptance, this->m_rejection);
}



void RawProfileSectionOperation::buildPrimary() {
	std::string sourceName = this->m_source->model()->name().toStdString();
	std::string targetName = this->m_target->model()->name().toStdString();

	this->m_sourceCurve = this->m_originalSourceCurve;
	this->m_targetCurve = this->m_originalTargetCurve;
	this->m_primary->updateCaption(std::string("Profile - Raw"), sourceName, targetName);
	this->m_primary->updateScene(this->m_sourceCurve, this->m_targetCurve, Curve::NormalizeOption::Unilateral);
}



void LocalEstimationProfileSectionOperation::buildPrimary() {
	std::string sourceName = this->m_source->model()->name().toStdString();
	std::string targetName = this->m_target->model()->name().toStdString();

	LocalEstimationProfileSectionProvider()(this->m_originalSourceCurve, this->m_sourceCurve);
	LocalEstimationProfileSectionProvider()(this->m_originalTargetCurve, this->m_targetCurve);
	this->m_primary->updateCaption(std::string("Profile - Local Estimation"), sourceName, targetName);
	this->m_primary->updateScene(this->m_sourceCurve, this->m_targetCurve, Curve::NormalizeOption::Unilateral);
}



void LocalEstimationSlopeSectionOperation::buildPrimary() {
	std::string sourceName = this->m_source->model()->name().toStdString();
	std::string targetName = this->m_target->model()->name().toStdString();

	LocalEstimationSlopeSectionProvider()(this->m_originalSourceCurve, this->m_sourceCurve);
	LocalEstimationSlopeSectionProvider()(this->m_originalTargetCurve, this->m_targetCurve);
	this->m_primary->updateCaption(std::string("Slope - Local Estimation"), sourceName, targetName);
	this->m_primary->updateScene(this->m_sourceCurve, this->m_targetCurve, Curve::NormalizeOption::Bilateral);
}



void LocalEstimationCurvatureSectionOperation::buildPrimary() {
	std::string sourceName = this->m_source->model()->name().toStdString();
	std::string targetName = this->m_target->model()->name().toStdString();

	LocalEstimationCurvatureSectionProvider()(this->m_originalSourceCurve, this->m_sourceCurve);
	LocalEstimationCurvatureSectionProvider()(this->m_originalTargetCurve, this->m_targetCurve);
	this->m_primary->updateCaption(std::string("Curvature - Local Estimation"), sourceName, targetName);
	this->m_primary->updateScene(this->m_sourceCurve, this->m_targetCurve, Curve::NormalizeOption::Bilateral);
}



void LocalOptimizedProfileSectionOperation::buildPrimary() {
	std::string sourceName = this->m_source->model()->name().toStdString();
	std::string targetName = this->m_target->model()->name().toStdString();

	LocalEstimationProfileSectionProvider()(this->m_originalSourceCurve, this->m_sourceCurve);
	LocalEstimationProfileSectionProvider()(this->m_originalTargetCurve, this->m_targetCurve);
	this->m_primary->updateCaption(std::string("Profile - Local Optimized"), sourceName, targetName);
	this->m_primary->updateScene(this->m_sourceCurve, this->m_targetCurve, Curve::NormalizeOption::Unilateral);
}



void LocalOptimizedSlopeSectionOperation::buildPrimary() {
	std::string sourceName = this->m_source->model()->name().toStdString();
	std::string targetName = this->m_target->model()->name().toStdString();

	SplineFittingSectionProvider()(SimplifiedPolylineSectionProvider()(LocalEstimationSlopeSectionProvider()(this->m_originalSourceCurve, Curve()), Curve()), this->m_sourceCurve);
	SplineFittingSectionProvider()(SimplifiedPolylineSectionProvider()(LocalEstimationSlopeSectionProvider()(this->m_originalTargetCurve, Curve()), Curve()), this->m_targetCurve);
	this->m_primary->updateCaption(std::string("Slope - Local Optimized"), sourceName, targetName);
	this->m_primary->updateScene(this->m_sourceCurve, this->m_targetCurve, Curve::NormalizeOption::Bilateral);
}



void LocalOptimizedCurvatureSectionOperation::buildPrimary() {
	std::string sourceName = this->m_source->model()->name().toStdString();
	std::string targetName = this->m_target->model()->name().toStdString();

	SplineFittingSectionProvider()(SimplifiedPolylineSectionProvider()(LocalEstimationCurvatureSectionProvider()(this->m_originalSourceCurve, Curve()), Curve()), this->m_sourceCurve);
	SplineFittingSectionProvider()(SimplifiedPolylineSectionProvider()(LocalEstimationCurvatureSectionProvider()(this->m_originalTargetCurve, Curve()), Curve()), this->m_targetCurve);
	this->m_primary->updateCaption(std::string("Curvature - Local Optimized"), sourceName, targetName);
	this->m_primary->updateScene(this->m_sourceCurve, this->m_targetCurve, Curve::NormalizeOption::Bilateral);
}



void GlobalEstimationProfileSectionOperation::buildPrimary() {
	std::string sourceName = this->m_source->model()->name().toStdString();
	std::string targetName = this->m_target->model()->name().toStdString();

	GlobalEstimationProfileSectionProvider()(this->m_originalSourceCurve, this->m_sourceCurve);
	GlobalEstimationProfileSectionProvider()(this->m_originalTargetCurve, this->m_targetCurve);
	this->m_primary->updateCaption(std::string("Profile - Global Estimation"), sourceName, targetName);
	this->m_primary->updateScene(this->m_sourceCurve, this->m_targetCurve, Curve::NormalizeOption::Unilateral);
}



void GlobalEstimationSlopeSectionOperation::buildPrimary() {
	std::string sourceName = this->m_source->model()->name().toStdString();
	std::string targetName = this->m_target->model()->name().toStdString();

	GlobalEstimationSlopeSectionProvider()(this->m_originalSourceCurve, this->m_sourceCurve);
	GlobalEstimationSlopeSectionProvider()(this->m_originalTargetCurve, this->m_targetCurve);
	this->m_primary->updateCaption(std::string("Slope - Global Estimation"), sourceName, targetName);
	this->m_primary->updateScene(this->m_sourceCurve, this->m_targetCurve, Curve::NormalizeOption::Bilateral);
}



void GlobalEstimationCurvatureSectionOperation::buildPrimary() {
	std::string sourceName = this->m_source->model()->name().toStdString();
	std::string targetName = this->m_target->model()->name().toStdString();

	GlobalEstimationCurvatureSectionProvider()(this->m_originalSourceCurve, this->m_sourceCurve);
	GlobalEstimationCurvatureSectionProvider()(this->m_originalTargetCurve, this->m_targetCurve);
	this->m_primary->updateCaption(std::string("Curvature - Global Estimation"), sourceName, targetName);
	this->m_primary->updateScene(this->m_sourceCurve, this->m_targetCurve, Curve::NormalizeOption::Bilateral);
}



void SplineFittingProfileSectionOperation::buildPrimary() {
	std::string sourceName = this->m_source->model()->name().toStdString();
	std::string targetName = this->m_target->model()->name().toStdString();

	SplineFittingSectionProvider()(this->m_originalSourceCurve, this->m_sourceCurve);
	SplineFittingSectionProvider()(this->m_originalTargetCurve, this->m_targetCurve);
	this->m_primary->updateCaption(std::string("Profile - Spline Fitting"), sourceName, targetName);
	this->m_primary->updateScene(this->m_sourceCurve, this->m_targetCurve, Curve::NormalizeOption::Unilateral);
}



void SplineFittingSlopeSectionOperation::buildPrimary() {
	std::string sourceName = this->m_source->model()->name().toStdString();
	std::string targetName = this->m_target->model()->name().toStdString();

	LocalEstimationSlopeSectionProvider()(SplineFittingSectionProvider()(this->m_originalSourceCurve, Curve()), this->m_sourceCurve);
	LocalEstimationSlopeSectionProvider()(SplineFittingSectionProvider()(this->m_originalTargetCurve, Curve()), this->m_targetCurve);
	this->m_primary->updateCaption(std::string("Slope - Spline Fitting"), sourceName, targetName);
	this->m_primary->updateScene(this->m_sourceCurve, this->m_targetCurve, Curve::NormalizeOption::Bilateral);
}



void SplineFittingCurvatureSectionOperation::buildPrimary() {
	std::string sourceName = this->m_source->model()->name().toStdString();
	std::string targetName = this->m_target->model()->name().toStdString();

	LocalEstimationCurvatureSectionProvider()(SplineFittingSectionProvider()(this->m_originalSourceCurve, Curve()), this->m_sourceCurve);
	LocalEstimationCurvatureSectionProvider()(SplineFittingSectionProvider()(this->m_originalTargetCurve, Curve()), this->m_targetCurve);
	this->m_primary->updateCaption(std::string("Curvature - Spline Fitting"), sourceName, targetName);
	this->m_primary->updateScene(this->m_sourceCurve, this->m_targetCurve, Curve::NormalizeOption::Bilateral);
}



void SimplifiedPolylineProfileSectionOperation::buildPrimary() {
	std::string sourceName = this->m_source->model()->name().toStdString();
	std::string targetName = this->m_target->model()->name().toStdString();

	SimplifiedPolylineSectionProvider()(this->m_originalSourceCurve, this->m_sourceCurve);
	SimplifiedPolylineSectionProvider()(this->m_originalTargetCurve, this->m_targetCurve);
	this->m_primary->updateCaption(std::string("Profile - Simplified Polyline"), sourceName, targetName);
	this->m_primary->updateScene(this->m_sourceCurve, this->m_targetCurve, Curve::NormalizeOption::Unilateral);
}



void SimplifiedPolylineSlopeSectionOperation::buildPrimary() {
	std::string sourceName = this->m_source->model()->name().toStdString();
	std::string targetName = this->m_target->model()->name().toStdString();

	GlobalEstimationSlopeSectionProvider()(SimplifiedPolylineSectionProvider()(this->m_originalSourceCurve, Curve()), this->m_sourceCurve);
	GlobalEstimationSlopeSectionProvider()(SimplifiedPolylineSectionProvider()(this->m_originalTargetCurve, Curve()), this->m_targetCurve);
	this->m_primary->updateCaption(std::string("Slope - Simplified Polyline"), sourceName, targetName);
	this->m_primary->updateScene(this->m_sourceCurve, this->m_targetCurve, Curve::NormalizeOption::Bilateral);
}



void SimplifiedPolylineCurvatureSectionOperation::buildPrimary() {
	std::string sourceName = this->m_source->model()->name().toStdString();
	std::string targetName = this->m_target->model()->name().toStdString();

	GlobalEstimationCurvatureSectionProvider()(SimplifiedPolylineSectionProvider()(this->m_originalSourceCurve, Curve()), this->m_sourceCurve);
	GlobalEstimationCurvatureSectionProvider()(SimplifiedPolylineSectionProvider()(this->m_originalTargetCurve, Curve()), this->m_targetCurve);
	this->m_primary->updateCaption(std::string("Curvature - Simplified Polyline"), sourceName, targetName);
	this->m_primary->updateScene(this->m_sourceCurve, this->m_targetCurve, Curve::NormalizeOption::Bilateral);
}



void SimplifiedSplineProfileSectionOperation::buildPrimary() {
	std::string sourceName = this->m_source->model()->name().toStdString();
	std::string targetName = this->m_target->model()->name().toStdString();

	SplineFittingSectionProvider()(SimplifiedPolylineSectionProvider()(this->m_originalSourceCurve, Curve()), this->m_sourceCurve);
	SplineFittingSectionProvider()(SimplifiedPolylineSectionProvider()(this->m_originalTargetCurve, Curve()), this->m_targetCurve);
	this->m_primary->updateCaption(std::string("Profile - Simplified Spline"), sourceName, targetName);
	this->m_primary->updateScene(this->m_sourceCurve, this->m_targetCurve, Curve::NormalizeOption::Unilateral);
}



void SimplifiedSplineSlopeSectionOperation::buildPrimary() {
	std::string sourceName = this->m_source->model()->name().toStdString();
	std::string targetName = this->m_target->model()->name().toStdString();

	LocalEstimationSlopeSectionProvider()(SplineFittingSectionProvider()(SimplifiedPolylineSectionProvider()(this->m_originalSourceCurve, Curve()), Curve()), this->m_sourceCurve);
	LocalEstimationSlopeSectionProvider()(SplineFittingSectionProvider()(SimplifiedPolylineSectionProvider()(this->m_originalTargetCurve, Curve()), Curve()), this->m_targetCurve);
	this->m_primary->updateCaption(std::string("Slope - Simplified Spline"), sourceName, targetName);
	this->m_primary->updateScene(this->m_sourceCurve, this->m_targetCurve, Curve::NormalizeOption::Bilateral);
}



void SimplifiedSplineCurvatureSectionOperation::buildPrimary() {
	std::string sourceName = this->m_source->model()->name().toStdString();
	std::string targetName = this->m_target->model()->name().toStdString();

	LocalEstimationCurvatureSectionProvider()(SplineFittingSectionProvider()(SimplifiedPolylineSectionProvider()(this->m_originalSourceCurve, Curve()), Curve()), this->m_sourceCurve);
	LocalEstimationCurvatureSectionProvider()(SplineFittingSectionProvider()(SimplifiedPolylineSectionProvider()(this->m_originalTargetCurve, Curve()), Curve()), this->m_targetCurve);
	this->m_primary->updateCaption(std::string("Curvature - Simplified Spline"), sourceName, targetName);
	this->m_primary->updateScene(this->m_sourceCurve, this->m_targetCurve, Curve::NormalizeOption::Bilateral);
}



void SectionHandler::setParameters(bool culling, real_t acceptance, real_t rejection) {
	if (this->m_operation) ((SectionOperation *)this->m_operation.get())->setParameters(culling, acceptance, rejection);
}



SectionFunction::SectionFunction() {
	this->m_primaryScene = new SectionScene();
	this->m_secondaryScene = new SectionScene();
	this->reset();
}

void SectionFunction::reset() {
	this->m_handler = new SectionHandler();
}

void SectionFunction::bind(std::vector<StandardScene *> scenes) {
	this->m_scenes.resize(scenes.size());
	this->m_activated.resize(scenes.size());
	for (size_t i = 0; i < scenes.size(); ++i) {
		this->m_scenes[i] = scenes[i];
		this->m_activated[i] = new osg::Uniform("section.activated", false);
		osg::StateSet *states = this->m_scenes[i]->content()->node()->getOrCreateStateSet();
		states->addUniform(this->m_activated[i]);
	}
}

std::vector<Scene *> SectionFunction::views() {
	std::vector<Scene *> views(2);
	views[0] = this->m_primaryScene;
	views[1] = this->m_secondaryScene;

	return (views);
}

std::vector<Scene *> SectionFunction::layout(const Settings &settings) {
	if (typeid(settings) != typeid(SectionSettings)) return (Function::layout(settings));
	const SectionSettings &instance(dynamic_cast<const SectionSettings &>(settings));

	if (this->m_scenes.empty()) return (Function::layout(settings));

	std::vector<Scene *> layout;
	if (instance.activated) {
		layout.push_back(this->m_scenes[instance.source]);
		layout.push_back(this->m_scenes[instance.target]);
		layout.push_back(this->m_primaryScene);
		layout.push_back(this->m_secondaryScene);
	} else {
		for (size_t i = 0; i < this->m_scenes.size(); ++i) layout.push_back(this->m_scenes[i]);
	}

	return (layout);
}

void SectionFunction::activate(const Settings &settings) {
	if (typeid(settings) != typeid(SectionSettings)) return;
	const SectionSettings &instance(dynamic_cast<const SectionSettings &>(settings));

	for (size_t i = 0; i < this->m_scenes.size(); ++i) {
		if (!this->m_scenes[i]->ready()) continue;
		this->m_activated[i]->set(instance.activated);
		this->m_activated[i]->dirty();
	}

	this->modify(settings);
}

void SectionFunction::modify(const Settings &settings) {
	if (typeid(settings) != typeid(SectionSettings)) return;
	const SectionSettings &instance(dynamic_cast<const SectionSettings &>(settings));

	// Disposes and detaches from scenes the previous handler.
	for (size_t i = 0; i < this->m_scenes.size(); ++i) this->detach(this->m_scenes[i]);
	if (this->m_handler) this->m_handler->dispose();

	// Creates and attaches to scenes the new handler.
	if (instance.activated) {
		if (!this->m_scenes[instance.source]->ready()) return;
		if (!this->m_scenes[instance.target]->ready()) return;
		switch (instance.mode) {
		case SectionSettings::Mode::RawProfile:
			this->m_handler->setOperation(new RawProfileSectionOperation(
					this->m_scenes[instance.source], this->m_scenes[instance.target], this->m_primaryScene, this->m_secondaryScene));
			break;
		case SectionSettings::Mode::LocalEstimatedProfile:
			this->m_handler->setOperation(new LocalEstimationProfileSectionOperation(
					this->m_scenes[instance.source], this->m_scenes[instance.target], this->m_primaryScene, this->m_secondaryScene));
			break;
		case SectionSettings::Mode::LocalEstimatedSlope:
			this->m_handler->setOperation(new LocalEstimationSlopeSectionOperation(
					this->m_scenes[instance.source], this->m_scenes[instance.target], this->m_primaryScene, this->m_secondaryScene));
			break;
		case SectionSettings::Mode::LocalEstimatedCurvature:
			this->m_handler->setOperation(new LocalEstimationCurvatureSectionOperation(
					this->m_scenes[instance.source], this->m_scenes[instance.target], this->m_primaryScene, this->m_secondaryScene));
			break;
		case SectionSettings::Mode::LocalOptimizedProfile:
			this->m_handler->setOperation(new LocalOptimizedProfileSectionOperation(
					this->m_scenes[instance.source], this->m_scenes[instance.target], this->m_primaryScene, this->m_secondaryScene));
			break;
		case SectionSettings::Mode::LocalOptimizedSlope:
			this->m_handler->setOperation(new LocalOptimizedSlopeSectionOperation(
					this->m_scenes[instance.source], this->m_scenes[instance.target], this->m_primaryScene, this->m_secondaryScene));
			break;
		case SectionSettings::Mode::LocalOptimizedCurvature:
			this->m_handler->setOperation(new LocalOptimizedCurvatureSectionOperation(
					this->m_scenes[instance.source], this->m_scenes[instance.target], this->m_primaryScene, this->m_secondaryScene));
			break;
		case SectionSettings::Mode::GlobalEstimatedProfile:
			this->m_handler->setOperation(new GlobalEstimationProfileSectionOperation(
					this->m_scenes[instance.source], this->m_scenes[instance.target], this->m_primaryScene, this->m_secondaryScene));
			break;
		case SectionSettings::Mode::GlobalEstimatedSlope:
			this->m_handler->setOperation(new GlobalEstimationSlopeSectionOperation(
					this->m_scenes[instance.source], this->m_scenes[instance.target], this->m_primaryScene, this->m_secondaryScene));
			break;
		case SectionSettings::Mode::GlobalEstimatedCurvature:
			this->m_handler->setOperation(new GlobalEstimationCurvatureSectionOperation(
					this->m_scenes[instance.source], this->m_scenes[instance.target], this->m_primaryScene, this->m_secondaryScene));
			break;
		case SectionSettings::Mode::SplineFittingProfile:
			this->m_handler->setOperation(new SplineFittingProfileSectionOperation(
					this->m_scenes[instance.source], this->m_scenes[instance.target], this->m_primaryScene, this->m_secondaryScene));
			break;
		case SectionSettings::Mode::SplineFittingSlope:
			this->m_handler->setOperation(new SplineFittingSlopeSectionOperation(
					this->m_scenes[instance.source], this->m_scenes[instance.target], this->m_primaryScene, this->m_secondaryScene));
			break;
		case SectionSettings::Mode::SplineFittingCurvature:
			this->m_handler->setOperation(new SplineFittingCurvatureSectionOperation(
					this->m_scenes[instance.source], this->m_scenes[instance.target], this->m_primaryScene, this->m_secondaryScene));
			break;
		case SectionSettings::Mode::SimplifiedPolylineProfile:
			this->m_handler->setOperation(new SimplifiedPolylineProfileSectionOperation(
					this->m_scenes[instance.source], this->m_scenes[instance.target], this->m_primaryScene, this->m_secondaryScene));
			break;
		case SectionSettings::Mode::SimplifiedPolylineSlope:
			this->m_handler->setOperation(new SimplifiedPolylineSlopeSectionOperation(
					this->m_scenes[instance.source], this->m_scenes[instance.target], this->m_primaryScene, this->m_secondaryScene));
			break;
		case SectionSettings::Mode::SimplifiedPolylineCurvature:
			this->m_handler->setOperation(new SimplifiedPolylineCurvatureSectionOperation(
					this->m_scenes[instance.source], this->m_scenes[instance.target], this->m_primaryScene, this->m_secondaryScene));
			break;
		case SectionSettings::Mode::SimplifiedSplineProfile:
			this->m_handler->setOperation(new SimplifiedSplineProfileSectionOperation(
					this->m_scenes[instance.source], this->m_scenes[instance.target], this->m_primaryScene, this->m_secondaryScene));
			break;
		case SectionSettings::Mode::SimplifiedSplineSlope:
			this->m_handler->setOperation(new SimplifiedSplineSlopeSectionOperation(
					this->m_scenes[instance.source], this->m_scenes[instance.target], this->m_primaryScene, this->m_secondaryScene));
			break;
		case SectionSettings::Mode::SimplifiedSplineCurvature:
			this->m_handler->setOperation(new SimplifiedSplineCurvatureSectionOperation(
					this->m_scenes[instance.source], this->m_scenes[instance.target], this->m_primaryScene, this->m_secondaryScene));
			break;
		}
		for (size_t i = 0; i < this->m_scenes.size(); ++i) {
			if (!this->m_scenes[i]->ready()) continue;
			this->attach(this->m_scenes[i]);
		}
	}

	this->update(settings);
}

void SectionFunction::update(const Settings &settings) {
	if (typeid(settings) != typeid(SectionSettings)) return;
	const SectionSettings &instance(dynamic_cast<const SectionSettings &>(settings));

	if (instance.activated) {
		if (this->m_handler) {
			SectionHandler *handler = ((SectionHandler *)this->m_handler.get());
			handler->setParameters(instance.culling, instance.acceptance, instance.rejection);
			handler->operate();
		}
	}
}



#pragma warning (pop)
