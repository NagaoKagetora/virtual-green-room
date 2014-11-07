#pragma once
#ifndef _VGR_CORE_ALGORITHM_REGISTRATION_THREEPOINTREGISTRATIONPROVIDER_HPP
#define _VGR_CORE_ALGORITHM_REGISTRATION_THREEPOINTREGISTRATIONPROVIDER_HPP

#include <common/global.hpp>
#include <core/basic/Model.hpp>
#include <core/algorithm/registration/CorrespondenceRegistrationProvider.hpp>



class ThreePointRegistrationProvider;



class ThreePointRegistrationProvider : public CorrespondenceRegistrationProvider {

private:
	int									m_origin;
	int									m_start;
	int									m_stop;

public:
	ThreePointRegistrationProvider(int origin, int start, int stop) : m_origin(origin), m_start(start), m_stop(stop) { }

protected:
	virtual bool regist(Model *source, Model *target);
};



#endif /* _VGR_CORE_ALGORITHM_REGISTRATION_THREEPOINTREGISTRATIONPROVIDER_HPP */
