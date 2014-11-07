#include "Curve.hpp"
#include <common/algo/util.hpp>
#include <QtCore/QString>
#include <osg/LineWidth>
#include <osg/Hint>
#include <osgUtil/PlaneIntersector>
#include <core/basic/Model.hpp>
#include <core/basic/Scene.hpp>

#pragma warning (push)
#pragma warning (disable:4482)	/* nonstandard extension used: enum 'enum' used in qualified name */



struct index_t;
struct pred_t;



struct index_t {

public:
	size_t								line;
	size_t								knot;

public:
	index_t() : line(-1), knot(-1) { }
	index_t(int line, int knot) : line(line), knot(knot) { }
};



struct pred_t {

public:
	bool operator ()(const std::pair<real_t, index_t> &a, const std::pair<real_t, index_t> &b) const {
		return (a.first < b.first);
	}
};



Curve::Curve(const Curve &curve) {
	this->m_segments.resize(curve.m_segments.size());
	for (size_t i = 0; i < this->m_segments.size(); ++i) {
		this->m_segments[i] = curve.m_segments[i];
	}
}

Curve &Curve::operator =(const Curve &curve) {
	this->m_segments.resize(curve.m_segments.size());
	for (size_t i = 0; i < this->m_segments.size(); ++i) {
		this->m_segments[i] = curve.m_segments[i];
	}
	return (*this);
}

size_t Curve::knotsCount() const {
	size_t result = 0;
	for (size_t i = 0; i < this->m_segments.size(); ++i) {
		result += this->m_segments[i].cols();
	}
	return (result);
}

eigen::Vector3 Curve::min() const {
	eigen::Vector3 result = eigen::Vector3::Constant(VGR_REAL_MAX);
	for (size_t i = 0; i < this->m_segments.size(); ++i) {
		result = result.cwiseMin(this->m_segments[i].rowwise().minCoeff());
	}
	return (result);
}

eigen::Vector3 Curve::max() const {
	eigen::Vector3 result = eigen::Vector3::Constant(-VGR_REAL_MAX);
	for (size_t i = 0; i < this->m_segments.size(); ++i) {
		result = result.cwiseMax(this->m_segments[i].rowwise().maxCoeff());
	}
	return (result);
}

void Curve::intersect(StandardScene *scene, osg::Plane plane, osg::Polytope polytope, osg::Quat quat, bool culling) {
	if (!scene) return;
	
	this->m_segments.clear();
	osgUtil::PlaneIntersector *picker = new osgUtil::PlaneIntersector(
			osgUtil::Intersector::CoordinateFrame::WINDOW, plane, polytope);
	osgUtil::IntersectionVisitor iv(picker);
	iv.setTraversalMask(StandardScene::NodeMask::Pickable);
	scene->screen().camera()->accept(iv);
	if (picker->containsIntersections()) {

		osgUtil::PlaneIntersector::Intersections &raws(picker->getIntersections());
		
		// Detects and culls occlusion.
		if (culling) {

			// Rotates polylines to xy-plane.
			std::vector<std::vector<bool> > flags(raws.size());
			for (size_t i = 0; i < raws.size(); ++i) {
				size_t size = raws[i].polyline.size();
				flags[i].resize(size, true);
				for (size_t j = 0; j < size; ++j) {
					raws[i].polyline[j] = quat * raws[i].polyline[j];
					raws[i].polyline[j].z() = 0;
				}
			}

			// Sorts knots with respect to x-coordinate.
			std::vector<std::pair<real_t, index_t> > knots;
			for (size_t i = 0; i < raws.size(); ++i) {
				for (size_t j = 0; j < raws[i].polyline.size(); ++j) {
					knots.push_back(std::pair<real_t, index_t>(raws[i].polyline[j].x(), index_t(i, j)));
				}
			}
			std::sort(knots.begin(), knots.end(), pred_t());

			// Marks occluded knots.
			for (size_t i = 0; i < raws.size(); ++i) {
				for (size_t j = 0; j < raws[i].polyline.size() - 1; ++j) {
					real_t x1 = raws[i].polyline[j].x(), x2 = raws[i].polyline[j + 1].x();
					real_t y1 = raws[i].polyline[j].y(), y2 = raws[i].polyline[j + 1].y();
					auto beg = std::lower_bound(knots.begin(), knots.end(), std::make_pair(std::min<real_t>(x1, x2), index_t()), pred_t()) + 1;
					auto end = std::upper_bound(knots.begin(), knots.end(), std::make_pair(std::max<real_t>(x1, x2), index_t()), pred_t()) - 1;
					for (auto iter = beg; iter < end; ++iter) {
						real_t x = raws[iter->second.line].polyline[iter->second.knot].x();
						real_t y = raws[iter->second.line].polyline[iter->second.knot].y();
						real_t det = ((x - x1) * (y2 - y) - (y - y1) * (x2 - x)) * (x2 - x1);
						if (det > 0) flags[iter->second.line][iter->second.knot] = false;
					}
				}
			}

			// Reconstructs polylines.
			std::vector<std::vector<osg::Vec3d> > polylines;
			for (size_t i = 0; i < raws.size(); ++i) {
				polylines.push_back(std::vector<osg::Vec3d>());
				for (size_t j = 0; j < raws[i].polyline.size(); ++j) {
					if (flags[i][j]) polylines.back().push_back(raws[i].polyline[j]);
					else {
						if (polylines.back().size() < 2) polylines.back().clear();
						else polylines.push_back(std::vector<osg::Vec3d>());
					}
				}
				if (polylines.back().size() < 2) polylines.pop_back();
			}

			// Translates to matrix storage.
			for (size_t i = 0; i < polylines.size(); ++i) {
				size_t size = polylines[i].size();
				this->m_segments.push_back(Segment(3, size));
				for (size_t j = 0; j < size; ++j) {
					this->m_segments.back().col(j) = algo::Util::toVector(polylines[i][j]);
				}
				this->m_segments.back().row(2).setZero();
			}

		} else {

			// Translates to matrix storage.
			for (size_t i = 0; i < raws.size(); ++i) {
				size_t size = raws[i].polyline.size();
				this->m_segments.push_back(Segment(3, size));
				for (size_t j = 0; j < size; ++j) {
					this->m_segments.back().col(j) = algo::Util::toVector(quat * raws[i].polyline[j]);
				}
				this->m_segments.back().row(2).setZero();
			}
		}
	}
}

Curve Curve::normalized(eigen::Vector3 scales, NormalizeOption option) const {
	return (this->normalized(this->min(), this->max(), scales, option));
}

Curve Curve::normalized(eigen::Vector3 min, eigen::Vector3 max, eigen::Vector3 scales, NormalizeOption option) const {
	Curve result(*this);
	for (size_t i = 0; i < result.segmentsCount(); ++i) {
		result.segment(i).row(0).array() -= min(0);
		result.segment(i).row(0).array() /= max(0) - min(0);
		result.segment(i).row(2).array().setZero();
	}
	if (option == NormalizeOption::Unilateral) {
		for (size_t i = 0; i < result.segmentsCount(); ++i) {
			result.segment(i).row(1).array() -= min(1);
			result.segment(i).row(1).array() /= max(1) - min(1);
		}
	}
	if (option == NormalizeOption::Bilateral) {
		for (size_t i = 0; i < result.segmentsCount(); ++i) {
			result.segment(i).row(1).array() += std::max(std::abs(max(1)), std::abs(min(1)));
			result.segment(i).row(1).array() /= std::max(std::abs(max(1)), std::abs(min(1))) * 2.0;
		}
	}
	for (size_t i = 0; i < result.segmentsCount(); ++i) {
		result.segment(i).row(0).array() *= scales(0);
		result.segment(i).row(1).array() *= scales(1);
	}
	return (result);
}



#pragma warning (pop)
