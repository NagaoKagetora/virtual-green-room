#pragma once
#ifndef _VGR_CORE_ALGORITHM_SECTION_SECTIONPROVIDER_HPP
#define _VGR_CORE_ALGORITHM_SECTION_SECTIONPROVIDER_HPP

#include <osg/Referenced>
#include <common/global.hpp>
#include <core/algorithm/section/Curve.hpp>



class SectionProvider;



class SectionProvider : public osg::Referenced {

public:
	virtual Curve &operator ()(const Curve &source, const Curve &target, Curve &result) { return (result); }
	virtual Curve &operator ()(const Curve &source, Curve &result) { return (result); }
};



#endif /* _VGR_CORE_ALGORITHM_SECTION_SECTIONPROVIDER_HPP */
