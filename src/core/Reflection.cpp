#include "Reflection.hpp"
#include <core/basic/Scene.hpp>



ReflectionFunction::ReflectionFunction() {
	this->m_start = new osg::Uniform("reflection.start", osg::Vec2(0.0f, 0.0f));
	this->m_stop = new osg::Uniform("reflection.stop", osg::Vec2(0.0f, 0.0f));
	this->m_mode = new osg::Uniform("reflection.mode", 0);
	this->m_scale = new osg::Uniform("reflection.scale", 0.0f);
	this->m_rotation = new osg::Uniform("reflection.rotation", 0.0f);
	this->reset();
}

void ReflectionFunction::reset() {
	this->m_handler = new ReflectionHandler(this->m_start, this->m_stop);
}

void ReflectionFunction::bind(std::vector<StandardScene *> scenes) {
	this->m_scenes.resize(scenes.size());
	this->m_activated.resize(scenes.size());
	for (size_t i = 0; i < scenes.size(); ++i) {
		this->m_scenes[i] = scenes[i];
		this->m_activated[i] = new osg::Uniform("reflection.activated", false);
		osg::StateSet *states = this->m_scenes[i]->content()->node()->getOrCreateStateSet();
		states->addUniform(this->m_activated[i]);
		states->addUniform(this->m_start);
		states->addUniform(this->m_stop);
		states->addUniform(this->m_mode);
		states->addUniform(this->m_scale);
		states->addUniform(this->m_rotation);
	}
}

void ReflectionFunction::activate(const Settings &settings) {
	if (typeid(settings) != typeid(ReflectionSettings)) return;
	const ReflectionSettings &instance(dynamic_cast<const ReflectionSettings &>(settings));

	for (size_t i = 0; i < this->m_scenes.size(); ++i) {
		if (!this->m_scenes[i]->ready()) continue;
		this->m_activated[i]->set(instance.activated);
		this->m_activated[i]->dirty();
	}

	this->modify(settings);
}

void ReflectionFunction::modify(const Settings &settings) {
	if (typeid(settings) != typeid(ReflectionSettings)) return;
	const ReflectionSettings &instance(dynamic_cast<const ReflectionSettings &>(settings));

	// Disposes and detaches from scenes the previous handler.
	for (size_t i = 0; i < this->m_scenes.size(); ++i) this->detach(this->m_scenes[i]);
	if (this->m_handler) this->m_handler->dispose();

	// Creates and attaches to scenes the new handler.
	if (instance.activated) {
		for (size_t i = 0; i < this->m_scenes.size(); ++i) {
			if (!this->m_scenes[i]->ready()) continue;
			this->attach(this->m_scenes[i]);
		}
	}

	this->update(settings);
}

void ReflectionFunction::update(const Settings &settings) {
	if (typeid(settings) != typeid(ReflectionSettings)) return;
	const ReflectionSettings &instance(dynamic_cast<const ReflectionSettings &>(settings));

	this->m_mode->set(instance.mode);
	this->m_mode->dirty();
	this->m_scale->set((float)instance.scale);
	this->m_scale->dirty();
	this->m_rotation->set((float)instance.rotation);
	this->m_rotation->dirty();
}
