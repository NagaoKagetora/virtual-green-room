#pragma once
#ifndef _VGR_CORE_BASIC_CONFIGURATION_HPP
#define _VGR_CORE_BASIC_CONFIGURATION_HPP

#include <common/global.hpp>
#include <osg/Vec3>
#include <osg/Vec4>
#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QMap>
#include <QtCore/QVariant>
#include <QtGui/QColor>



class QDataStream;
class Converter;
class Configuration;
class Session;



class Converter {

private:
	Converter() { }
	~Converter() { }

public:
	static QColor toQColor(const osg::Vec3 &color) { return (QColor::fromRgbF(color[0], color[1], color[2])); }
	static QColor toQColor(const osg::Vec4 &color) { return (QColor::fromRgbF(color[0], color[1], color[2], color[3])); }
	static osg::Vec3 toVec3(const QColor &color) { return (osg::Vec3(color.redF(), color.greenF(), color.blueF())); }
	static osg::Vec4 toVec4(const QColor &color) { return (osg::Vec4(color.redF(), color.greenF(), color.blueF(), color.alphaF())); }
};



class Configuration : public QObject {

public:
	class ConfigurationBase {

	public:
		virtual void deserialize(QDataStream &stream) = 0;
		virtual void serialize(QDataStream &stream) const = 0;
	};

	class ApplicationConfiguration : public ConfigurationBase {

	private:
		QString							m_workspacePath;
		QString							m_recentDirectory;
		bool							m_usingRecentDirectory;
		bool							m_usingAutoRepairVertexConnectivities;
		bool							m_usingAutoRepairFaceConnectivities;

	public:
		ApplicationConfiguration();

	public:
		const QString &workspacePath() const { return (this->m_workspacePath); }
		void setWorkspacePath(const QString &value) { this->m_workspacePath = value; }
		const QString &recentDirectory() const { return (this->m_recentDirectory); }
		void setRecentDirectory(const QString &value) { this->m_recentDirectory = value; }
		bool usingRecentDirectory() const { return (this->m_usingRecentDirectory); }
		void setUsingRecentDirectory(bool value) { this->m_usingRecentDirectory = value; }
		bool usingAutoRepairVertexConnectivities() const { return (this->m_usingAutoRepairFaceConnectivities); }
		void setUsingAutoRepairVertexConnectivities(bool value) { this->m_usingAutoRepairVertexConnectivities = value; }
		bool usingAutoRepairFaceConnectivities() const { return (this->m_usingAutoRepairFaceConnectivities); }
		void setUsingAutoRepairFaceConnectivities(bool value) { this->m_usingAutoRepairFaceConnectivities = value; }

	public:
		virtual void deserialize(QDataStream &stream);
		virtual void serialize(QDataStream &stream) const;
	};

	class SceneConfiguration : public ConfigurationBase {

	private:
		QColor							m_sceneBackgroundColor;
		qreal							m_pointSize;
		QColor							m_pointColor;
		qreal							m_lineWidth;
		QColor							m_lineColor;
		QColor							m_lightIntensity;
		QColor							m_frontMaterialAmbient;
		QColor							m_frontMaterialDiffuse;
		QColor							m_frontMaterialSpecular;
		qreal							m_frontMaterialShininess;
		QColor							m_backMaterialAmbient;
		QColor							m_backMaterialDiffuse;
		QColor							m_backMaterialSpecular;
		qreal							m_backMaterialShininess;
		QColor							m_effectMaterialAmbient;
		QColor							m_effectMaterialDiffuse;
		QColor							m_effectMaterialSpecular;
		qreal							m_effectMaterialShininess;

	public:
		SceneConfiguration();

	public:
		const QColor &sceneBackgroundColor() const { return (this->m_sceneBackgroundColor); }
		void setSceneBackgroundColor(const QColor &value) { this->m_sceneBackgroundColor = value; }
		qreal pointSize() const { return (this->m_pointSize); }
		void setPointSize(qreal value) { this->m_pointSize = value; }
		const QColor &pointColor() const { return (this->m_pointColor); }
		void setPointColor(const QColor &value) { this->m_pointColor = value; }
		qreal lineWidth() const { return (this->m_lineWidth); }
		void setLineWidth(qreal value) { this->m_lineWidth = value; }
		const QColor &lineColor() const { return (this->m_lineColor); }
		void setLineColor(const QColor &value) { this->m_lineColor = value; }
		const QColor &lightIntensity() const { return (this->m_lightIntensity); }
		void setLightIntensity(const QColor &value) { this->m_lightIntensity = value; }
		const QColor &frontMaterialAmbient() const { return (this->m_frontMaterialAmbient); }
		void setFrontMaterialAmbient(const QColor &value) { this->m_frontMaterialAmbient = value; }
		const QColor &frontMaterialDiffuse() const { return (this->m_frontMaterialDiffuse); }
		void setFrontMaterialDiffuse(const QColor &value) { this->m_frontMaterialDiffuse = value; }
		const QColor &frontMaterialSpecular() const { return (this->m_frontMaterialSpecular); }
		void setFrontMaterialSpecular(const QColor &value) { this->m_frontMaterialSpecular = value; }
		qreal frontMaterialShininess() const { return (this->m_frontMaterialShininess); }
		void setFrontMaterialShininess(qreal value) { this->m_frontMaterialShininess = value; }
		const QColor &backMaterialAmbient() const { return (this->m_backMaterialAmbient); }
		void setBackMaterialAmbient(const QColor &value) { this->m_backMaterialAmbient = value; }
		const QColor &backMaterialDiffuse() const { return (this->m_backMaterialDiffuse); }
		void setBackMaterialDiffuse(const QColor &value) { this->m_backMaterialDiffuse = value; }
		const QColor &backMaterialSpecular() const { return (this->m_backMaterialSpecular); }
		void setBackMaterialSpecular(const QColor &value) { this->m_backMaterialSpecular = value; }
		qreal backMaterialShininess() const { return (this->m_backMaterialShininess); }
		void setBackMaterialShininess(qreal value) { this->m_backMaterialShininess = value; }
		const QColor &effectMaterialAmbient() const { return (this->m_effectMaterialAmbient); }
		void setEffectMaterialAmbient(const QColor &value) { this->m_effectMaterialAmbient = value; }
		const QColor &effectMaterialDiffuse() const { return (this->m_effectMaterialDiffuse); }
		void setEffectMaterialDiffuse(const QColor &value) { this->m_effectMaterialDiffuse = value; }
		const QColor &effectMaterialSpecular() const { return (this->m_effectMaterialSpecular); }
		void setEffectMaterialSpecular(const QColor &value) { this->m_effectMaterialSpecular = value; }
		qreal effectMaterialShininess() const { return (this->m_effectMaterialShininess); }
		void setEffectMaterialShininess(qreal value) { this->m_effectMaterialShininess = value; }

	public:
		virtual void deserialize(QDataStream &stream);
		virtual void serialize(QDataStream &stream) const;
	};

	class SpriteConfiguration : public ConfigurationBase {

	private:
		QString							m_largeFontName;
		int								m_largeFontSize;
		QColor							m_largeFontColor;
		QString							m_mediumFontName;
		int								m_mediumFontSize;
		QColor							m_mediumFontColor;
		QString							m_smallFontName;
		int								m_smallFontSize;
		QColor							m_smallFontColor;

	public:
		SpriteConfiguration();

	public:
		const QString &largeFontName() const { return (this->m_largeFontName); }
		void setLargeFontName(const QString &value) { this->m_largeFontName = value; }
		int largeFontSize() const { return (this->m_largeFontSize); }
		void setLargeFontSize(int value) { this->m_largeFontSize = value; }
		const QColor &largeFontColor() const { return (this->m_largeFontColor); }
		void setLargeFontColor(const QColor &value) { this->m_largeFontColor = value; }
		const QString &mediumFontName() const { return (this->m_mediumFontName); }
		void setMediumFontName(const QString &value) { this->m_mediumFontName = value; }
		int mediumFontSize() const { return (this->m_mediumFontSize); }
		void setMediumFontSize(int value) { this->m_mediumFontSize = value; }
		const QColor &mediumFontColor() const { return (this->m_mediumFontColor); }
		void setMediumFontColor(const QColor &value) { this->m_mediumFontColor = value; }
		const QString &smallFontName() const { return (this->m_smallFontName); }
		void setSmallFontName(const QString &value) { this->m_smallFontName = value; }
		int smallFontSize() const { return (this->m_smallFontSize); }
		void setSmallFontSize(int value) { this->m_smallFontSize = value; }
		const QColor &smallFontColor() const { return (this->m_smallFontColor); }
		void setSmallFontColor(const QColor &value) { this->m_smallFontColor = value; }

	public:
		virtual void deserialize(QDataStream &stream);
		virtual void serialize(QDataStream &stream) const;
	};

private:
	ApplicationConfiguration			m_applicationConfiguration;
	SceneConfiguration					m_sceneConfiguration;
	SpriteConfiguration					m_spriteConfiguration;

public:
	static Configuration &instance();

private:
	Configuration();
	Configuration(const Configuration &obj) { }
	~Configuration();

public:
	ApplicationConfiguration &applicationConfiguration() { return (this->m_applicationConfiguration); }
	const ApplicationConfiguration &applicationConfiguration() const { return (this->m_applicationConfiguration); }
	SceneConfiguration &sceneConfiguration() { return (this->m_sceneConfiguration); }
	const SceneConfiguration &sceneConfiguration() const { return (this->m_sceneConfiguration); }
	SpriteConfiguration &spriteConfiguration() { return (this->m_spriteConfiguration); }
	const SpriteConfiguration &spriteConfiguration() const { return (this->m_spriteConfiguration); }
};



class Session {

private:
	QMap<QString, QVariant>				m_variables;

public:
	static Session &instance();

public:
	Session() { }

public:
	QVariant &operator [](const QString &key) { return (this->m_variables[key]); }
	const QVariant operator [](const QString &key) const { return (this->m_variables[key]); }

public:
	void clear() { this->m_variables.clear(); }
	bool add(const QString &key, const QVariant &value) {
		if (this->m_variables.contains(key)) return (false);
		this->m_variables.insert(key, value);
		return (true);
	}
	bool remove(const QString &key) {
		if (!this->m_variables.contains(key)) return (false);
		this->m_variables.remove(key);
		return (true);
	}
	bool contains(const QString &key) {
		return (this->m_variables.contains(key));
	}
};



#endif /* _VGR_CORE_BASIC_CONFIGURATION_HPP */
