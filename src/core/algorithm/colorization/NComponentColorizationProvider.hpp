#pragma once
#ifndef _VGR_CORE_ALGORITHM_COLORIZATION_NCOMPONENTCOLORIZATIONPROVIDER_HPP
#define _VGR_CORE_ALGORITHM_COLORIZATION_NCOMPONENTCOLORIZATIONPROVIDER_HPP

#include <common/global.hpp>
#include <core/algorithm/colorization/ScalarColorizationProvider.hpp>



class NComponentColorizationProvider;



class NComponentColorizationProvider : public ScalarColorizationProvider {

public:
	NComponentColorizationProvider() : ScalarColorizationProvider() { }

protected:
	virtual bool compute(Model *source, Model *target);
};



#endif /* _VGR_CORE_ALGORITHM_COLORIZATION_NCOMPONENTCOLORIZATIONPROVIDER_HPP */
