#include "SegmentedSectionProvider.hpp"
#include <common/3rdparty/boost.hpp>



Curve &SegmentedSectionProvider::run(const Curve &source, Curve &result) {
	result.segmentsResize(source.segmentsCount());
	for (size_t i = 0; i < source.segmentsCount(); ++i) {
		this->run(source.segment(i), result.segment(i));
	}

	return (result);
}

Curve &SegmentedSectionProvider::operator ()(const Curve &source, Curve &result) {
	if (this->m_provider) {
		return (this->run(this->m_provider->operator ()(source, Curve()), result));
	}
	return (this->run(source, result));
};
