#pragma once
#ifndef _VGR_CORE_ALGORITHM_SECTION_GLOBALESTIMATIONSECTIONPROVIDER_HPP
#define _VGR_CORE_ALGORITHM_SECTION_GLOBALESTIMATIONSECTIONPROVIDER_HPP

#include <common/global.hpp>
#include <core/algorithm/section/PolynomialEstimationSectionProvider.hpp>



class GlobalEstimationSectionProvider;



class GlobalEstimationSectionProvider : public PolynomialEstimationSectionProvider {

protected:
	static const int					DEFAULT_ORDER = 6;

protected:
	int									m_order;

public:
	GlobalEstimationSectionProvider(int order = DEFAULT_ORDER) : m_order(order) { }

protected:
	virtual void run(const Curve::Segment &source, Curve::Segment &result);
	virtual real_t estimate(real_t x, const eigen::Vector &coeff, int order) { return (0.0); }
};



#endif /* _VGR_CORE_ALGORITHM_SECTION_GLOBALESTIMATIONSECTIONPROVIDER_HPP */
