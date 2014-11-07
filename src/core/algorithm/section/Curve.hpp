#pragma once
#ifndef _VGR_CORE_ALGORITHM_SECTION_CURVE_HPP
#define _VGR_CORE_ALGORITHM_SECTION_CURVE_HPP

#include <osgViewer/View>
#include <common/global.hpp>
#include <common/3rdparty/eigen.hpp>
#include <core/basic/Scene.hpp>

#pragma warning (push)
#pragma warning (disable:4482)	/* nonstandard extension used: enum 'enum' used in qualified name */



class StandardScene;
class Curve;



class Curve {

public:
	enum NormalizeOption {
		Unilateral,
		Bilateral
	};

	typedef eigen::Matrix3xN			Segment;

private:
	std::vector<Segment>				m_segments;

public:
	Curve() : m_segments() { }
	Curve(const Curve &curve);

public:
	std::vector<Segment> &segments() { return (this->m_segments); }
	const std::vector<Segment> &segments() const { return (this->m_segments); }
	
public:
	Curve &operator =(const Curve &curve);

public:
	bool valid() const { return (!this->m_segments.empty()); }
	Segment &segment(int i) { return (this->m_segments[i]); }
	const Segment &segment(int i) const { return (this->m_segments[i]); }
	void segmentsResize(size_t size) { this->m_segments.resize(size); }
	size_t segmentsCount() const { return (this->m_segments.size()); }
	size_t knotsCount() const;
	eigen::Vector3 min() const;
	eigen::Vector3 max() const;
	void intersect(StandardScene *scene, osg::Plane plane, osg::Polytope polytope, osg::Quat quat, bool culling);
	Curve normalized(eigen::Vector3 scales, NormalizeOption option) const;
	Curve normalized(eigen::Vector3 min, eigen::Vector3 max, eigen::Vector3 scales, NormalizeOption option) const;
};



#pragma warning (pop)

#endif /* _VGR_CORE_ALGORITHM_SECTION_CURVE_HPP */
