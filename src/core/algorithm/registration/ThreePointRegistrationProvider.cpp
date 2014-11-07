#include "ThreePointRegistrationProvider.hpp"
#include <common/util/logger.hpp>
#include <common/algo/register.hpp>



bool ThreePointRegistrationProvider::regist(Model *source, Model *target) {
	if ((this->m_origin < 0) || (this->m_start < 0) || (this->m_stop < 0)) {
		_VGR_SYSTEM_ERR("Constraints are insufficient. Registration aborted.");
		return (false);
	}
	eigen::Matrix3 sourceCoordinateSystem = algo::Util::coordinate(
			source->mesh()->vert[this->m_origin].cP(),
			source->mesh()->vert[this->m_start].cP(),
			source->mesh()->vert[this->m_stop].cP());
	eigen::Matrix3 targetCoordinateSystem = algo::Util::coordinate(
			target->mesh()->vert[this->m_origin].cP(),
			target->mesh()->vert[this->m_start].cP(),
			target->mesh()->vert[this->m_stop].cP());
	if (sourceCoordinateSystem.isZero() || targetCoordinateSystem.isZero()) {
		_VGR_SYSTEM_ERR("Constraints are unstable due to approximate coincidence or collinearity. Registration aborted.");
		return (false);
	}
	this->m_rotation = sourceCoordinateSystem * targetCoordinateSystem.transpose();
	this->m_translation = algo::Util::toVector(source->mesh()->vert[this->m_origin].cP())
			- this->m_rotation * algo::Util::toVector(target->mesh()->vert[this->m_origin].cP());
	return (true);
}
