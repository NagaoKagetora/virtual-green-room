#pragma once
#ifndef _VGR_CORE_ALGORITHM_REGISTRATION_REGISTRATIONPROVIDER_HPP
#define _VGR_CORE_ALGORITHM_REGISTRATION_REGISTRATIONPROVIDER_HPP

#include <osg/Referenced>
#include <common/global.hpp>
#include <core/basic/Model.hpp>



class RegistrationProvider;



class RegistrationProvider : public osg::Referenced {

public:
	virtual bool operator ()(Model *source, Model *target) { return (true); }
};



#endif /* _VGR_CORE_ALGORITHM_REGISTRATION_REGISTRATIONPROVIDER_HPP */
