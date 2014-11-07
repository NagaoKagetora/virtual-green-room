#include "LocalEstimationCurvatureSectionProvider.hpp"



real_t LocalEstimationCurvatureSectionProvider::estimate(real_t x, const eigen::Vector &coeff, int order) {
	real_t y1 = 0, y2 = 0;
	for (int i = 0; i < order; ++i) {
		y1 += i * coeff(i) * std::pow(x, i - 1);
		y2 -= i * (i - 1) * coeff(i) * std::pow(x, i - 2);
	}
	return (y2 / std::pow(1.0 + y1 * y1, 1.5));
}
