#include "QVGRManager.h"
#include <common/algo/util.hpp>
#include <common/util/logger.hpp>
#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QFileInfo>
#include <QtXml/QXmlStreamReader>
#include <QtXml/QXmlStreamWriter>

#pragma warning (push)
#pragma warning (disable:4482)	/* nonstandard extension used: enum 'enum' used in qualified name */



void QVGRManager::LoadThread::run() {

	_VGR_SYSTEM_MSG("Loading started.");
	_VGR_LOGGER_BEG_PROG;
	_VGR_LOGGER_UPD_PROG(0, this->m_paths.size());
	for (size_t i = 0; i < this->m_paths.size(); ++i) {
		this->m_manager->m_items.push_back(new Model(this->m_paths[i]));
		_VGR_LOGGER_UPD_PROG(i + 1, this->m_paths.size());
	}
	_VGR_LOGGER_END_PROG;
	_VGR_SYSTEM_MSG("Loading finished.");
	
	_VGR_SYSTEM_MSG("Processing started.");
	_VGR_LOGGER_BEG_PROG;
	_VGR_LOGGER_UPD_PROG(0, this->m_paths.size());
	for (size_t i = 0; i < this->m_paths.size(); ++i) {
		this->m_manager->m_items[i]->preprocess();
		_VGR_LOGGER_UPD_PROG(i + 1, this->m_paths.size());
	}
	_VGR_LOGGER_END_PROG;
	_VGR_SYSTEM_MSG("Processing finished.");
}



QVGRManager::QVGRManager(QObject *parent) {

	this->m_items = std::vector<Model *>();
	this->m_loadThread = new LoadThread(this);

	connect(this->m_loadThread, SIGNAL(finished()), this, SIGNAL(loadingFinished()));
}

void QVGRManager::clear() {
	for (size_t i = 0; i < this->m_items.size(); ++i) {
		this->m_items[i]->dispose();
	}
	this->m_items.clear();
}

void QVGRManager::addItem(QString path) {
	emit loadingStarted();
	this->m_loadThread->setPaths(QStringList() << path);
	this->m_loadThread->start();
}

void QVGRManager::addItems(QStringList paths) {
	emit loadingStarted();
	this->m_loadThread->setPaths(paths);
	this->m_loadThread->start();
}

void QVGRManager::loadProject(QString path) {
	_VGR_SYSTEM_MSG(boost::format("Loading project from file <%1%>.") % path.toStdString());
	QStringList paths;
	QFileInfo fileInfo(path);
	if (!fileInfo.exists()) {
		_VGR_SYSTEM_ERR(boost::format("Loading failed, file <%1%> does not exist.") % path.toStdString());
		return;
	}
	QDir dir = fileInfo.absoluteDir();
	QFile *file = new QFile(fileInfo.absoluteFilePath());
	file->open(QIODevice::OpenModeFlag::ReadOnly | QIODevice::OpenModeFlag::Text);
	QXmlStreamReader xml(file);
	xml.readNextStartElement();
	while (xml.readNextStartElement()) {
		paths.append(dir.filePath(xml.readElementText()));
	}
	file->close();
	_VGR_SYSTEM_MSG(boost::format("Loading project <%1%> finished.") % path.toStdString());
	this->addItems(paths);
}

void QVGRManager::saveProject(QString path) const {
	_VGR_SYSTEM_MSG(boost::format("Saving project to file <%1%>.") % path.toStdString());
	QFileInfo fileInfo(path);
	QDir dir = fileInfo.absoluteDir();
	QFile *file = new QFile(fileInfo.absoluteFilePath());
	file->open(QIODevice::OpenModeFlag::WriteOnly | QIODevice::OpenModeFlag::Text);
	QXmlStreamWriter xml(file);
	xml.setAutoFormatting(true);
	xml.writeStartDocument();
	xml.writeStartElement(QString("project"));
	for (size_t i = 0; i < this->m_items.size(); ++i) {
		xml.writeTextElement(QString("model"), dir.relativeFilePath(this->m_items[i]->path()));
	}
	xml.writeEndElement();
	xml.writeEndDocument();
	file->close();
	_VGR_SYSTEM_MSG(boost::format("Saving project <%1%> finished.") % path.toStdString());
}



#pragma warning (pop)
