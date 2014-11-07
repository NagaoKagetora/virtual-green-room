#pragma once
#ifndef _VGR_CORE_ALGORITHM_COLORIZATION_XCOMPONENTCOLORIZATIONPROVIDER_HPP
#define _VGR_CORE_ALGORITHM_COLORIZATION_XCOMPONENTCOLORIZATIONPROVIDER_HPP

#include <common/global.hpp>
#include <core/algorithm/colorization/ScalarColorizationProvider.hpp>



class XComponentColorizationProvider;



class XComponentColorizationProvider : public ScalarColorizationProvider {

public:
	XComponentColorizationProvider() : ScalarColorizationProvider() { }

protected:
	virtual bool compute(Model *source, Model *target);
};



#endif /* _VGR_CORE_ALGORITHM_COLORIZATION_XCOMPONENTCOLORIZATIONPROVIDER_HPP */
