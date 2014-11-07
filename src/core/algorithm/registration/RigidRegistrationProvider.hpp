#pragma once
#ifndef _VGR_CORE_ALGORITHM_REGISTRATION_RIGIDREGISTRATIONPROVIDER_HPP
#define _VGR_CORE_ALGORITHM_REGISTRATION_RIGIDREGISTRATIONPROVIDER_HPP

#include <common/global.hpp>
#include <common/3rdparty/eigen.hpp>
#include <core/basic/Model.hpp>
#include <core/algorithm/registration/RegistrationProvider.hpp>



class RigidRegistrationProvider;



class RigidRegistrationProvider : public RegistrationProvider {

protected:
	eigen::Vector3						m_translation;
	eigen::Matrix3						m_rotation;

public:
	virtual bool operator ()(Model *source, Model *target);

protected:
	virtual bool regist(Model *source, Model *target) { return (true); }

private:
	void transform(Model *model) const;
};



#endif /* _VGR_CORE_ALGORITHM_REGISTRATION_RIGIDREGISTRATIONPROVIDER_HPP */
