#include "LeastSquaresRegistrationProvider.hpp"
#include <common/util/logger.hpp>
#include <common/algo/register.hpp>



bool LeastSquaresRegistrationProvider::regist(Model *source, Model *target) {
	eigen::Matrix3xN sourceSamples;
	eigen::Matrix3xN targetSamples;
	if (this->m_selection.empty()) {
		algo::Util::toMatrix(*source->mesh(), &sourceSamples);
		algo::Util::toMatrix(*target->mesh(), &targetSamples);
	} else {
		algo::Util::toMatrix(*source->mesh(), this->m_selection, &sourceSamples);
		algo::Util::toMatrix(*target->mesh(), this->m_selection, &targetSamples);
	}
	if ((sourceSamples.cols() < 6) || (targetSamples.cols() < 6)) {
		_VGR_SYSTEM_ERR("Constraints are insufficient. Registration aborted.");
		return (false);
	}
	algo::Register::transformation(targetSamples, sourceSamples, &this->m_rotation, &this->m_translation);
	return (true);
}
