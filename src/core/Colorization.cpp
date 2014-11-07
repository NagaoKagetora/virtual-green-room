#include "Colorization.hpp"
#include <common/util/logger.hpp>
#include <common/3rdparty/eigen.hpp>
#include <core/basic/Scene.hpp>
#include <core/basic/Model.hpp>
#include <core/basic/Configuration.hpp>
#include <core/algorithm/colorization/NComponentColorizationProvider.hpp>
#include <core/algorithm/colorization/XComponentColorizationProvider.hpp>
#include <core/algorithm/colorization/YComponentColorizationProvider.hpp>
#include <core/algorithm/colorization/ZComponentColorizationProvider.hpp>
#include <core/algorithm/colorization/NDeviationColorizationProvider.hpp>
#include <core/algorithm/colorization/CurvatureColorizationProvider.hpp>

#pragma warning (push)
#pragma warning (disable:4482)	/* nonstandard extension used: enum 'enum' used in qualified name */



ColorizationOperation::Colorbar::Colorbar(StandardScene *scene) : m_scene(scene) {

	this->m_barVertices = new osg::Vec3Array();
	this->m_barColors = new osg::Vec4Array();
	this->m_barPrimitives = new osg::DrawArrays();
	this->m_barGeometry = new osg::Geometry();
	this->m_barGeometry->setVertexArray(this->m_barVertices);
	this->m_barGeometry->setColorArray(this->m_barColors);
	this->m_barGeometry->setColorBinding(osg::Geometry::AttributeBinding::BIND_PER_PRIMITIVE);
	this->m_barGeometry->addPrimitiveSet(this->m_barPrimitives);
	this->m_toleranceVertices = new osg::Vec3Array();
	this->m_toleranceColors = new osg::Vec4Array();
	this->m_toleranceColors->push_back(osg::Vec4(0.0f, 1.0f, 0.0f, 1.0f));
	this->m_tolerancePrimitives = new osg::DrawArrays();
	this->m_toleranceGeometry = new osg::Geometry();
	this->m_toleranceGeometry->setVertexArray(this->m_toleranceVertices);
	this->m_toleranceGeometry->setColorArray(this->m_toleranceColors);
	this->m_toleranceGeometry->setColorBinding(osg::Geometry::AttributeBinding::BIND_OVERALL);
	this->m_toleranceGeometry->addPrimitiveSet(this->m_tolerancePrimitives);
	this->m_barGeode = new osg::Geode();
	this->m_barGeode->addDrawable(this->m_barGeometry);
	this->m_barGeode->addDrawable(this->m_toleranceGeometry);
	this->m_barTransform = new Transform2D();
	this->m_barTransform->addChild(this->m_barGeode);
	this->m_frameRect = new Rect();
	this->m_frameRect->setFillColor(osg::Vec4(0.0f, 0.0f, 0.0f, 0.8f));
	this->m_frameRect->setBorderColor(osg::Vec4(0.0f, 0.0f, 0.0f, 0.4f));
	this->m_frameTransform = new Transform2D();
	this->m_frameTransform->addChild(this->m_frameRect);
	this->m_upperBoundText = new Text();
	this->m_upperBoundText->setFont(Configuration::instance().spriteConfiguration().smallFontName().toStdString());
	this->m_upperBoundText->setFontSize(Configuration::instance().spriteConfiguration().smallFontSize());
	this->m_upperBoundText->setColor(Converter::toVec4(Configuration::instance().spriteConfiguration().smallFontColor()));
	this->m_upperBoundText->setAlignment(osgText::Text::AlignmentType::RIGHT_BOTTOM_BASE_LINE);
	this->m_lowerBoundText = new Text();
	this->m_lowerBoundText->setFont(Configuration::instance().spriteConfiguration().smallFontName().toStdString());
	this->m_lowerBoundText->setFontSize(Configuration::instance().spriteConfiguration().smallFontSize());
	this->m_lowerBoundText->setColor(Converter::toVec4(Configuration::instance().spriteConfiguration().smallFontColor()));
	this->m_lowerBoundText->setAlignment(osgText::Text::AlignmentType::RIGHT_BOTTOM_BASE_LINE);

	this->addChild(this->m_frameTransform);
	this->addChild(this->m_barTransform);
	this->addChild(this->m_upperBoundText);
	this->addChild(this->m_lowerBoundText);

	if (this->m_scene && this->m_scene->ready()) {
		StandardScene::Sprite *sprite = this->m_scene->sprite();
		if (sprite) sprite->addDecorator(std::string("colorization.colorbar"), this);
		this->updateBar(10);
		this->updateBound(0);
		this->updateTolerance(0);
	}
}

void ColorizationOperation::Colorbar::updateBar(int slices) {
	this->m_barVertices->clear();
	for (int i = 0; i < slices * 2; ++i) {
		this->m_barVertices->push_back(osg::Vec3(0.0f, 0.5f * i / slices      , 0.0f));
		this->m_barVertices->push_back(osg::Vec3(1.0f, 0.5f * i / slices      , 0.0f));
		this->m_barVertices->push_back(osg::Vec3(1.0f, 0.5f * (i + 1) / slices, 0.0f));
		this->m_barVertices->push_back(osg::Vec3(0.0f, 0.5f * (i + 1) / slices, 0.0f));
	}
	this->m_barVertices->dirty();
	this->m_barColors->clear();
	for (int i = 0; i < slices; ++i) {
		this->m_barColors->push_back(osg::Vec4(0.0f, (float)(i + 1) / slices, 1.0f, 1.0f));
	}
	for (size_t i = 0; i < slices; ++i) {
		this->m_barColors->push_back(osg::Vec4(1.0f, 1.0f - (float)i / slices, 0.0f, 1.0f));
	}
	this->m_barColors->dirty();
	this->m_barPrimitives->set(osg::PrimitiveSet::Mode::QUADS, 0, this->m_barVertices->size());
	this->m_barPrimitives->dirty();
	this->m_barGeometry->dirtyDisplayList();
	this->m_barGeometry->dirtyBound();
	this->m_barGeode->dirtyBound();
	this->m_barTransform->dirtyBound();
	this->dirtyBound();
}

void ColorizationOperation::Colorbar::updateBound(real_t bound) {
	this->m_upperBoundText->setText((boost::format("%1%") %  bound).str());
	this->m_lowerBoundText->setText((boost::format("%1%") % -bound).str());
	this->dirtyBound();
}

void ColorizationOperation::Colorbar::updateTolerance(real_t tolerance) {
	this->m_toleranceVertices->clear();
	this->m_toleranceVertices->push_back(osg::Vec3(0.1f, 0.5f - tolerance * 0.5f, 0.0f));
	this->m_toleranceVertices->push_back(osg::Vec3(0.9f, 0.5f - tolerance * 0.5f, 0.0f));
	this->m_toleranceVertices->push_back(osg::Vec3(0.9f, 0.5f + tolerance * 0.5f, 0.0f));
	this->m_toleranceVertices->push_back(osg::Vec3(0.1f, 0.5f + tolerance * 0.5f, 0.0f));
	this->m_toleranceVertices->dirty();
	this->m_tolerancePrimitives->set(osg::PrimitiveSet::Mode::QUADS, 0, 4);
	this->m_tolerancePrimitives->dirty();
	this->m_toleranceGeometry->dirtyDisplayList();
	this->m_toleranceGeometry->dirtyBound();
	this->m_barGeode->dirtyBound();
	this->m_barTransform->dirtyBound();
	this->dirtyBound();
}

void ColorizationOperation::Colorbar::setViewport(int width, int height) {
	this->m_barTransform->setPosition(width - 35.0f, 20.0f);
	this->m_barTransform->setSize(30.0f, height - 40.0f);
	this->m_frameTransform->setPosition(width - 45.0f, 0.0f);
	this->m_frameTransform->setSize(45.0f, height);
	this->m_upperBoundText->setPosition(width - 5.0f, height - 15.0f);
	this->m_lowerBoundText->setPosition(width - 5.0f, 5.0f);
	this->setProjectionMatrixAsOrtho2D(0, width, 0, height);
	this->dirtyBound();
}

void ColorizationOperation::Colorbar::dispose() {
	if (this->m_scene && this->m_scene->ready()) {
		StandardScene::Sprite *sprite = this->m_scene->sprite();
		if (sprite) sprite->removeDecorator(std::string("colorization.colorbar"));
	}
}

void ColorizationOperation::setParameters(real_t resolution, real_t range, real_t tolerance) {
	if (this->m_colorbar) this->m_colorbar->updateBar(resolution);
	if (this->m_colorbar) this->m_colorbar->updateBound(range);
	if (this->m_colorbar) this->m_colorbar->updateTolerance(tolerance / range);
	this->m_resolution->set((float)resolution);
	this->m_range->set((float)range);
	this->m_tolerance->set((float)tolerance);
}



bool NComponentColorizationOperation::operate() {
	if (!this->m_source || !this->m_source->ready()) return (false);
	if (!this->m_target || !this->m_target->ready()) return (false);

	real_t peak = 0;

	if (!NComponentColorizationProvider()(this->m_source->model(), this->m_target->model(), peak)) return (false);

	this->m_peak->set((float)peak);
	this->m_peak->dirty();

	return (true);
}



bool XComponentColorizationOperation::operate() {
	if (!this->m_source || !this->m_source->ready()) return (false);
	if (!this->m_target || !this->m_target->ready()) return (false);

	real_t peak = 0;

	if (!XComponentColorizationProvider()(this->m_source->model(), this->m_target->model(), peak)) return (false);

	this->m_peak->set((float)peak);
	this->m_peak->dirty();

	return (true);
}



bool YComponentColorizationOperation::operate() {
	if (!this->m_source || !this->m_source->ready()) return (false);
	if (!this->m_target || !this->m_target->ready()) return (false);

	real_t peak = 0;

	if (!YComponentColorizationProvider()(this->m_source->model(), this->m_target->model(), peak)) return (false);

	this->m_peak->set((float)peak);
	this->m_peak->dirty();

	return (true);
}



bool ZComponentColorizationOperation::operate() {
	if (!this->m_source || !this->m_source->ready()) return (false);
	if (!this->m_target || !this->m_target->ready()) return (false);

	real_t peak = 0;

	if (!ZComponentColorizationProvider()(this->m_source->model(), this->m_target->model(), peak)) return (false);

	this->m_peak->set((float)peak);
	this->m_peak->dirty();

	return (true);
}



bool NDeviationColorizationOperation::operate() {
	if (!this->m_source || !this->m_source->ready()) return (false);
	if (!this->m_target || !this->m_target->ready()) return (false);

	real_t peak = 0;

	if (!NDeviationColorizationProvider()(this->m_source->model(), this->m_target->model(), peak)) return (false);

	this->m_peak->set((float)peak);
	this->m_peak->dirty();

	return (true);
}



bool CurvatureColorizationOperation::operate() {
	if (!this->m_source || !this->m_source->ready()) return (false);
	if (!this->m_target || !this->m_target->ready()) return (false);

	real_t peak = 0;

	if (!CurvatureColorizationProvider()(this->m_source->model(), this->m_target->model(), peak)) return (false);

	this->m_peak->set((float)peak);
	this->m_peak->dirty();

	return (true);
}



void ColorizationHandler::setParameters(real_t resolution, real_t range, real_t tolerance) {
	if (this->m_operation) ((ColorizationOperation *)this->m_operation.get())->setParameters(resolution, range, tolerance);
}



ColorizationFunction::ColorizationFunction() {
	this->m_start = new osg::Uniform("colorization.start", osg::Vec2(0.0f, 0.0f));
	this->m_stop = new osg::Uniform("colorization.stop", osg::Vec2(0.0f, 0.0f));
	this->m_peak = new osg::Uniform("colorization.peak", 0.0f);
	this->m_resolution = new osg::Uniform("colorization.resolution", 0.0f);
	this->m_range = new osg::Uniform("colorization.range", 0.0f);
	this->m_tolerance = new osg::Uniform("colorization.tolerance", 0.0f);
	this->reset();
}

void ColorizationFunction::reset() {
	this->m_handler = new ColorizationHandler();
}

void ColorizationFunction::bind(std::vector<StandardScene *> scenes) {
	this->m_scenes.resize(scenes.size());
	this->m_activated.resize(scenes.size());
	for (size_t i = 0; i < scenes.size(); ++i) {
		this->m_scenes[i] = scenes[i];
		this->m_activated[i] = new osg::Uniform("colorization.activated", false);
		osg::StateSet *states = this->m_scenes[i]->content()->node()->getOrCreateStateSet();
		states->addUniform(this->m_activated[i]);
		states->addUniform(this->m_start);
		states->addUniform(this->m_stop);
		states->addUniform(this->m_peak);
		states->addUniform(this->m_resolution);
		states->addUniform(this->m_range);
		states->addUniform(this->m_tolerance);
	}
}

void ColorizationFunction::activate(const Settings &settings) {
	if (typeid(settings) != typeid(ColorizationSettings)) return;
	const ColorizationSettings &instance(dynamic_cast<const ColorizationSettings &>(settings));

	this->modify(settings);
}

void ColorizationFunction::modify(const Settings &settings) {
	if (typeid(settings) != typeid(ColorizationSettings)) return;
	const ColorizationSettings &instance(dynamic_cast<const ColorizationSettings &>(settings));

	for (size_t i = 0; i < this->m_scenes.size(); ++i) {
		if (!this->m_scenes[i]->ready()) continue;
		this->m_activated[i]->set(false);
		this->m_activated[i]->dirty();
	}

	// Disposes and detaches from scenes the previous handler.
	for (size_t i = 0; i < this->m_scenes.size(); ++i) this->detach(this->m_scenes[i]);
	if (this->m_handler) this->m_handler->dispose();

	// Creates and attaches to scenes the new handler.
	if (instance.activated) {
		if (!this->m_scenes[instance.source]->ready()) return;
		if (!this->m_scenes[instance.target]->ready()) return;
		switch (instance.mode) {
		case ColorizationSettings::Mode::NComponent:
			this->m_handler->setOperation(new NComponentColorizationOperation(this->m_scenes[instance.source], this->m_scenes[instance.target],
					this->m_start, this->m_stop, this->m_peak, this->m_resolution, this->m_range, this->m_tolerance));
			break;
		case ColorizationSettings::Mode::XComponent:
			this->m_handler->setOperation(new XComponentColorizationOperation(this->m_scenes[instance.source], this->m_scenes[instance.target],
					this->m_start, this->m_stop, this->m_peak, this->m_resolution, this->m_range, this->m_tolerance));
			break;
		case ColorizationSettings::Mode::YComponent:
			this->m_handler->setOperation(new YComponentColorizationOperation(this->m_scenes[instance.source], this->m_scenes[instance.target],
					this->m_start, this->m_stop, this->m_peak, this->m_resolution, this->m_range, this->m_tolerance));
			break;
		case ColorizationSettings::Mode::ZComponent:
			this->m_handler->setOperation(new ZComponentColorizationOperation(this->m_scenes[instance.source], this->m_scenes[instance.target],
					this->m_start, this->m_stop, this->m_peak, this->m_resolution, this->m_range, this->m_tolerance));
			break;
		case ColorizationSettings::Mode::NDeviation:
			this->m_handler->setOperation(new NDeviationColorizationOperation(this->m_scenes[instance.source], this->m_scenes[instance.target],
					this->m_start, this->m_stop, this->m_peak, this->m_resolution, this->m_range, this->m_tolerance));
			break;
		case ColorizationSettings::Mode::Curvature:
			this->m_handler->setOperation(new CurvatureColorizationOperation(this->m_scenes[instance.source], this->m_scenes[instance.target],
					this->m_start, this->m_stop, this->m_peak, this->m_resolution, this->m_range, this->m_tolerance));
			break;
		}
		this->m_activated[instance.target]->set(true);
		for (size_t i = 0; i < this->m_scenes.size(); ++i) {
			if (!this->m_scenes[i]->ready()) continue;
			this->attach(this->m_scenes[i]);
		}
	}

	this->update(settings);
}

void ColorizationFunction::update(const Settings &settings) {
	if (typeid(settings) != typeid(ColorizationSettings)) return;
	const ColorizationSettings &instance(dynamic_cast<const ColorizationSettings &>(settings));

	if (instance.activated) {
		if (!this->m_scenes[instance.source]->ready()) return;
		if (!this->m_scenes[instance.target]->ready()) return;
		if (this->m_handler) {
			ColorizationHandler *handler = ((ColorizationHandler *)this->m_handler.get());
			handler->setParameters(instance.resolution, instance.range, instance.tolerance);
		}
	}
}

void ColorizationFunction::execute(const Settings &settings) {
	if (typeid(settings) != typeid(ColorizationSettings)) return;
	const ColorizationSettings &instance(dynamic_cast<const ColorizationSettings &>(settings));

	_VGR_SYSTEM_MSG("Colorization started.");

	if (instance.source == instance.target) {
		_VGR_SYSTEM_ERR("Source view is identical with target view. Colorization aborted.");
		return;
	}
	if (!this->m_scenes[instance.source]->ready()) {
		_VGR_SYSTEM_ERR("Source model is unavailable. Colorization aborted.");
		return;
	}
	if (!this->m_scenes[instance.target]->ready()) {
		_VGR_SYSTEM_ERR("Target model is unavailable. Colorization aborted.");
		return;
	}
	if (this->m_scenes[instance.source]->model() == this->m_scenes[instance.target]->model()) {
		_VGR_SYSTEM_ERR("Source model is identical with target model. Colorization aborted.");
		return;
	}

	if (this->m_handler->operate()) {
		this->update(instance);
		_VGR_SYSTEM_MSG("Colorization finished.");
	}
}

const Results &ColorizationFunction::getResults() const {
	static ColorizationResults results;
	float peak;
	if (this->m_peak && this->m_peak->get(peak)) {
		results.peak = peak;
	}
	return (results);
}



#pragma warning (pop)
