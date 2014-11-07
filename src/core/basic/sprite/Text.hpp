#pragma once
#ifndef _VGR_CORE_BASIC_SPRITE_TEXT_HPP
#define _VGR_CORE_BASIC_SPRITE_TEXT_HPP

#include <osg/Geode>
#include <osgText/Text>



class Text;



class Text : public osg::Geode {

private:
	osg::ref_ptr<osgText::Text>			m_text;

public:
	Text();

public:
	void setPosition(osg::Vec2 position) { this->setPosition(position.x(), position.y()); }
	void setPosition(float x, float y);
	void setText(const std::string &text);
	void setText(int value);
	void setText(double value);
	void setColor(osg::Vec4 color);
	void setFont(const std::string &name);
	void setFontSize(int size);
	void setAlignment(osgText::Text::AlignmentType alignment);
};



#endif /* _VGR_CORE_BASIC_SPRITE_TEXT_HPP */
