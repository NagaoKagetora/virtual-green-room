#include "Configuration.hpp"
#include <QtCore/QDir>
#include <QtCore/QDataStream>
#include <QtCore/QFile>

#pragma warning (push)
#pragma warning (disable:4482)	/* nonstandard extension used: enum 'enum' used in qualified name */



Configuration::ApplicationConfiguration::ApplicationConfiguration() {
	this->m_workspacePath = QString(".");
	this->m_recentDirectory = QString(".");
	this->m_usingRecentDirectory = true;
	this->m_usingAutoRepairVertexConnectivities = true;
	this->m_usingAutoRepairFaceConnectivities = true;
}

void Configuration::ApplicationConfiguration::deserialize(QDataStream &stream) {
	stream >> this->m_recentDirectory;
	stream >> this->m_recentDirectory;
	stream >> this->m_usingRecentDirectory;
	stream >> this->m_usingAutoRepairVertexConnectivities;
	stream >> this->m_usingAutoRepairFaceConnectivities;
}

void Configuration::ApplicationConfiguration::serialize(QDataStream &stream) const {
	stream << this->m_recentDirectory;
	stream << this->m_recentDirectory;
	stream << this->m_usingRecentDirectory;
	stream << this->m_usingAutoRepairVertexConnectivities;
	stream << this->m_usingAutoRepairFaceConnectivities;
}



Configuration::SceneConfiguration::SceneConfiguration() {
	this->m_sceneBackgroundColor = QColor::fromRgbF(0.0f, 0.0f, 0.0f);
	this->m_pointSize = 3.0f;
	this->m_pointColor = QColor::fromRgbF(0.1f, 0.1f, 0.1f);
	this->m_lineWidth = 1.2f;
	this->m_lineColor = QColor::fromRgbF(0.1f, 0.1f, 0.1f);
	this->m_lightIntensity = QColor::fromRgbF(1.0f, 1.0f, 1.0f);
	this->m_frontMaterialAmbient = QColor::fromRgbF(0.25f, 0.25f, 0.25f);
	this->m_frontMaterialDiffuse = QColor::fromRgbF(0.25f, 0.25f, 0.25f);
	this->m_frontMaterialSpecular = QColor::fromRgbF(0.1f, 0.1f, 0.1f);
	this->m_frontMaterialShininess = 128.0f;
	this->m_backMaterialAmbient = QColor::fromRgbF(0.25f, 0.0f, 0.0f);
	this->m_backMaterialDiffuse = QColor::fromRgbF(0.25f, 0.25f, 0.25f);
	this->m_backMaterialSpecular = QColor::fromRgbF(0.1f, 0.1f, 0.1f);
	this->m_backMaterialShininess = 128.0f;
	this->m_effectMaterialAmbient = QColor::fromRgbF(0.5f, 0.5f, 0.5f);
	this->m_effectMaterialDiffuse = QColor::fromRgbF(0.5f, 0.5f, 0.5f);
	this->m_effectMaterialSpecular = QColor::fromRgbF(0.1f, 0.1f, 0.1f);
	this->m_effectMaterialShininess = 128.0f;
}

void Configuration::SceneConfiguration::deserialize(QDataStream &stream) {
	stream >> this->m_sceneBackgroundColor;
	stream >> this->m_pointSize;
	stream >> this->m_pointColor;
	stream >> this->m_lineWidth;
	stream >> this->m_lineColor;
	stream >> this->m_lightIntensity;
	stream >> this->m_frontMaterialAmbient;
	stream >> this->m_frontMaterialDiffuse;
	stream >> this->m_frontMaterialSpecular;
	stream >> this->m_frontMaterialShininess;
	stream >> this->m_backMaterialAmbient;
	stream >> this->m_backMaterialDiffuse;
	stream >> this->m_backMaterialSpecular;
	stream >> this->m_backMaterialShininess;
	stream >> this->m_effectMaterialAmbient;
	stream >> this->m_effectMaterialDiffuse;
	stream >> this->m_effectMaterialSpecular;
	stream >> this->m_effectMaterialShininess;
}

void Configuration::SceneConfiguration::serialize(QDataStream &stream) const {
	stream << this->m_sceneBackgroundColor;
	stream << this->m_pointSize;
	stream << this->m_pointColor;
	stream << this->m_lineWidth;
	stream << this->m_lineColor;
	stream << this->m_lightIntensity;
	stream << this->m_frontMaterialAmbient;
	stream << this->m_frontMaterialDiffuse;
	stream << this->m_frontMaterialSpecular;
	stream << this->m_frontMaterialShininess;
	stream << this->m_backMaterialAmbient;
	stream << this->m_backMaterialDiffuse;
	stream << this->m_backMaterialSpecular;
	stream << this->m_backMaterialShininess;
	stream << this->m_effectMaterialAmbient;
	stream << this->m_effectMaterialDiffuse;
	stream << this->m_effectMaterialSpecular;
	stream << this->m_effectMaterialShininess;
}



Configuration::SpriteConfiguration::SpriteConfiguration() {
	this->m_largeFontName = QString("arial.ttf");;
	this->m_largeFontSize = 36;
	this->m_largeFontColor = QColor::fromRgbF(1.0f, 1.0f, 1.0f, 0.5f);
	this->m_mediumFontName = QString("arial.ttf");;
	this->m_mediumFontSize = 24;
	this->m_mediumFontColor = QColor::fromRgbF(1.0f, 1.0f, 1.0f, 0.5f);;
	this->m_smallFontName = QString("arial.ttf");;
	this->m_smallFontSize = 12;
	this->m_smallFontColor = QColor::fromRgbF(1.0f, 1.0f, 1.0f, 0.5f);;
}

void Configuration::SpriteConfiguration::deserialize(QDataStream &stream) {
	stream >> this->m_largeFontName;
	stream >> this->m_largeFontSize;
	stream >> this->m_largeFontColor;
	stream >> this->m_mediumFontName;
	stream >> this->m_mediumFontSize;
	stream >> this->m_mediumFontColor;
	stream >> this->m_smallFontName;
	stream >> this->m_smallFontSize;
	stream >> this->m_smallFontColor;
}

void Configuration::SpriteConfiguration::serialize(QDataStream &stream) const {
	stream << this->m_largeFontName;
	stream << this->m_largeFontSize;
	stream << this->m_largeFontColor;
	stream << this->m_mediumFontName;
	stream << this->m_mediumFontSize;
	stream << this->m_mediumFontColor;
	stream << this->m_smallFontName;
	stream << this->m_smallFontSize;
	stream << this->m_smallFontColor;
}



Configuration &Configuration::instance() {
	static Configuration Instance;
	return (Instance);
}

Configuration::Configuration() {
	this->m_applicationConfiguration = ApplicationConfiguration();
	this->m_sceneConfiguration = SceneConfiguration();
	this->m_spriteConfiguration = SpriteConfiguration();
	QFile file(QDir::homePath() + QString("/vgr-settings-210.dat"));
	if (file.open(QIODevice::ReadOnly)) {
		QDataStream stream(&file);
		this->m_applicationConfiguration.deserialize(stream);
		this->m_sceneConfiguration.deserialize(stream);
		this->m_spriteConfiguration.deserialize(stream);
		file.close();
	}
}

Configuration::~Configuration() {
	QFile file(QDir::homePath() + QString("/vgr-settings-210.dat"));
	if (file.open(QIODevice::WriteOnly)) {
		QDataStream stream(&file);
		this->m_applicationConfiguration.serialize(stream);
		this->m_sceneConfiguration.serialize(stream);
		this->m_spriteConfiguration.serialize(stream);
		file.close();
	}
}



Session &Session::instance() {
	static Session Instance;
	return (Instance);
}



#pragma warning (pop)
