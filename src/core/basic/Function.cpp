#include "Function.hpp"
#include <core/basic/Scene.hpp>



void Function::setViewport(int width, int height) {
	if (!this->handler()) return;
	this->handler()->setViewport(width, height);
}

void Function::setIndicator(Indicator *indicator) {
	if (!indicator || !this->handler()) return;
	this->handler()->setIndicator(indicator);
}

void Function::setOperation(Operation *operation) {
	if (!operation || !this->handler()) return;
	this->handler()->setOperation(operation);
}

void Function::attach(StandardScene *scene) {
	if (!scene || !scene->ready() || !this->handler()) return;
	scene->addHandler(this->handler());
	this->handler()->setViewport(scene->width(), scene->height());
}

void Function::detach(StandardScene *scene) {
	if (!scene || !scene->ready() || !this->handler()) return;
	scene->removeHandler(this->handler());
}
