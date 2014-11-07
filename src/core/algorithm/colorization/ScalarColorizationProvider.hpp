#pragma once
#ifndef _VGR_CORE_ALGORITHM_COLORIZATION_SCALARCOLORIZATIONPROVIDER_HPP
#define _VGR_CORE_ALGORITHM_COLORIZATION_SCALARCOLORIZATIONPROVIDER_HPP

#include <common/global.hpp>
#include <common/3rdparty/eigen.hpp>
#include <core/algorithm/colorization/ColorizationProvider.hpp>



class ScalarColorizationProvider;



class ScalarColorizationProvider : public ColorizationProvider {

protected:
	eigen::Vector						m_attributes;
	std::vector<bool>					m_valid;
	real_t								m_peak;

public:
	ScalarColorizationProvider();

public:
	virtual bool operator ()(Model *source, Model *target, real_t &peak);

protected:
	virtual bool compute(Model *source, Model *target) { return (true); }

private:
	void colorize(Model *model) const;
};



#endif /* _VGR_CORE_ALGORITHM_COLORIZATION_SCALARCOLORIZATIONPROVIDER_HPP */
