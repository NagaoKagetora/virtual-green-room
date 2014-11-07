#ifndef _VGR_GUI_MODULE_QVGRSECTIONMODULE_H
#define _VGR_GUI_MODULE_QVGRSECTIONMODULE_H

#include <gui/module/QVGRModule.h>
#include <core/Section.hpp>



class QAction;
class QCheckBox;
class QComboBox;
class QLabel;
class QLineEdit;
class QVGRSectionModule;



class QVGRSectionModule : public QVGRModule {

	Q_OBJECT

private:
	SectionFunction						m_function;
	QAction								*m_activateAction;
	QLabel								*m_modeLabel;
	QComboBox							*m_modeComboBox;
	QLabel								*m_sourceLabel;
	QComboBox							*m_sourceComboBox;
	QLabel								*m_targetLabel;
	QComboBox							*m_targetComboBox;
	QLabel								*m_cullingLabel;
	QCheckBox							*m_cullingCheckBox;
	QLabel								*m_acceptanceLabel;
	QLineEdit							*m_acceptanceLineEdit;
	QLabel								*m_rejectionLabel;
	QLineEdit							*m_rejectionLineEdit;

public:
	QVGRSectionModule(const QString &title, QWidget *parent = 0);

public:
	virtual QString name() const { return (QString("Section")); }
	virtual Function &getFunction() { return (this->m_function); }
	virtual const Function &getFunction() const { return (this->m_function); }
	virtual const Settings &getSettings() const;

private slots:
	void updateWidget();
};



#endif /* _VGR_GUI_MODULE_QVGRSECTIONMODULE_H */
