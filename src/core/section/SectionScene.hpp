#pragma once
#ifndef _VGR_CORE_SECTION_SECTIONSCENE_HPP
#define _VGR_CORE_SECTION_SECTIONSCENE_HPP

#include <osgViewer/View>
#include <common/global.hpp>
#include <core/basic/sprite/Hud.hpp>
#include <core/basic/sprite/Rect.hpp>
#include <core/basic/sprite/Text.hpp>
#include <core/basic/sprite/Transform.hpp>
#include <core/basic/Scene.hpp>
#include <core/algorithm/section/Curve.hpp>



class SectionScene;



class SectionScene : public Scene {

public:
	class Sprite : public osg::Referenced {

	private:
		osg::ref_ptr<Text>				m_titleText;
		osg::ref_ptr<Text>				m_sourceNameText;
		osg::ref_ptr<Text>				m_targetNameText;
		osg::ref_ptr<Text>				m_xText;
		osg::ref_ptr<Text>				m_yMaxText;
		osg::ref_ptr<Text>				m_yMinText;
		osg::ref_ptr<Rect>				m_sourceLegendRect;
		osg::ref_ptr<Transform2D>		m_sourceLegendTransform;
		osg::ref_ptr<Rect>				m_targetLegendRect;
		osg::ref_ptr<Transform2D>		m_targetLegendTransform;
		osg::ref_ptr<osg::Vec3Array>	m_gridVertices;
		osg::ref_ptr<osg::Vec4Array>	m_gridColors;
		osg::ref_ptr<osg::DrawArrays>	m_gridPrimitives;
		osg::ref_ptr<osg::Geometry>		m_gridGeometry;
		osg::ref_ptr<osg::Vec3Array>	m_zeroLineVertices;
		osg::ref_ptr<osg::Vec4Array>	m_zeroLineColors;
		osg::ref_ptr<osg::DrawArrays>	m_zeroLinePrimitives;
		osg::ref_ptr<osg::Geometry>		m_zeroLineGeometry;
		osg::ref_ptr<osg::Geode>		m_gridGeode;
		osg::ref_ptr<Transform2D>		m_gridTransform;
		osg::ref_ptr<Hud>				m_hud;
		osg::ref_ptr<osg::Group>		m_node;

	public:
		Sprite();

	public:
		const osg::ref_ptr<osg::Group> &node() const { return (this->m_node); }
		osg::ref_ptr<osg::Group> &node() { return (this->m_node); }

	public:
		void setCaption(const std::string &title, const std::string &sourceName, const std::string &targetName);
		void setCaption(const std::string &title);
		void setNumbers(real_t x, real_t yMax, real_t yMin);
		void setGrid(int verticalSlices, int horizontalSlices);
		void setZeroLine(bool symmetric);

	public:
		virtual void setViewport(int width, int height);
	};

	class Content : public osg::Referenced {

	private:
		osg::ref_ptr<osg::Geode>		m_geode;
		osg::ref_ptr<osg::Camera>		m_camera;
		osg::ref_ptr<osg::Group>		m_node;

	public:
		Content();

	public:
		const osg::ref_ptr<osg::Group> &node() const { return (this->m_node); }
		osg::ref_ptr<osg::Group> &node() { return (this->m_node); }

	public:
		void updateCurves(const Curve &source, const Curve &target);
		void updateCurves(const Curve &source, real_t acceptance, real_t rejection);

	public:
		virtual void setViewport(int width, int height);

	private:
		void addLineCurve(const Curve &curve, osg::Vec4 color);
		void addBarCurve(const Curve &curve, real_t acceptance, real_t rejection);
	};

private:
	std::string							m_caption;
	osg::ref_ptr<Sprite>				m_sprite;
	osg::ref_ptr<Content>				m_content;
	osg::ref_ptr<osg::Group>			m_root;

public:
	SectionScene();

public:
	void updateCaption(const std::string &title, const std::string &sourceName, const std::string &targetName);
	void updateCaption(const std::string &title);
	void updateScene(const Curve &source, const Curve &target, Curve::NormalizeOption option);
	void updateScene(const Curve &source, real_t acceptance, real_t rejection);

public:
	virtual std::string caption() const { return (this->m_caption); }
	virtual void setViewport(int width, int height);
};



#pragma warning (pop)

#endif /* _VGR_CORE_SECTION_SECTIONSCENE_HPP */
