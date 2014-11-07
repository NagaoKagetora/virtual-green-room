#pragma once
#ifndef _VGR_CORE_ALGORITHM_SECTION_LOCALESTIMATIONSLOPESECTIONPROVIDER_HPP
#define _VGR_CORE_ALGORITHM_SECTION_LOCALESTIMATIONSLOPESECTIONPROVIDER_HPP

#include <common/global.hpp>
#include <core/algorithm/section/LocalEstimationSectionProvider.hpp>



class LocalEstimationSlopeSectionProvider;



class LocalEstimationSlopeSectionProvider : public LocalEstimationSectionProvider {

public:
	LocalEstimationSlopeSectionProvider(int order = DEFAULT_ORDER, int radius = DEFAULT_RADIUS) : LocalEstimationSectionProvider(order, radius) { }

protected:
	virtual real_t estimate(real_t x, const eigen::Vector &coeff, int order);
};



#endif /* _VGR_CORE_ALGORITHM_SECTION_LOCALESTIMATIONSLOPESECTIONPROVIDER_HPP */
