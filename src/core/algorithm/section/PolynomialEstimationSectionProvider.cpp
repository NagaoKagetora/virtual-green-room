#include "PolynomialEstimationSectionProvider.hpp"
#include <common/3rdparty/eigen.hpp>

#pragma warning (push)
#pragma warning (disable:4482)	/* nonstandard extension used: enum 'enum' used in qualified name */



eigen::Vector PolynomialEstimationSectionProvider::computeCoefficient(const eigen::Vector &xs, const eigen::Vector &ys, int order) {
	if (xs.size() != ys.size()) return (eigen::Vector::Zero(order));

	eigen::Vector xd = eigen::Vector::Ones(xs.size());
	eigen::Vector yd = ys;
	eigen::Matrix mat = eigen::Matrix::Zero(order, order);
	eigen::Vector vec = eigen::Vector::Zero(order);
	for (size_t i = 0; i < order; ++i) {
		real_t xsum = xd.sum();
		real_t ysum = yd.sum();
		xd = xd.cwiseProduct(xs);
		yd = yd.cwiseProduct(xs);
		for (size_t j = 0; j <= i; ++j) {
			mat(j, i - j) = xsum;
		}
		vec(i) = ysum;
	}
	for (size_t i = 1; i < order; ++i) {
		real_t xsum = xd.sum();
		xd = xd.cwiseProduct(xs);
		for (size_t j = i; j < order; ++j) {
			mat(j, order - j + i - 1) = xsum;
		}
	}

	return (mat.jacobiSvd(eigen::DecompositionOptions::ComputeFullU | eigen::DecompositionOptions::ComputeFullV).solve(vec));
}



#pragma warning (pop)
