#pragma once
#ifndef _VGR_CORE_ALGORITHM_COLORIZATION_NDEVIATIONCOLORIZATIONPROVIDER_HPP
#define _VGR_CORE_ALGORITHM_COLORIZATION_NDEVIATIONCOLORIZATIONPROVIDER_HPP

#include <common/global.hpp>
#include <core/algorithm/colorization/ScalarColorizationProvider.hpp>



class NDeviationColorizationProvider;



class NDeviationColorizationProvider : public ScalarColorizationProvider {

public:
	NDeviationColorizationProvider() : ScalarColorizationProvider() { }

protected:
	virtual bool compute(Model *source, Model *target);
};



#endif /* _VGR_CORE_ALGORITHM_COLORIZATION_NDEVIATIONCOLORIZATIONPROVIDER_HPP */
