#include "LocalEstimationSectionProvider.hpp"



void LocalEstimationSectionProvider::run(const Curve::Segment &source, Curve::Segment &result) {
	
	size_t diameter = 2 * this->m_radius + 1;
	size_t size = source.cols();
	
	// Extrapolates the original curves according to the sampling radius.
	Curve::Segment points(3, size + 2 * this->m_radius);
	points.middleCols(this->m_radius, size) = source;
	for (int i = 0; i < this->m_radius; ++i) {
		points.col(this->m_radius - i - 1) =  2 * points.col(this->m_radius - i) - points.col(this->m_radius - i + 1);
		points.col(size + this->m_radius + i) =  2 * points.col(size + this->m_radius + i - 1) - points.col(size + this->m_radius - i - 2);
	}

	// Performs polynomial fitting.
	result.setZero(3, size);
	eigen::Vector xs(diameter);
	eigen::Vector ys(diameter);
	eigen::Vector weights(size);
	weights.setZero();
	for (size_t i = 0; i < size; ++i) {
		xs.setZero();
		ys.setZero();
		for (size_t j = 0; j < diameter; ++j) {
			xs(j) = points(0, i + j);
			ys(j) = points(1, i + j);
		}

		// Estimates the coefficients of the polynomial.
		eigen::Vector coeff = this->computeCoefficient(xs, ys, this->m_order);

		// Assigns values with smoothing.
		int beg = std::max<int>(-this->m_radius, -i);
		int end = std::min<int>(this->m_radius + 1, size - i);
		for (int j = beg; j < end; ++j) {
			int idx = i + j;
			real_t x = source(0, idx);
			real_t y = this->estimate(x, coeff, this->m_order);
			real_t w = std::exp(-std::abs(j * 0.5));
			result.col(idx) += eigen::Vector3(x, y, 0) * w;
			weights(idx) += w;
		}
	}

	// Normalizes values.
	for (size_t i = 0; i < size; ++i) result.col(i) /= weights(i);
}
