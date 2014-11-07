#ifndef _VGR_GUI_QVGRMANAGER_H
#define _VGR_GUI_QVGRMANAGER_H

#include <common/global.hpp>
#include <common/3rdparty/vcglib.hpp>
#include <QtCore/QMutex>
#include <QtCore/QThread>
#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <core/basic/Model.hpp>

using namespace vgr;



class QVGRManager;



class QVGRManager : public QObject {

	Q_OBJECT

private:
	class LoadThread : public QThread {

	private:
		QVGRManager						*m_manager;
		QStringList						m_paths;

	public:
		LoadThread(QVGRManager *manager) : QThread(), m_manager(manager), m_paths() { }

	public:
		void setManager(QVGRManager *manager) { this->m_manager = manager; }
		void setPaths(const QStringList &paths) { this->m_paths = paths; }

	protected:
		virtual void run();
	};

private:
	std::vector<Model *>				m_items;
	LoadThread							*m_loadThread;

public:
	QVGRManager(QObject *parent = 0);
	~QVGRManager() { this->clear(); }

public:
	size_t size() const { return (this->m_items.size()); }
	const Model *item(size_t index) const { return (this->m_items[index]); }
	Model *item(size_t index) { return (this->m_items[index]); }
	void clear();
	void addItem(QString path);
	void addItems(QStringList paths);
	void loadProject(QString path);
	void saveProject(QString path) const;

signals:
	void loadingStarted();
	void loadingFinished();
};



#endif /* _VGR_GUI_QVGRMANAGER_H */
