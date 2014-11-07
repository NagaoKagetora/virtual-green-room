#pragma once
#ifndef _VGR_CORE_ALGORITHM_COLORIZATION_YCOMPONENTCOLORIZATIONPROVIDER_HPP
#define _VGR_CORE_ALGORITHM_COLORIZATION_YCOMPONENTCOLORIZATIONPROVIDER_HPP

#include <common/global.hpp>
#include <core/algorithm/colorization/ScalarColorizationProvider.hpp>



class YComponentColorizationProvider;



class YComponentColorizationProvider : public ScalarColorizationProvider {

public:
	YComponentColorizationProvider() : ScalarColorizationProvider() { }

protected:
	virtual bool compute(Model *source, Model *target);
};



#endif /* _VGR_CORE_ALGORITHM_COLORIZATION_YCOMPONENTCOLORIZATIONPROVIDER_HPP */
