#include "Registration.hpp"
#include <osg/Point>
#include <osg/Hint>
#include <osgUtil/PolytopeIntersector>
#include <common/3rdparty/vcglib.hpp>
#include <common/algo/util.hpp>
#include <common/algo/register.hpp>
#include <common/util/logger.hpp>
#include <core/basic/Configuration.hpp>
#include <core/basic/Scene.hpp>
#include <core/basic/Model.hpp>
#include <core/algorithm/registration/ThreePointRegistrationProvider.hpp>
#include <core/algorithm/registration/LeastSquaresRegistrationProvider.hpp>
#include <core/algorithm/registration/IterativeRegistrationProvider.hpp>

using namespace vgr;

#pragma warning (push)
#pragma warning (disable:4482)	/* nonstandard extension used: enum 'enum' used in qualified name */



ThreePointRegistrationOperation::Selection::Selection(StandardScene *scene) :
		m_scene(scene), m_origin(-1), m_start(-1), m_stop(-1) {
	
	this->m_vertices = new osg::Vec3Array();
	this->m_backgroundColors = new osg::Vec4Array();
	this->m_backgroundPrimitives = new osg::DrawArrays();
	this->m_backgroundGeometry = new osg::Geometry();
	this->m_backgroundGeometry->setVertexArray(this->m_vertices);
	this->m_backgroundGeometry->setColorArray(this->m_backgroundColors);
	this->m_backgroundGeometry->setColorBinding(osg::Geometry::AttributeBinding::BIND_PER_VERTEX);
	this->m_backgroundGeometry->addPrimitiveSet(this->m_backgroundPrimitives);
	this->m_backgroundGeometry->getOrCreateStateSet()->setAttribute(new osg::Point(10.0f), osg::StateAttribute::Values::ON);
	this->m_foregroundColors = new osg::Vec4Array();
	this->m_foregroundPrimitives = new osg::DrawArrays();
	this->m_foregroundGeometry = new osg::Geometry();
	this->m_foregroundGeometry->setVertexArray(this->m_vertices);
	this->m_foregroundGeometry->setColorArray(this->m_foregroundColors);
	this->m_foregroundGeometry->setColorBinding(osg::Geometry::AttributeBinding::BIND_PER_VERTEX);
	this->m_foregroundGeometry->addPrimitiveSet(this->m_foregroundPrimitives);
	this->m_foregroundGeometry->getOrCreateStateSet()->setAttribute(new osg::Point(6.0f), osg::StateAttribute::Values::ON);
	this->m_geode = new osg::Geode();
	this->m_geode->addDrawable(this->m_backgroundGeometry);
	this->m_geode->addDrawable(this->m_foregroundGeometry);
	this->m_geode->getOrCreateStateSet()->setMode(GL_POINT_SMOOTH, osg::StateAttribute::Values::ON);
	this->m_geode->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::Values::OFF);
	this->m_geode->getOrCreateStateSet()->setMode(GL_DEPTH_TEST, osg::StateAttribute::Values::OFF);
	this->m_geode->getOrCreateStateSet()->setMode(GL_BLEND, osg::StateAttribute::Values::ON);

	this->addChild(this->m_geode);

	if (this->m_scene && this->m_scene->ready()) {
		StandardScene::Sprite *sprite = this->m_scene->sprite();
		if (sprite) sprite->addDecorator(std::string("registration.selection"), this);
		this->updateSelection();
	}
}

bool ThreePointRegistrationOperation::Selection::add(int index) {
	if (this->m_origin == index) return (false);
	if (this->m_start == index) return (false);
	if (this->m_stop == index) return (false);
	if (this->m_origin < 0) {
		this->m_origin = index;
		this->updateSelection();
		return (true);
	}
	if (this->m_start < 0) {
		this->m_start = index;
		this->updateSelection();
		return (true);
	}
	if (this->m_stop < 0) {
		this->m_stop = index;
		this->updateSelection();
		return (true);
	}
	return (false);
}

bool ThreePointRegistrationOperation::Selection::remove(int index) {
	if (this->m_origin == index) {
		this->m_origin = -1;
		this->updateSelection();
		return (true);
	}
	if (this->m_start == index) {
		this->m_start = -1;
		this->updateSelection();
		return (true);
	}
	if (this->m_stop == index) {
		this->m_stop = -1;
		this->updateSelection();
		return (true);
	}
	return (false);
}

void ThreePointRegistrationOperation::Selection::dispose() {
	if (this->m_scene && this->m_scene->ready()) {
		StandardScene::Sprite *sprite = this->m_scene->sprite();
		//if (sprite) sprite->removeSceneObject(std::string("registration"));
	}
}

void ThreePointRegistrationOperation::Selection::updateSelection() {
	if (!this->m_scene || !this->m_scene->ready()) return;

	const pcvcg::TriMesh *mesh = this->m_scene->model()->mesh();

	this->m_vertices->clear();
	this->m_backgroundColors->clear();
	this->m_foregroundColors->clear();
	if (!(this->m_origin < 0)) {
		this->m_vertices->push_back(algo::Util::cast<osg::Vec3>(mesh->vert[this->m_origin].cP()));
		this->m_backgroundColors->push_back(osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f));
		this->m_foregroundColors->push_back(osg::Vec4(1.0f, 0.0f, 0.0f, 1.0f));
	}
	if (!(this->m_start < 0)) {
		this->m_vertices->push_back(algo::Util::cast<osg::Vec3>(mesh->vert[this->m_start].cP()));
		this->m_backgroundColors->push_back(osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f));
		this->m_foregroundColors->push_back(osg::Vec4(0.0f, 1.0f, 0.0f, 1.0f));
	}
	if (!(this->m_stop < 0)) {
		this->m_vertices->push_back(algo::Util::cast<osg::Vec3>(mesh->vert[this->m_stop].cP()));
		this->m_backgroundColors->push_back(osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f));
		this->m_foregroundColors->push_back(osg::Vec4(0.0f, 0.0f, 1.0f, 1.0f));
	}
	this->m_vertices->dirty();
	this->m_backgroundColors->dirty();
	this->m_foregroundColors->dirty();
	this->m_backgroundPrimitives->set(osg::PrimitiveSet::Mode::POINTS, 0, this->m_vertices->size());
	this->m_backgroundPrimitives->dirty();
	this->m_backgroundGeometry->dirtyDisplayList();
	this->m_backgroundGeometry->dirtyBound();
	this->m_foregroundPrimitives->set(osg::PrimitiveSet::Mode::POINTS, 0, this->m_vertices->size());
	this->m_foregroundPrimitives->dirty();
	this->m_foregroundGeometry->dirtyDisplayList();
	this->m_foregroundGeometry->dirtyBound();
	this->m_geode->dirtyBound();
	this->dirtyBound();
}



void ThreePointRegistrationOperation::operator ()(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa, const Handler &handler) {
	if (!this->m_target || !this->m_target->ready()) return;
	if (ea.getEventType() != osgGA::GUIEventAdapter::EventType::RELEASE) return;

	if (this->m_selection->ready() && (ea.getModKeyMask() & osgGA::GUIEventAdapter::ModKeyMask::MODKEY_SHIFT)) return;
	if (this->m_selection->empty() && (ea.getModKeyMask() & osgGA::GUIEventAdapter::ModKeyMask::MODKEY_ALT)) return;

	osg::Vec2 min(std::min<float>(handler.start().x(), handler.stop().x()), std::min<float>(handler.start().y(), handler.stop().y()));
	osg::Vec2 max(std::max<float>(handler.start().x(), handler.stop().x()), std::max<float>(handler.start().y(), handler.stop().y()));

	osgUtil::PolytopeIntersector *picker = new osgUtil::PolytopeIntersector(
			osgUtil::Intersector::CoordinateFrame::WINDOW, min.x(), min.y(), max.x(), max.y());
	picker->setDimensionMask(osgUtil::PolytopeIntersector::DimZero);
	osgUtil::IntersectionVisitor iv(picker);
	iv.setTraversalMask(StandardScene::NodeMask::Pickable);
	this->m_target->screen().camera()->accept(iv);
	if (picker->containsIntersections()) {
		const osgUtil::PolytopeIntersector::Intersections &intersections(picker->getIntersections());
		if (ea.getModKeyMask() & osgGA::GUIEventAdapter::ModKeyMask::MODKEY_SHIFT) {
			for (osgUtil::PolytopeIntersector::Intersections::const_iterator iter = intersections.begin();
					iter != intersections.end(); ++iter) {
				if (this->m_selection->add(iter->primitiveIndex)) break;
			}
		} else if (ea.getModKeyMask() & osgGA::GUIEventAdapter::ModKeyMask::MODKEY_ALT) {
			for (osgUtil::PolytopeIntersector::Intersections::const_iterator iter = intersections.begin();
					iter != intersections.end(); ++iter) {
				this->m_selection->remove(iter->primitiveIndex);
			}
		}
	}
}

bool ThreePointRegistrationOperation::operate() {
	if (!this->m_source || !this->m_source->ready()) return (false);
	if (!this->m_target || !this->m_target->ready()) return (false);

	if (ThreePointRegistrationProvider(this->m_selection->origin(), this->m_selection->start(), this->m_selection->stop())(this->m_source->model(), this->m_target->model())) {
		this->m_selection->updateSelection();

		StandardScene::Manipulator *manipulator = StandardScene::sharedManipulator();
		const pcvcg::TriMesh *mesh = this->m_target->model()->mesh();
		eigen::Vector3 origin = algo::Util::toVector(mesh->vert[this->m_selection->origin()].cP());
		eigen::Vector3 start = algo::Util::toVector(mesh->vert[this->m_selection->start()].cP());
		eigen::Vector3 stop = algo::Util::toVector(mesh->vert[this->m_selection->stop()].cP());
		eigen::Vector3 center = (origin + start + stop) / 3.0;
		eigen::Vector3 normal = (start - origin).cross(stop - origin).normalized();
		osg::Vec3d up(0.0, 0.0, 0.0);
		real_t distance = 4.0 * std::max<real_t>((center - origin).norm(),
				std::max<real_t>((center - start).norm(), (center - stop).norm()));
		manipulator->getTransformation(osg::Vec3d(), osg::Vec3d(), up);
		manipulator->setTransformation(algo::Util::cast<osg::Vec3d>(center + normal * distance), algo::Util::cast<osg::Vec3d>(center), up);
		return (true);
	}
	return (false);
}



bool LeastSquaresRegistrationOperation::operate() {
	if (!this->m_source || !this->m_source->ready()) return (false);
	if (!this->m_target || !this->m_target->ready()) return (false);

	this->m_selection.clear();
	osgUtil::PolytopeIntersector *picker = new osgUtil::PolytopeIntersector(
			osgUtil::Intersector::CoordinateFrame::WINDOW, this->m_start.x(), this->m_start.y(), this->m_stop.x(), this->m_stop.y());
	picker->setDimensionMask(osgUtil::PolytopeIntersector::DimZero);
	osgUtil::IntersectionVisitor iv(picker);
	iv.setTraversalMask(StandardScene::NodeMask::Pickable);
	this->m_target->screen().camera()->accept(iv);
	pcvcg::TriMesh *mesh = this->m_source->model()->mesh();
	if (picker->containsIntersections()) {
		const osgUtil::PolytopeIntersector::Intersections &intersections(picker->getIntersections());
		for (osgUtil::PolytopeIntersector::Intersections::const_iterator iter = intersections.begin();
				iter != intersections.end(); ++iter) {
			this->m_selection.insert(iter->primitiveIndex);
		}
	}

	return (LeastSquaresRegistrationProvider(this->m_selection.begin(), this->m_selection.end())(this->m_source->model(), this->m_target->model()));
}



bool IterativeRegistrationOperation::operate() {
	if (!this->m_source || !this->m_source->ready()) return (false);
	if (!this->m_target || !this->m_target->ready()) return (false);

	this->m_selection.clear();
	osgUtil::PolytopeIntersector *picker = new osgUtil::PolytopeIntersector(
			osgUtil::Intersector::CoordinateFrame::WINDOW, this->m_start.x(), this->m_start.y(), this->m_stop.x(), this->m_stop.y());
	picker->setDimensionMask(osgUtil::PolytopeIntersector::DimZero);
	osgUtil::IntersectionVisitor iv(picker);
	iv.setTraversalMask(StandardScene::NodeMask::Pickable);
	this->m_target->screen().camera()->accept(iv);
	pcvcg::TriMesh *mesh = this->m_source->model()->mesh();
	if (picker->containsIntersections()) {
		const osgUtil::PolytopeIntersector::Intersections &intersections(picker->getIntersections());
		for (osgUtil::PolytopeIntersector::Intersections::const_iterator iter = intersections.begin();
				iter != intersections.end(); ++iter) {
			this->m_selection.insert(iter->primitiveIndex);
		}
	}

	return (IterativeRegistrationProvider(this->m_selection.begin(), this->m_selection.end())(this->m_source->model(), this->m_target->model()));
}



RegistrationFunction::RegistrationFunction() {
	this->reset();
}

void RegistrationFunction::reset() {
	this->m_handler = NULL;
}

void RegistrationFunction::bind(std::vector<StandardScene *> scenes) {
	this->m_scenes.resize(scenes.size());
	this->m_activated.resize(scenes.size());
	for (size_t i = 0; i < scenes.size(); ++i) {
		this->m_scenes[i] = scenes[i];
		this->m_activated[i] = new osg::Uniform("registration.activated", false);
		osg::StateSet *states = this->m_scenes[i]->content()->node()->getOrCreateStateSet();
		states->addUniform(this->m_activated[i]);
	}
}

void RegistrationFunction::activate(const Settings &settings) {
	if (typeid(settings) != typeid(RegistrationSettings)) return;
	const RegistrationSettings &instance(dynamic_cast<const RegistrationSettings &>(settings));

	for (size_t i = 0; i < this->m_scenes.size(); ++i) {
		if (!this->m_scenes[i]->ready()) continue;
		this->m_activated[i]->set(instance.activated);
		this->m_activated[i]->dirty();
	}

	this->modify(settings);
}

void RegistrationFunction::modify(const Settings &settings) {
	if (typeid(settings) != typeid(RegistrationSettings)) return;
	const RegistrationSettings &instance(dynamic_cast<const RegistrationSettings &>(settings));

	// Disposes and detaches from scenes the previous handler.
	for (size_t i = 0; i < this->m_scenes.size(); ++i) this->detach(this->m_scenes[i]);
	if (this->m_handler) this->m_handler->dispose();

	// Creates and attaches to scenes the new handler.
	if (instance.activated) {
		if (!this->m_scenes[instance.source]->ready()) return;
		if (!this->m_scenes[instance.target]->ready()) return;
		for (size_t i = 0; i < this->m_scenes.size(); ++i) {
			StandardScene::Sprite *sprite = this->m_scenes[i]->sprite();
			if (sprite) sprite->removeDecorator(std::string("registration.selection"));
		}
		switch (instance.mode) {
		case RegistrationSettings::Mode::ThreePoint:
			this->m_handler = new ThreePointRegistrationHandler(this->m_scenes[instance.source], this->m_scenes[instance.target]);
			break;
		case RegistrationSettings::Mode::LeastSquares:
			this->m_handler = new LeastSquaresRegistrationHandler(this->m_scenes[instance.source], this->m_scenes[instance.target]);
			break;
		case RegistrationSettings::Mode::Iterative:
			this->m_handler = new IterativeRegistrationHandler(this->m_scenes[instance.source], this->m_scenes[instance.target]);
			break;
		}
		this->attach(this->m_scenes[instance.target]);
	}

	this->update(settings);
}

void RegistrationFunction::execute(const Settings &settings) {
	if (typeid(settings) != typeid(RegistrationSettings)) return;
	const RegistrationSettings &instance(dynamic_cast<const RegistrationSettings &>(settings));

	_VGR_SYSTEM_MSG("Registration started.");

	if (instance.source == instance.target) {
		_VGR_SYSTEM_ERR("Source view is identical with target view. Registration aborted.");
		return;
	}
	if (!this->m_scenes[instance.source]->ready()) {
		_VGR_SYSTEM_ERR("Source model is unavailable. Registration aborted.");
		return;
	}
	if (!this->m_scenes[instance.target]->ready()) {
		_VGR_SYSTEM_ERR("Target model is unavailable. Registration aborted.");
		return;
	}
	if (this->m_scenes[instance.source]->model() == this->m_scenes[instance.target]->model()) {
		_VGR_SYSTEM_ERR("Source model is identical with target model. Registration aborted.");
		return;
	}

	if (this->m_handler->operate()) {
		_VGR_SYSTEM_MSG("Registration finished.");
	}
}



#pragma warning (pop)
