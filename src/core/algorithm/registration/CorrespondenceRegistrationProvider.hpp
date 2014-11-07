#pragma once
#ifndef _VGR_CORE_ALGORITHM_REGISTRATION_CORRESPONDENCEREGISTRATIONPROVIDER_HPP
#define _VGR_CORE_ALGORITHM_REGISTRATION_CORRESPONDENCEREGISTRATIONPROVIDER_HPP

#include <common/global.hpp>
#include <common/3rdparty/eigen.hpp>
#include <core/basic/Model.hpp>
#include <core/algorithm/registration/RigidRegistrationProvider.hpp>



class CorrespondenceRegistrationProvider;



class CorrespondenceRegistrationProvider : public RigidRegistrationProvider {

public:
	virtual bool operator ()(Model *source, Model *target);

private:
	bool verify(const Model *source, const Model *target) const;
};



#endif /* _VGR_CORE_ALGORITHM_REGISTRATION_CORRESPONDENCEREGISTRATIONPROVIDER_HPP */
