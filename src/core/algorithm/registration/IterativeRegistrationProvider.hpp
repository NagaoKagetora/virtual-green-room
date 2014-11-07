#pragma once
#ifndef _VGR_CORE_ALGORITHM_REGISTRATION_ITERATIVEREGISTRATIONPROVIDER_HPP
#define _VGR_CORE_ALGORITHM_REGISTRATION_ITERATIVEREGISTRATIONPROVIDER_HPP

#include <common/global.hpp>
#include <common/3rdparty/eigen.hpp>
#include <core/basic/Model.hpp>
#include <core/algorithm/registration/RigidRegistrationProvider.hpp>



class IterativeRegistrationProvider;



class IterativeRegistrationProvider : public RigidRegistrationProvider {

private:
	std::vector<size_t>					m_selection;

public:
	template <typename Iterator>
	IterativeRegistrationProvider(Iterator begin, Iterator end) : m_selection(begin, end) { }

protected:
	virtual bool regist(Model *source, Model *target);
};



#endif /* _VGR_CORE_ALGORITHM_REGISTRATION_ITERATIVEREGISTRATIONPROVIDER_HPP */
