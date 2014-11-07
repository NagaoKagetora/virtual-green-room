#include "IterativeRegistrationProvider.hpp"
#include <common/algo/register.hpp>
#include <common/algo/util.hpp>
#include <common/util/logger.hpp>
#include <core/basic/Execution.hpp>

#pragma warning (disable:4291)
#include <flann/flann.hpp>

using namespace vgr;



bool IterativeRegistrationProvider::regist(Model *source, Model *target) {

	static const size_t MAXIMUM_QUERIES = 10000;

	// Builds tree index.
	_VGR_SYSTEM_MSG("Building tree index.");
	_VGR_LOGGER_BEG_PROG;
	_VGR_LOGGER_BSY_PROG;
	flann::Matrix<float> dataset(new float[source->mesh()->vn * 3], source->mesh()->vn, 3);
	for (size_t i = 0; i < source->mesh()->vn; ++i) {
		dataset[i][0] = source->mesh()->vert[i].cP()[0];
		dataset[i][1] = source->mesh()->vert[i].cP()[1];
		dataset[i][2] = source->mesh()->vert[i].cP()[2];
	}

	flann::Index<flann::L2<float> > index(dataset, flann::KDTreeIndexParams());
	index.buildIndex();
	_VGR_LOGGER_END_PROG;

	size_t size = std::min<size_t>(MAXIMUM_QUERIES, (this->m_selection.empty() ? target->mesh()->vn : this->m_selection.size()));

	eigen::Matrix3xN originals(3, size);
	eigen::Matrix3xN transforms(3, size);
	eigen::Matrix3xN results(3, size);

	// Builds queries.
	if (this->m_selection.empty()) {
		if (target->mesh()->vn > MAXIMUM_QUERIES) {
			real_t factor = target->mesh()->vn / (real_t)MAXIMUM_QUERIES;
			for (size_t i = 0; i < size; ++i) {
				originals.col(i) = algo::Util::toVector(target->mesh()->vert[i * factor].cP());
				transforms.col(i) = algo::Util::toVector(target->mesh()->vert[i * factor].cP());
			}
		} else {
			algo::Util::toMatrix(*target->mesh(), &originals);
			algo::Util::toMatrix(*target->mesh(), &transforms);
		}
	} else {
		if (this->m_selection.size() > MAXIMUM_QUERIES) {
			real_t factor = this->m_selection.size() / (real_t)MAXIMUM_QUERIES;
			for (size_t i = 0; i < size; ++i) {
				originals.col(i) = algo::Util::toVector(target->mesh()->vert[this->m_selection[i * factor]].cP());
				transforms.col(i) = algo::Util::toVector(target->mesh()->vert[this->m_selection[i * factor]].cP());
			}
		} else {
			algo::Util::toMatrix(*target->mesh(), this->m_selection, &originals);
			algo::Util::toMatrix(*target->mesh(), this->m_selection, &transforms);
		}
	}
	if (transforms.cols() < 6) {
		_VGR_SYSTEM_ERR("Constraints are insufficient. Registration aborted.");
		return (false);
	}

	flann::Matrix<float> queries(new float[size * 3], size, 3);
	flann::Matrix<int> indices(new int[size], size, 1);
	flann::Matrix<float> dists(new float[size], size, 1);
	size_t count = 0;
	real_t lastDiff = 0, currentDiff = 0;

	// Performs ICP.
	_VGR_SYSTEM_MSG("Performing ICP.");
	_VGR_LOGGER_BEG_PROG;
	_VGR_LOGGER_BSY_PROG;
	while (true) {

		_VGR_SYSTEM_MSG(boost::format("Performing ICP round %1%.") % count);

		for (size_t i = 0; i < size; ++i) {
			queries[i][0] = transforms(0, i);
			queries[i][1] = transforms(1, i);
			queries[i][2] = transforms(2, i);
		}

		index.knnSearch(queries, indices, dists, 1, flann::SearchParams(128));

		for (size_t i = 0; i < size; ++i) {
			results.col(i) = algo::Util::toVector(source->mesh()->vert[indices[i][0]].cP());
		}

		// Computes the transform matrices and updates feature vertices queires.
		algo::Register::transformation(originals, results, &this->m_rotation, &this->m_translation);
		transforms = this->m_rotation * originals;
		transforms.colwise() += this->m_translation;
		currentDiff = (results - transforms).colwise().norm().mean();

		_VGR_SYSTEM_MSG(boost::format("ICP round %1% done, difference %2%.") % count % std::abs(lastDiff - currentDiff));

		// Checks conditions to break to the loop.
		if (algo::Util::zero(lastDiff - currentDiff) || !(++count < 40)) break;

		// Updates vertices differences.
		lastDiff = currentDiff;
	}
	_VGR_LOGGER_END_PROG;

	delete[] dataset.ptr();
	delete[] queries.ptr();
	delete[] indices.ptr();
	delete[] dists.ptr();

	return (true);
}
