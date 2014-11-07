#ifndef _VGR_GUI_MODULE_QVGRREGISTRATIONMODULE_H
#define _VGR_GUI_MODULE_QVGRREGISTRATIONMODULE_H

#include <gui/module/QVGRModule.h>
#include <core/Registration.hpp>



class QAction;
class QComboBox;
class QLabel;
class QVGRRegistrationModule;



class QVGRRegistrationModule : public QVGRModule {

	Q_OBJECT

private:
	RegistrationFunction				m_function;
	QAction								*m_activateAction;
	QLabel								*m_modeLabel;
	QComboBox							*m_modeComboBox;
	QLabel								*m_sourceLabel;
	QComboBox							*m_sourceComboBox;
	QLabel								*m_targetLabel;
	QComboBox							*m_targetComboBox;
	QAction								*m_startAction;

public:
	QVGRRegistrationModule(const QString &title, QWidget *parent = 0);

public:
	virtual QString name() const { return (QString("Registration")); }
	virtual Function &getFunction() { return (this->m_function); }
	virtual const Function &getFunction() const { return (this->m_function); }
	virtual const Settings &getSettings() const;

private slots:
	void updateWidget();
};



#endif /* _VGR_GUI_MODULE_QVGRREGISTRATIONMODULE_H */
