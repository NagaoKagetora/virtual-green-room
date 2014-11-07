#pragma once
#ifndef _VGR_CORE_ALGORITHM_COLORIZATION_CURVATURECOLORIZATIONPROVIDER_HPP
#define _VGR_CORE_ALGORITHM_COLORIZATION_CURVATURECOLORIZATIONPROVIDER_HPP

#include <common/global.hpp>
#include <core/algorithm/colorization/ScalarColorizationProvider.hpp>



class CurvatureColorizationProvider;



class CurvatureColorizationProvider : public ScalarColorizationProvider {

public:
	CurvatureColorizationProvider() : ScalarColorizationProvider() { }

protected:
	virtual bool compute(Model *source, Model *target);
};



#endif /* _VGR_CORE_ALGORITHM_COLORIZATION_CURVATURECOLORIZATIONPROVIDER_HPP */
