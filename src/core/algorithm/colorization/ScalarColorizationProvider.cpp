#include "ScalarColorizationProvider.hpp"
#include <osg/Vec4>



ScalarColorizationProvider::ScalarColorizationProvider() {
	this->m_attributes.setZero();
	this->m_peak = 0;
}

bool ScalarColorizationProvider::operator ()(Model *source, Model *target, real_t &peak) {
	if (this->compute(source, target)) {
		this->colorize(target);
		peak = this->m_peak;
		return (true);
	}
	return (false);
}

void ScalarColorizationProvider::colorize(Model *model) const {
	osg::ref_ptr<osg::Vec4Array> array = new osg::Vec4Array();
	array->reserve(this->m_attributes.size());
	for (int i = 0; i < this->m_attributes.size(); ++i) {
		if (this->m_valid[i]) {
			array->push_back(osg::Vec4(0.0f, 0.0f, 0.0f, this->m_attributes(i)));
		} else {
			array->push_back(osg::Vec4(0.0f, -1.0f, 0.0f, 0.0f));
		}
	}
	model->arrangeVertexColors(*array);
}
