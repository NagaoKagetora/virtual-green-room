#pragma once
#ifndef _VGR_CORE_ALGORITHM_COLORIZATION_COLORIZATIONPROVIDER_HPP
#define _VGR_CORE_ALGORITHM_COLORIZATION_COLORIZATIONPROVIDER_HPP

#include <osg/Referenced>
#include <common/global.hpp>
#include <core/basic/Model.hpp>



class ColorizationProvider;



class ColorizationProvider : public osg::Referenced {

public:
	virtual bool operator ()(Model *source, Model *target, real_t &peak) { return (true); }
};



#endif /* _VGR_CORE_ALGORITHM_COLORIZATION_COLORIZATIONPROVIDER_HPP */
