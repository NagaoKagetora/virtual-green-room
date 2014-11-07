#pragma once
#ifndef _VGR_CORE_ALGORITHM_REGISTRATION_LEASTSQUARESREGISTRATIONPROVIDER_HPP
#define _VGR_CORE_ALGORITHM_REGISTRATION_LEASTSQUARESREGISTRATIONPROVIDER_HPP

#include <common/global.hpp>
#include <core/basic/Model.hpp>
#include <core/algorithm/registration/CorrespondenceRegistrationProvider.hpp>



class LeastSquaresRegistrationProvider;



class LeastSquaresRegistrationProvider : public CorrespondenceRegistrationProvider {

private:
	std::vector<size_t>					m_selection;

public:
	template <typename Iterator>
	LeastSquaresRegistrationProvider(Iterator begin, Iterator end) : m_selection(begin, end) { }

protected:
	virtual bool regist(Model *source, Model *target);
};



#endif /* _VGR_CORE_ALGORITHM_REGISTRATION_LEASTSQUARESREGISTRATIONPROVIDER_HPP */
