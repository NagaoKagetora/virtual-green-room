#include "SimplifiedPolylineSectionProvider.hpp"
#include <common/3rdparty/eigen.hpp>
#include <common/algo/util.hpp>



typedef std::pair<bool, eigen::Vector3>	Knot;

size_t									smooth(std::vector<Knot> &knots, size_t begin, size_t size, real_t epsilon);



size_t smooth(std::vector<Knot> &knots, size_t begin, size_t size, real_t epsilon) {
	eigen::Vector3 start = knots[begin].second;
	eigen::Vector3 stop = knots[begin + size - 1].second;
	eigen::Vector3 direct = (start - stop).normalized();

	real_t dmax = 0;
	size_t imax = 0;
	for (size_t i = 1; i < size - 1; ++i) {
		real_t d = (knots[begin + i].second - start).cross(direct).norm();
		if (d > dmax) { dmax = d; imax = i; }
	}

	if (epsilon < 0) epsilon = dmax * 0.05;

	if ((dmax > epsilon) && !algo::Util::zero(dmax)) {
		return (smooth(knots, begin, imax + 1, epsilon) + smooth(knots, begin + imax, size - imax, epsilon) - 1);
	}
	knots[begin].first = true;
	knots[begin + size - 1].first = true;
	return (2);
}



void SimplifiedPolylineSectionProvider::run(const Curve::Segment &source, Curve::Segment &result) {

	std::vector<Knot> knots;
	for (int i = 0; i < source.cols(); ++i) knots.push_back(Knot(false, source.col(i)));

	size_t count = smooth(knots, 0, knots.size(), -1.0);

	result.resize(3, count);
	for (size_t i = 0, c = 0; i < knots.size(); ++i) if (knots[i].first) result.col(c++) = knots[i].second;
}
