#include "GlobalEstimationSlopeSectionProvider.hpp"



real_t GlobalEstimationSlopeSectionProvider::estimate(real_t x, const eigen::Vector &coeff, int order) {
	real_t y = 0;
	for (int i = 0; i < order; ++i) y += i * coeff(i) * std::pow(x, i - 1);
	return (y);
}
