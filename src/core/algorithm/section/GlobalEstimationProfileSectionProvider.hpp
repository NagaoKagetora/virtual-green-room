#pragma once
#ifndef _VGR_CORE_ALGORITHM_SECTION_GLOBALESTIMATIONPROFILESECTIONPROVIDER_HPP
#define _VGR_CORE_ALGORITHM_SECTION_GLOBALESTIMATIONPROFILESECTIONPROVIDER_HPP

#include <common/global.hpp>
#include <core/algorithm/section/GlobalEstimationSectionProvider.hpp>



class GlobalEstimationProfileSectionProvider;



class GlobalEstimationProfileSectionProvider : public GlobalEstimationSectionProvider {

public:
	GlobalEstimationProfileSectionProvider(int order = DEFAULT_ORDER) : GlobalEstimationSectionProvider(order) { }

protected:
	virtual real_t estimate(real_t x, const eigen::Vector &coeff, int order);
};



#endif /* _VGR_CORE_ALGORITHM_SECTION_GLOBALESTIMATIONPROFILESECTIONPROVIDER_HPP */
