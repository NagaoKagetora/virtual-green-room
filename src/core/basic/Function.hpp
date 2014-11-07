#pragma once
#ifndef _VGR_CORE_BASIC_FUNCTION_HPP
#define _VGR_CORE_BASIC_FUNCTION_HPP

#include <common/global.hpp>
#include <core/basic/Handler.hpp>

using namespace vgr;



class Scene;
class StandardScene;
class Settings;
class Results;
class Function;



class Settings {

public:
	bool								activated;

public:
	virtual ~Settings() { }
};



class Results {

public:
	virtual ~Results() { }
};



class Function {

public:
	virtual osg::ref_ptr<Handler> handler() { return (NULL); }
	virtual void setViewport(int width, int height);
	virtual void reset() { }
	virtual void bind(std::vector<StandardScene *> scenes) { }
	virtual std::vector<Scene *> views() { return (std::vector<Scene *>()); }
	virtual std::vector<Scene *> layout(const Settings &settings) { return (std::vector<Scene *>()); }
	virtual void activate(const Settings &settings) { }
	virtual void modify(const Settings &settings) { }
	virtual void update(const Settings &settings) { }
	virtual void execute(const Settings &settings) { }
	virtual const Results &getResults() const {
		static Results results;
		return (results);
	}

protected:
	virtual void setIndicator(Indicator *indicator);
	virtual void setOperation(Operation *operation);
	virtual void attach(StandardScene *scene);
	virtual void detach(StandardScene *scene);
};



#endif /* _VGR_CORE_BASIC_FUNCTION_HPP */
