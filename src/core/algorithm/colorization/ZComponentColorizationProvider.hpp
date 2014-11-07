#pragma once
#ifndef _VGR_CORE_ALGORITHM_COLORIZATION_ZCOMPONENTCOLORIZATIONPROVIDER_HPP
#define _VGR_CORE_ALGORITHM_COLORIZATION_ZCOMPONENTCOLORIZATIONPROVIDER_HPP

#include <common/global.hpp>
#include <core/algorithm/colorization/ScalarColorizationProvider.hpp>



class ZComponentColorizationProvider;



class ZComponentColorizationProvider : public ScalarColorizationProvider {

public:
	ZComponentColorizationProvider() : ScalarColorizationProvider() { }

protected:
	virtual bool compute(Model *source, Model *target);
};



#endif /* _VGR_CORE_ALGORITHM_COLORIZATION_ZCOMPONENTCOLORIZATIONPROVIDER_HPP */
