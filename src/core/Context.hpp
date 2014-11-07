#pragma once
#ifndef _VGR_CORE_CONTEXT_HPP
#define _VGR_CORE_CONTEXT_HPP

#include <common/global.hpp>
#include <osgViewer/View>
#include <core/basic/Scene.hpp>
#include <core/Registration.hpp>
#include <core/Reflection.hpp>
#include <core/Section.hpp>
#include <core/Colorization.hpp>
#include <core/Drainage.hpp>
#include <core/Stonecheck.hpp>



class Context;



class Context {

public:
	class RenderSettings {

	private:
		osg::ref_ptr<osg::Uniform>		m_pointSize;
		osg::ref_ptr<osg::Uniform>		m_pointColor;
		osg::ref_ptr<osg::Uniform>		m_lineWidth;
		osg::ref_ptr<osg::Uniform>		m_lineColor;
		osg::ref_ptr<osg::Uniform>		m_lightPosition;
		osg::ref_ptr<osg::Uniform>		m_lightIntensity;
		osg::ref_ptr<osg::Uniform>		m_frontMaterialAmbient;
		osg::ref_ptr<osg::Uniform>		m_frontMaterialDiffuse;
		osg::ref_ptr<osg::Uniform>		m_frontMaterialSpecular;
		osg::ref_ptr<osg::Uniform>		m_frontMaterialShininess;
		osg::ref_ptr<osg::Uniform>		m_backMaterialAmbient;
		osg::ref_ptr<osg::Uniform>		m_backMaterialDiffuse;
		osg::ref_ptr<osg::Uniform>		m_backMaterialSpecular;
		osg::ref_ptr<osg::Uniform>		m_backMaterialShininess;
		osg::ref_ptr<osg::Uniform>		m_effectMaterialAmbient;
		osg::ref_ptr<osg::Uniform>		m_effectMaterialDiffuse;
		osg::ref_ptr<osg::Uniform>		m_effectMaterialSpecular;
		osg::ref_ptr<osg::Uniform>		m_effectMaterialShininess;
		osg::ref_ptr<osg::Uniform>		m_vertexMode;
		osg::ref_ptr<osg::Uniform>		m_edgeMode;
		osg::ref_ptr<osg::Uniform>		m_faceMode;

	public:
		RenderSettings();

	public:
		const osg::ref_ptr<osg::Uniform> pointSize() const { return (this->m_pointSize); }
		osg::ref_ptr<osg::Uniform> pointSize() { return (this->m_pointSize); }
		const osg::ref_ptr<osg::Uniform> pointColor() const { return (this->m_pointColor); }
		osg::ref_ptr<osg::Uniform> pointColor() { return (this->m_pointColor); }
		const osg::ref_ptr<osg::Uniform> lineWidth() const { return (this->m_lineWidth); }
		osg::ref_ptr<osg::Uniform> lineWidth() { return (this->m_lineWidth); }
		const osg::ref_ptr<osg::Uniform> lineColor() const { return (this->m_lineColor); }
		osg::ref_ptr<osg::Uniform> lineColor() { return (this->m_lineColor); }
		const osg::ref_ptr<osg::Uniform> lightPosition() const { return (this->m_lightPosition); }
		osg::ref_ptr<osg::Uniform> lightPosition() { return (this->m_lightPosition); }
		const osg::ref_ptr<osg::Uniform> lightIntensity() const { return (this->m_lightIntensity); }
		osg::ref_ptr<osg::Uniform> lightIntensity() { return (this->m_lightIntensity); }
		const osg::ref_ptr<osg::Uniform> frontMaterialAmbient() const { return (this->m_frontMaterialAmbient); }
		osg::ref_ptr<osg::Uniform> frontMaterialAmbient() { return (this->m_frontMaterialAmbient); }
		const osg::ref_ptr<osg::Uniform> frontMaterialDiffuse() const { return (this->m_frontMaterialDiffuse); }
		osg::ref_ptr<osg::Uniform> frontMaterialDiffuse() { return (this->m_frontMaterialDiffuse); }
		const osg::ref_ptr<osg::Uniform> frontMaterialSpecular() const { return (this->m_frontMaterialSpecular); }
		osg::ref_ptr<osg::Uniform> frontMaterialSpecular() { return (this->m_frontMaterialSpecular); }
		const osg::ref_ptr<osg::Uniform> frontMaterialShininess() const { return (this->m_frontMaterialShininess); }
		osg::ref_ptr<osg::Uniform> frontMaterialShininess() { return (this->m_frontMaterialShininess); }
		const osg::ref_ptr<osg::Uniform> backMaterialAmbient() const { return (this->m_backMaterialAmbient); }
		osg::ref_ptr<osg::Uniform> backMaterialAmbient() { return (this->m_backMaterialAmbient); }
		const osg::ref_ptr<osg::Uniform> backMaterialDiffuse() const { return (this->m_backMaterialDiffuse); }
		osg::ref_ptr<osg::Uniform> backMaterialDiffuse() { return (this->m_backMaterialDiffuse); }
		const osg::ref_ptr<osg::Uniform> backMaterialSpecular() const { return (this->m_backMaterialSpecular); }
		osg::ref_ptr<osg::Uniform> backMaterialSpecular() { return (this->m_backMaterialSpecular); }
		const osg::ref_ptr<osg::Uniform> backMaterialShininess() const { return (this->m_backMaterialShininess); }
		osg::ref_ptr<osg::Uniform> backMaterialShininess() { return (this->m_backMaterialShininess); }
		const osg::ref_ptr<osg::Uniform> effectMaterialAmbient() const { return (this->m_effectMaterialAmbient); }
		osg::ref_ptr<osg::Uniform> effectMaterialAmbient() { return (this->m_effectMaterialAmbient); }
		const osg::ref_ptr<osg::Uniform> effectMaterialDiffuse() const { return (this->m_effectMaterialDiffuse); }
		osg::ref_ptr<osg::Uniform> effectMaterialDiffuse() { return (this->m_effectMaterialDiffuse); }
		const osg::ref_ptr<osg::Uniform> effectMaterialSpecular() const { return (this->m_effectMaterialSpecular); }
		osg::ref_ptr<osg::Uniform> effectMaterialSpecular() { return (this->m_effectMaterialSpecular); }
		const osg::ref_ptr<osg::Uniform> effectMaterialShininess() const { return (this->m_effectMaterialShininess); }
		osg::ref_ptr<osg::Uniform> effectMaterialShininess() { return (this->m_effectMaterialShininess); }
		const osg::ref_ptr<osg::Uniform> vertexMode() const { return (this->m_vertexMode); }
		osg::ref_ptr<osg::Uniform> vertexMode() { return (this->m_vertexMode); }
		const osg::ref_ptr<osg::Uniform> edgeMode() const { return (this->m_edgeMode); }
		osg::ref_ptr<osg::Uniform> edgeMode() { return (this->m_edgeMode); }
		const osg::ref_ptr<osg::Uniform> faceMode() const { return (this->m_faceMode); }
		osg::ref_ptr<osg::Uniform> faceMode() { return (this->m_faceMode); }

	public:
		void updateSettings();
	};

private:
	RenderSettings						m_renderSettings;

public:
	Context() : m_renderSettings() { }

public:
	const RenderSettings &renderSettings() const { return (this->m_renderSettings); }
	RenderSettings &renderSettings() { return (this->m_renderSettings); }

public:
	void regist(Scene *scene);
};



#endif /* _VGR_CORE_CONTEXT_HPP */
