#pragma once
#ifndef _VGR_CORE_ALGORITHM_SECTION_SEGMENTEDSECTIONPROVIDER_HPP
#define _VGR_CORE_ALGORITHM_SECTION_SEGMENTEDSECTIONPROVIDER_HPP

#include <common/global.hpp>
#include <core/algorithm/section/SectionProvider.hpp>



class SegmentedSectionProvider;



class SegmentedSectionProvider : public SectionProvider {

protected:
	SegmentedSectionProvider			*m_provider;

public:
	SegmentedSectionProvider() : m_provider(NULL) { }
	SegmentedSectionProvider(SegmentedSectionProvider *provider) : m_provider(provider) { }

private:
	Curve &run(const Curve &source, Curve &result);

public:
	virtual Curve &operator ()(const Curve &source, Curve &result);

protected:
	virtual void run(const Curve::Segment &source, Curve::Segment &result) { }
};



#endif /* _VGR_CORE_ALGORITHM_SECTION_SEGMENTEDSECTIONPROVIDER_HPP */
