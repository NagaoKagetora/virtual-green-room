#pragma once
#ifndef _VGR_CORE_ALGORITHM_SECTION_LOCALESTIMATIONSECTIONPROVIDER_HPP
#define _VGR_CORE_ALGORITHM_SECTION_LOCALESTIMATIONSECTIONPROVIDER_HPP

#include <common/global.hpp>
#include <core/algorithm/section/PolynomialEstimationSectionProvider.hpp>



class LocalEstimationSectionProvider;



class LocalEstimationSectionProvider : public PolynomialEstimationSectionProvider {

protected:
	static const int					DEFAULT_ORDER = 3;
	static const int					DEFAULT_RADIUS = 8;

protected:
	int									m_order;
	int									m_radius;

public:
	LocalEstimationSectionProvider(int order = DEFAULT_ORDER, int radius = DEFAULT_RADIUS) : m_order(order), m_radius(radius) { }

protected:
	virtual void run(const Curve::Segment &source, Curve::Segment &result);
	virtual real_t estimate(real_t x, const eigen::Vector &coeff, int order) { return (0.0); }
};



#endif /* _VGR_CORE_ALGORITHM_SECTION_LOCALESTIMATIONSECTIONPROVIDER_HPP */
