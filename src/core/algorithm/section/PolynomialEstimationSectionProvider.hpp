#pragma once
#ifndef _VGR_CORE_ALGORITHM_SECTION_POLYNOMIALESTIMATIONSECTIONPROVIDER_HPP
#define _VGR_CORE_ALGORITHM_SECTION_POLYNOMIALESTIMATIONSECTIONPROVIDER_HPP

#include <common/global.hpp>
#include <core/algorithm/section/SegmentedSectionProvider.hpp>



class PolynomialEstimationSectionProvider;



class PolynomialEstimationSectionProvider : public SegmentedSectionProvider {

protected:
	eigen::Vector computeCoefficient(const eigen::Vector &xs, const eigen::Vector &ys, int order);
};



#endif /* _VGR_CORE_ALGORITHM_SECTION_POLYNOMIALESTIMATIONSECTIONPROVIDER_HPP */
