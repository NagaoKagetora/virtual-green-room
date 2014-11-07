#include "NDeviationColorizationProvider.hpp"
#include <osg/Vec4>
#include <common/util/logger.hpp>



bool NDeviationColorizationProvider::compute(Model *source, Model *target) {
	if (!source || !source->mesh() || !target || !target->mesh()) {
		_VGR_SYSTEM_ERR("Source or target model are unavailable. Colorization aborted.");
		return (false);
	}

	size_t size = target->mesh()->vn;
	this->m_attributes.setZero(size);
	this->m_valid.assign(size, true);
	this->m_peak = 0;

	// Builds tree index.
	_VGR_SYSTEM_MSG("Building tree index.");
	_VGR_LOGGER_BEG_PROG;
	_VGR_LOGGER_BSY_PROG;
	pcvcg::TreeIndex index;
	index.Set(source->mesh()->face.begin(), source->mesh()->face.end(),
			pcvcg::GetPointerFunctor(), pcvcg::GetBox3Functor(), pcvcg::GetBarycenter3Functor(), 10);
	_VGR_LOGGER_END_PROG;

	// Calculates normal deviations.
	_VGR_SYSTEM_MSG("Calculating normal deviations.");
	pcvcg::RayTriangleIntersectionFunctor functor;
	pcvcg::EmptyClass maker;
	pcvcg::TreeIndex::ScalarType distFront = 0, distBack = 0;
	bool testFront = false, testBack = false;
	_VGR_LOGGER_BEG_PROG;
	for (size_t i = 0; i < size; ++i) {
		_VGR_LOGGER_UPD_PROG(i, size);
		testFront = index.DoRay(functor, maker, pcvcg::Ray3(target->mesh()->vert[i].cP(), target->mesh()->vert[i].cN()), VGR_REAL_MAX, distFront);
		testBack = index.DoRay(functor, maker, pcvcg::Ray3(target->mesh()->vert[i].cP(), -target->mesh()->vert[i].cN()), VGR_REAL_MAX, distBack);
		if (testFront && testBack) {
			if (distFront > distBack) {
				this->m_attributes(i) = distBack;
			} else {
				this->m_attributes(i) = -distFront;
			}
		} else if (testFront) {
			this->m_attributes(i) = -distFront;
		} else if (testBack) {
			this->m_attributes(i) = distBack;
		} else {
			this->m_valid[i] = false;
		}
		if (std::abs(this->m_attributes(i)) > this->m_peak) this->m_peak = std::abs(this->m_attributes(i));
	}
	_VGR_LOGGER_END_PROG;

	return (true);
}
