#include "ZComponentColorizationProvider.hpp"
#include <osg/Vec4>
#include <common/util/logger.hpp>



bool ZComponentColorizationProvider::compute(Model *source, Model *target) {
	if (!source || !source->mesh() || !target || !target->mesh()) {
		_VGR_SYSTEM_ERR("Source or target model are unavailable. Colorization aborted.");
		return (false);
	}
	if (!(source->mesh()->vn == target->mesh()->vn)) {
		_VGR_SYSTEM_ERR("Source and target model are incompatible. Colorization aborted.");
		return (false);
	}

	size_t size = target->mesh()->vn;
	this->m_attributes.setZero(size);
	this->m_valid.assign(size, true);
	this->m_peak = 0;

	for (size_t i = 0; i < size; ++i) {
		real_t dist = (target->mesh()->vert[i].cP() - source->mesh()->vert[i].cP()).Z();
		if (std::abs(dist) > this->m_peak) this->m_peak = std::abs(dist);
		this->m_attributes(i) = dist;
	}
	return (true);
}
