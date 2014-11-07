#pragma once
#ifndef _VGR_CORE_ALGORITHM_SECTION_SIMPLIFIEDPOLYLINESECTIONPROVIDER_HPP
#define _VGR_CORE_ALGORITHM_SECTION_SIMPLIFIEDPOLYLINESECTIONPROVIDER_HPP

#include <common/global.hpp>
#include <core/algorithm/section/SegmentedSectionProvider.hpp>



class SimplifiedPolylineSectionProvider;



class SimplifiedPolylineSectionProvider : public SegmentedSectionProvider {

protected:
	virtual void run(const Curve::Segment &source, Curve::Segment &result);
};



#endif /* _VGR_CORE_ALGORITHM_SECTION_SIMPLIFIEDPOLYLINESECTIONPROVIDER_HPP */
