#pragma once
#ifndef _VGR_CORE_ALGORITHM_SECTION_GLOBALESTIMATIONSLOPESECTIONPROVIDER_HPP
#define _VGR_CORE_ALGORITHM_SECTION_GLOBALESTIMATIONSLOPESECTIONPROVIDER_HPP

#include <common/global.hpp>
#include <core/algorithm/section/GlobalEstimationSectionProvider.hpp>



class GlobalEstimationSlopeSectionProvider;



class GlobalEstimationSlopeSectionProvider : public GlobalEstimationSectionProvider {

public:
	GlobalEstimationSlopeSectionProvider(int order = DEFAULT_ORDER) : GlobalEstimationSectionProvider(order) { }

protected:
	virtual real_t estimate(real_t x, const eigen::Vector &coeff, int order);
};



#endif /* _VGR_CORE_ALGORITHM_SECTION_GLOBALESTIMATIONSLOPESECTIONPROVIDER_HPP */
