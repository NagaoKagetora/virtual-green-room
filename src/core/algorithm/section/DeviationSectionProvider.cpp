#include "DeviationSectionProvider.hpp"
#include <common/3rdparty/eigen.hpp>



bool									evaluate(const Curve::Segment &segment, real_t x, real_t &y);

bool evaluate(const Curve::Segment &segment, real_t x, real_t &y) {
	for (int i = 0; i < segment.cols() - 1; ++i) {
		real_t a = segment(0, i) - x;
		real_t b = x - segment(0, i + 1);
		if ((a * b) >= 0) {
			y = (segment(1, i) * std::abs(b) + segment(1, i + 1) * std::abs(a)) / (std::abs(a) + std::abs(b));
			return (true);
		}
	}
	return (false);
}



Curve &DeviationSectionProvider::operator ()(const Curve &source, const Curve &target, Curve &result) {

	eigen::Vector sourceXMin(source.segmentsCount());
	eigen::Vector sourceXMax(source.segmentsCount());
	for (size_t i = 0; i < source.segmentsCount(); ++i) {
		sourceXMin(i) = source.segment(i).row(0).minCoeff();
		sourceXMax(i) = source.segment(i).row(0).maxCoeff();
	}
	eigen::Vector targetXMin(target.segmentsCount());
	eigen::Vector targetXMax(target.segmentsCount());
	for (size_t i = 0; i < target.segmentsCount(); ++i) {
		targetXMin(i) = target.segment(i).row(0).minCoeff();
		targetXMax(i) = target.segment(i).row(0).maxCoeff();
	}

	real_t xMin = std::min<real_t>(sourceXMin.minCoeff(), targetXMin.minCoeff());
	real_t xMax = std::max<real_t>(sourceXMax.maxCoeff(), targetXMax.maxCoeff());

	result.segmentsResize(1);
	result.segment(0).resize(3, this->m_steps + 1);
	result.segment(0).setZero();
	for (int i = 0; i <= this->m_steps; ++i) {
		real_t x = xMin + (xMax - xMin) * i / this->m_steps;
		bool sourceOk = false;
		bool targetOk = false;
		real_t sourceY = 0;
		real_t targetY = 0;
		for (int j = 0; j < source.segmentsCount(); ++j) {
			if ((sourceXMin(j) <= x) && (sourceXMax(j) >= x)) {
				sourceOk = evaluate(source.segment(j), x, sourceY);
				break;
			}
		}
		for (int j = 0; j < target.segmentsCount(); ++j) {
			if ((targetXMin(j) <= x) && (targetXMax(j) >= x)) {
				targetOk = evaluate(target.segment(j), x, targetY);
				break;
			}
		}
		real_t y = (sourceOk && targetOk) ? (sourceY - targetY) : 0;
		result.segment(0).col(i) = eigen::Vector3(x, y, 0);
	}

	return (result);
}
