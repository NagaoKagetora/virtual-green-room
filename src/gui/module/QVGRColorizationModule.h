#ifndef _VGR_GUI_MODULE_QVGRCOLORIZATIONMODULE_H
#define _VGR_GUI_MODULE_QVGRCOLORIZATIONMODULE_H

#include <gui/module/QVGRModule.h>
#include <core/Colorization.hpp>



class QAction;
class QComboBox;
class QDoubleSpinBox;
class QLabel;
class QLineEdit;
class QVGRColorizationModule;



class QVGRColorizationModule : public QVGRModule {

	Q_OBJECT

private:
	ColorizationFunction				m_function;
	QAction								*m_activateAction;
	QLabel								*m_modeLabel;
	QComboBox							*m_modeComboBox;
	QLabel								*m_sourceLabel;
	QComboBox							*m_sourceComboBox;
	QLabel								*m_targetLabel;
	QComboBox							*m_targetComboBox;
	QLabel								*m_resolutionLabel;
	QDoubleSpinBox						*m_resolutionDoubleSpinBox;
	QLabel								*m_rangeLabel;
	QLineEdit							*m_rangeLineEdit;
	QLabel								*m_toleranceLabel;
	QLineEdit							*m_toleranceLineEdit;
	QAction								*m_startAction;

public:
	QVGRColorizationModule(const QString &title, QWidget *parent = 0);

public:
	virtual QString name() const { return (QString("Colorization")); }
	virtual Function &getFunction() { return (this->m_function); }
	virtual const Function &getFunction() const { return (this->m_function); }
	virtual const Settings &getSettings() const;
	virtual void applyResults(const Results &results);

private slots:
	void updateWidget();
};



#endif /* _VGR_GUI_MODULE_QVGRCOLORIZATIONMODULE_H */
