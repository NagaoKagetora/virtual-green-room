#include "SplineFittingSectionProvider.hpp"
#include <common/3rdparty/eigen.hpp>



void SplineFittingSectionProvider::run(const Curve::Segment &source, Curve::Segment &result) {

	eigen::Matrix func(4, 4);
	func <<  -1.0,  3.0, -3.0,  1.0,
			  3.0, -6.0,  3.0,  0.0,
			 -3.0,  0.0,  3.0,  0.0,
			  1.0,  4.0,  1.0,  0.0;
	func /= 6.0;
	func.transposeInPlace();

	size_t size = source.cols();
	Curve::Segment points(3, size + 2);
	points.middleCols(1, size) = source;
	points.col(0) = 2 * points.col(1) - points.col(2);
	points.col(size + 1) = 2 * points.col(size) - points.col(size - 1);

	eigen::Vector lengths(size - 1);
	for (int i = 0; i < size - 1; ++i) lengths(i) = (points.col(i + 1) - points.col(i + 2)).norm();
	real_t totalLengths = lengths.sum();

	std::vector<int> steps(size - 1);
	for (int i = 0; i < size - 1; ++i) steps[i] = std::max<int>(2, this->m_samplesCount * lengths(i) / totalLengths);
	int totalSteps = std::accumulate(steps.begin(), steps.end(), 0);

	result.resize(3, totalSteps);
	for (size_t i = 0, past = 0; i < (size - 1); past += steps[i], ++i) {
		eigen::Matrix time(steps[i], 4);
		for (int j = 0; j < steps[i]; ++j) {
			real_t t = real_t(j) / (steps[i] - 1);
			time(j, 0) = t * t * t;
			time(j, 1) = t * t;
			time(j, 2) = t;
			time(j, 3) = 1.0;
		}
		time.transposeInPlace();
		result.middleCols(past, steps[i]) = points.middleCols(i, 4) * func * time;
	}
}
