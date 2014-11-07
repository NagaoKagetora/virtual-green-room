#include "GlobalEstimationSectionProvider.hpp"



void GlobalEstimationSectionProvider::run(const Curve::Segment &source, Curve::Segment &result) {
	
	size_t size = source.cols();
	int order = std::min<int>(this->m_order, size - 1);

	result.resize(3, size);

	eigen::Vector coeff = this->computeCoefficient(source.row(0), source.row(1) * 100.0, order) * 0.01;

	for (int i = 0; i < size; ++i) {
		real_t x = source(0, i);
		real_t y = this->estimate(x, coeff, order);
		result.col(i) = eigen::Vector3(x, y, 0);
	}
}
