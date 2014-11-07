#ifndef _VGR_GUI_QVGRLOGGERWIDGET_H
#define _VGR_GUI_QVGRLOGGERWIDGET_H

#include <common/global.hpp>
#include <common/util/logger.hpp>
#include <QtCore/QMutex>
#include <QtCore/QThread>
#include <QtGui/QDialog>
#include <QtGui/QProgressBar>
#include <QtGui/QStandardItemModel>
#include <QtGui/QWidget>

using namespace vgr;



class QComboBox;
class QLabel;
class QPushButton;
class QSortFilterProxyModel;
class QTreeView;
class QVGRLogSignal;
class QVGRLogManager;
class QVGRLoggerWidget;



class QVGRLogSignal : public QObject {

	Q_OBJECT

public:
	void beginLogging(int index) { emit loggingStarted(index); }
	void endLogging(int index) { emit loggingStopped(index); }
	void beginProgress(int thread) { emit progressStarted(thread); }
	void endProgress(int thread) { emit progressStopped(thread); }
	void updateProgress(int thread, real_t progress) { emit progressChanged(thread, progress); }
	void busyProgress(int thread) { emit progressChanged(thread, -1); }

signals:
	void loggingStarted(int index);
	void loggingStopped(int index);
	void progressStarted(int thread);
	void progressStopped(int thread);
	void progressChanged(int thread, qreal progress);
};



class QVGRLogManager : public util::LogManager {

private:
	QStandardItemModel					*m_itemModel;
	QVGRLogSignal						*m_logSignal;
	QMutex								m_loggingMutex;
	QVector<QMutex *>					m_progressMutexs;

public:
	QVGRLogManager(QStandardItemModel *itemModel, QVGRLogSignal *logSignal) :
			m_itemModel(itemModel), m_logSignal(logSignal),
			m_loggingMutex(), m_progressMutexs(QThread::idealThreadCount()) {
		for (int i = 0; i < this->m_progressMutexs.size(); ++i) {
			this->m_progressMutexs[i] = new QMutex();
		}
	}
	~QVGRLogManager() {
		for (int i = 0; i < this->m_progressMutexs.size(); ++i) {
			delete this->m_progressMutexs[i];
		}
	}

public:
	QMutex &LoggingMutex() { return (this->m_loggingMutex); }
	const QMutex &LoggingMutex() const { return (this->m_loggingMutex); }

public:
	virtual void append(const Item &item);
	virtual void beginProgress() {
		while (this->m_progressMutexs[0]->tryLock());
		this->m_logSignal->beginProgress(0);
	}
	virtual void beginProgress(int thread) {
		while (this->m_progressMutexs[thread]->tryLock());
		this->m_logSignal->beginProgress(thread);
	}
	virtual void endProgress() {
		this->m_logSignal->endProgress(0);
		this->m_progressMutexs[0]->unlock();
	}
	virtual void endProgress(int thread) {
		this->m_logSignal->endProgress(thread);
		this->m_progressMutexs[thread]->unlock();
	}
	virtual void updateProgress(int completed, int total) {
		this->m_logSignal->updateProgress(0, qreal(completed) / qreal(total));
	}
	virtual void updateProgress(int thread, int completed, int total) {
		this->m_logSignal->updateProgress(thread, qreal(completed) / qreal(total));
	}
	virtual void busyProgress() {
		this->m_logSignal->busyProgress(0);
	}
	virtual void busyProgress(int thread) {
		this->m_logSignal->busyProgress(thread);
	}
};



class QVGRLoggerAnimationThread : public QThread {

	Q_OBJECT

private:
	int								m_interval;
	int								m_loopsCount;

public:
	QVGRLoggerAnimationThread() { }

public:
	void setInterval(int value) { this->m_interval = value; }
	void setLoopsCount(int value) { this->m_loopsCount = value; }

protected:
	virtual void run();

signals:
	void blink(bool on);
};



class QVGRLoggerWidget : public QDialog {

	Q_OBJECT

private:
	QComboBox							*m_filterComboBox;
	QPushButton							*m_clearPushButton;
	QPushButton							*m_copyPushButton;
	QVGRLogSignal						*m_logSignal;
	QProgressBar						*m_progressBar;
	QStandardItemModel					*m_itemModel;
	QSortFilterProxyModel				*m_proxyModel;
	QTreeView							*m_treeView;
	QLabel								*m_statusIconLabel;
	QLabel								*m_statusTextLabel;
	QWidget								*m_statusWidget;
	QVector<QProgressBar *>				m_statusProgressBars;
	QWidget								*m_statusPermanentWidget;
	QVGRLoggerAnimationThread			*m_animationThread;
	QWidget								*m_hostWidget;
	QVGRLogManager						*m_logManager;
	QString								m_hostWidgetStyleSheet;

public:
	QVGRLoggerWidget(QWidget *parent);
	virtual ~QVGRLoggerWidget() { }

public:
	QWidget *statusWidget() const { return (this->m_statusWidget); }
	QWidget *statusPermanentWidget() const { return (this->m_statusPermanentWidget); }
	QWidget *hostWidget() const { return (this->m_hostWidget); }
	void setHostWidget(QWidget *value) { this->m_hostWidget = value; }

public:
	void blink();

private slots:
	void onFilterComboBoxCurrentIndexChanged(int index);
	void onClearPushButtonClicked();
	void onCopyPushButtonClicked();
	void onLogSignalLoggingStarted(int index);
	void onLogSignalLoggingStopped(int index);
	void onLogSignalProgressStarted(int thread);
	void onLogSignalProgressStopped(int thread);
	void onLogSignalProgressChanged(int thread, qreal progress);
	void onItemModelRowsInserted(const QModelIndex &index, int start, int end);
	void onAnimationBlink(bool on);
};



#endif /* _VGR_GUI_QVGRLOGGERWIDGET_H */
