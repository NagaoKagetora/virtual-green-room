#pragma once
#ifndef _VGR_CORE_ALGORITHM_SECTION_LOCALESTIMATIONPROFILESECTIONPROVIDER_HPP
#define _VGR_CORE_ALGORITHM_SECTION_LOCALESTIMATIONPROFILESECTIONPROVIDER_HPP

#include <common/global.hpp>
#include <core/algorithm/section/LocalEstimationSectionProvider.hpp>



class LocalEstimationProfileSectionProvider;



class LocalEstimationProfileSectionProvider : public LocalEstimationSectionProvider {

public:
	LocalEstimationProfileSectionProvider(int order = DEFAULT_ORDER, int radius = DEFAULT_RADIUS) : LocalEstimationSectionProvider(order, radius) { }

protected:
	virtual real_t estimate(real_t x, const eigen::Vector &coeff, int order);
};



#endif /* _VGR_CORE_ALGORITHM_SECTION_LOCALESTIMATIONPROFILESECTIONPROVIDER_HPP */
