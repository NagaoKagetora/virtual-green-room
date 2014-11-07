#ifndef _VGR_GUI_MODULE_QVGRREFLECTIONMODULE_H
#define _VGR_GUI_MODULE_QVGRREFLECTIONMODULE_H

#include <gui/module/QVGRModule.h>
#include <core/Reflection.hpp>



class QAction;
class QComboBox;
class QDial;
class QLabel;
class QSlider;
class QVGRReflectionModule;



class QVGRReflectionModule : public QVGRModule {

	Q_OBJECT

private:
	ReflectionFunction					m_function;
	QAction								*m_activateAction;
	QLabel								*m_modeLabel;
	QComboBox							*m_modeComboBox;
	QLabel								*m_scaleLabel;
	QSlider								*m_scaleSlider;
	QDial								*m_scaleDial;
	QLabel								*m_rotationLabel;
	QSlider								*m_rotationSlider;
	QDial								*m_rotationDial;

public:
	QVGRReflectionModule(const QString &title, QWidget *parent = 0);

public:
	virtual QString name() const { return (QString("Reflection")); }
	virtual Function &getFunction() { return (this->m_function); }
	virtual const Function &getFunction() const { return (this->m_function); }
	virtual const Settings &getSettings() const;

private slots:
	void updateWidget();

protected slots:
	virtual void updateLayout();
};



#endif /* _VGR_GUI_MODULE_QVGRREFLECTIONMODULE_H */
