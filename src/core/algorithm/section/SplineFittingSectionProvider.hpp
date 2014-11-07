#pragma once
#ifndef _VGR_CORE_ALGORITHM_SECTION_SPLINEFITTINGSECTIONPROVIDER_HPP
#define _VGR_CORE_ALGORITHM_SECTION_SPLINEFITTINGSECTIONPROVIDER_HPP

#include <common/global.hpp>
#include <core/algorithm/section/SegmentedSectionProvider.hpp>



class SplineFittingSectionProvider;



class SplineFittingSectionProvider : public SegmentedSectionProvider {

private:
	static const int					DEFAULT_SAMPLES_COUNT = 100;

private:
	int									m_samplesCount;

public:
	SplineFittingSectionProvider(int samples = DEFAULT_SAMPLES_COUNT) : m_samplesCount(samples) { }

protected:
	virtual void run(const Curve::Segment &source, Curve::Segment &result);
};



#endif /* _VGR_CORE_ALGORITHM_SECTION_SPLINEFITTINGSECTIONPROVIDER_HPP */
