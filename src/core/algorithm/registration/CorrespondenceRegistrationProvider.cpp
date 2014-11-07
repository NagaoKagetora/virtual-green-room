#include "CorrespondenceRegistrationProvider.hpp"
#include <common/util/logger.hpp>



bool CorrespondenceRegistrationProvider::operator ()(Model *source, Model *target) {
	if (this->verify(source, target)) {
		return (RigidRegistrationProvider::operator ()(source, target));
	}
	return (false);
}

bool CorrespondenceRegistrationProvider::verify(const Model *source, const Model *target) const {
	if (!source || !source->mesh() || !target || !target->mesh()) {
		_VGR_SYSTEM_ERR("Source or target model are unavailable. Registration aborted.");
		return (false);
	}
	if (!(source->mesh()->vn == target->mesh()->vn)) {
		_VGR_SYSTEM_ERR("Source and target model are incompatible. Registration aborted.");
		return (false);
	}
	return (true);
}
