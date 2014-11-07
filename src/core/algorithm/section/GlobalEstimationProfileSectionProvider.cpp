#include "GlobalEstimationProfileSectionProvider.hpp"



real_t GlobalEstimationProfileSectionProvider::estimate(real_t x, const eigen::Vector &coeff, int order) {
	real_t y = 0;
	for (int i = 0; i < order; ++i) y += coeff(i) * std::pow(x, i);
	return (y);
}