#pragma once
#ifndef _VGR_CORE_ALGORITHM_SECTION_DEVIATIONSECTIONPROVIDER_HPP
#define _VGR_CORE_ALGORITHM_SECTION_DEVIATIONSECTIONPROVIDER_HPP

#include <common/global.hpp>
#include <core/algorithm/section/SectionProvider.hpp>



class DeviationSectionProvider;



class DeviationSectionProvider : public SectionProvider {

private:
	static const int					DEFAULT_STEPS = 100;

private:
	int									m_steps;

public:
	DeviationSectionProvider(int steps = DEFAULT_STEPS) : m_steps(steps) { }

public:
	virtual Curve &operator ()(const Curve &source, const Curve &target, Curve &result);
};



#endif /* _VGR_CORE_ALGORITHM_SECTION_DEVIATIONSECTIONPROVIDER_HPP */
