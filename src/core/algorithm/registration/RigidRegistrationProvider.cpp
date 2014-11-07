#include "RigidRegistrationProvider.hpp"
#include <common/algo/util.hpp>



bool RigidRegistrationProvider::operator ()(Model *source, Model *target) {
	if (this->regist(source, target)) {
		this->transform(target);
		return (true);
	}
	return (false);
}

void RigidRegistrationProvider::transform(Model *model) const {
	eigen::Matrix3xN coordinate;
	algo::Util::toMatrix(*(model->mesh()), &coordinate);
	coordinate = this->m_rotation * coordinate;
	coordinate.colwise() += this->m_translation;
	algo::Util::toMesh(coordinate, model->mesh());
	model->updateMesh();
	model->updateGeometry();
}
