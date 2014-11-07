#include "Text.hpp"
#include <common/3rdparty/boost.hpp>



Text::Text() : osg::Geode() {
	this->m_text = new osgText::Text();
	this->addDrawable(this->m_text);
}

void Text::setPosition(float x, float y) {
	this->m_text->setPosition(osg::Vec3(x, y, 0.0f));
	this->m_text->dirtyDisplayList();
	this->m_text->dirtyBound();
	this->dirtyBound();
}

void Text::setText(const std::string &text) {
	this->m_text->setText(text);
	this->m_text->dirtyDisplayList();
	this->m_text->dirtyBound();
	this->dirtyBound();
}

void Text::setText(int value) {
	this->setText(boost::str(boost::format("%1%") % value));
}

void Text::setText(double value) {
	this->setText(boost::str(boost::format("%1%") % value));
}

void Text::setColor(osg::Vec4 color) {
	this->m_text->setColor(color);
	this->m_text->dirtyDisplayList();
	this->m_text->dirtyBound();
	this->dirtyBound();
}

void Text::setFont(const std::string &name) {
	this->m_text->setFont(osgText::readFontFile(name));
	this->m_text->dirtyDisplayList();
	this->m_text->dirtyBound();
	this->dirtyBound();
}

void Text::setFontSize(int size) {
	this->m_text->setCharacterSize(size);
	this->m_text->setFontResolution(size, size);
	this->m_text->dirtyDisplayList();
	this->m_text->dirtyBound();
	this->dirtyBound();
}

void Text::setAlignment(osgText::Text::AlignmentType alignment) {
	this->m_text->setAlignment(alignment);
	this->m_text->dirtyDisplayList();
	this->m_text->dirtyBound();
	this->dirtyBound();
}
