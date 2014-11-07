#ifndef _VGR_GUI_MODULE_QVGRMODULE_H
#define _VGR_GUI_MODULE_QVGRMODULE_H

#include <QtCore/QVector>
#include <QtGui/QToolBar>



class Function;
class Settings;
class Results;
class QComboBox;
class QVGRModule;
class QVGRModuleGroup;



class QVGRModule : public QToolBar {

	Q_OBJECT

public:
	QVGRModule(const QString &title, QWidget *parent = 0);

protected:
	void makeModelSelection(QComboBox *modelComboBox);
	QAction *addControllerWidget(QWidget *indicator, QWidget *controller);
	QAction *addControllerWidget(QWidget *indicator, QWidget *horizontalController, QWidget *verticalController);

public:
	virtual QString name() const = 0;
	virtual Function &getFunction() = 0;
	virtual const Function &getFunction() const = 0;
	virtual const Settings &getSettings() const = 0;
	virtual void applyResults(const Results &results) { }

protected slots:
	void activate() { emit activated(this->name()); }
	void modify() { emit modified(this->name()); }
	void update() { emit updated(this->name()); }
	void execute() { emit executionStarted(this->name()); }

protected slots:
	virtual void updateLayout() { }

signals:
	void activated(QString name);
	void modified(QString name);
	void updated(QString name);
	void executionStarted(QString name);
};



class QVGRModuleGroup : public QVector<QVGRModule *> {

public:
	QVGRModuleGroup() : QVector() { }

public:
	void setEnabled(bool enabled);
	bool activated() const;
};



#endif /* _VGR_GUI_MODULE_QVGRMODULE_H */
