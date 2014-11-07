#include "QVGRSectionModule.h"
#include <QtGui/QAction>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QLabel>
#include <QtGui/QLayout>
#include <QtGUi/QLineEdit>

#pragma warning (push)
#pragma warning (disable:4482)	/* nonstandard extension used: enum 'enum' used in qualified name */



QVGRSectionModule::QVGRSectionModule(const QString &title, QWidget *parent) : QVGRModule(title, parent) {

	// Widgets.
	this->m_activateAction = new QAction(QIcon(QString(":/resource/toolbar2/section_activate.png")), tr("Activate"), this);
	this->m_activateAction->setCheckable(true);
	this->m_activateAction->setToolTip(tr("Section"));
	this->m_modeLabel = new QLabel(tr("Mode"));
	this->m_modeLabel->setPixmap(QPixmap(QString(":/resource/toolbar2/section_mode.png")).scaled(this->iconSize(), Qt::AspectRatioMode::IgnoreAspectRatio, Qt::TransformationMode::SmoothTransformation));
	this->m_modeComboBox = new QComboBox();
	this->m_modeComboBox->setFixedWidth(100);
	this->m_modeComboBox->addItem(tr("Profile (Raw)"));
	this->m_modeComboBox->addItem(tr("Profile (LE)"));
	this->m_modeComboBox->addItem(tr("Slope (LE)"));
	this->m_modeComboBox->addItem(tr("Curvature (LE)"));
	this->m_modeComboBox->addItem(tr("Profile (LO)"));
	this->m_modeComboBox->addItem(tr("Slope (LO)"));
	this->m_modeComboBox->addItem(tr("Curvature (LO)"));
	//this->m_modeComboBox->addItem(tr("Profile (GE)"));
	//this->m_modeComboBox->addItem(tr("Slope (GE)"));
	//this->m_modeComboBox->addItem(tr("Curvature (GE)"));
	//this->m_modeComboBox->addItem(tr("Profile (SF)"));
	//this->m_modeComboBox->addItem(tr("Slope (SF)"));
	//this->m_modeComboBox->addItem(tr("Curvature (SF)"));
	//this->m_modeComboBox->addItem(tr("Profile (SP)"));
	//this->m_modeComboBox->addItem(tr("Slope (SP)"));
	//this->m_modeComboBox->addItem(tr("Curvature (SP)"));
	//this->m_modeComboBox->addItem(tr("Profile (SS)"));
	//this->m_modeComboBox->addItem(tr("Slope (SS)"));
	//this->m_modeComboBox->addItem(tr("Curvature (SS)"));
	this->m_modeComboBox->setEnabled(false);
	this->m_sourceLabel = new QLabel(tr("Model"));
	this->m_sourceLabel->setPixmap(QPixmap(QString(":/resource/toolbar2/section_source.png")).scaled(this->iconSize(), Qt::AspectRatioMode::IgnoreAspectRatio, Qt::TransformationMode::SmoothTransformation));
	this->m_sourceComboBox = new QComboBox();
	this->m_sourceComboBox->setFixedWidth(100);
	this->m_sourceComboBox->setEnabled(false);
	this->makeModelSelection(this->m_sourceComboBox);
	this->m_targetLabel = new QLabel(tr("Target"));
	this->m_targetLabel->setPixmap(QPixmap(QString(":/resource/toolbar2/section_target.png")).scaled(this->iconSize(), Qt::AspectRatioMode::IgnoreAspectRatio, Qt::TransformationMode::SmoothTransformation));
	this->m_targetComboBox = new QComboBox();
	this->m_targetComboBox->setFixedWidth(100);
	this->m_targetComboBox->setEnabled(false);
	this->makeModelSelection(this->m_targetComboBox);
	this->m_cullingLabel = new QLabel(tr("Culling"));
	this->m_cullingLabel->setPixmap(QPixmap(QString(":/resource/toolbar2/section_culling.png")).scaled(this->iconSize(), Qt::AspectRatioMode::IgnoreAspectRatio, Qt::TransformationMode::SmoothTransformation));
	this->m_cullingCheckBox = new QCheckBox(tr("Visible Only"));
	this->m_cullingCheckBox->setEnabled(false);
	this->m_acceptanceLabel = new QLabel(tr("Acceptance"));
	this->m_acceptanceLabel->setPixmap(QPixmap(QString(":/resource/toolbar2/section_acceptance.png")).scaled(this->iconSize(), Qt::AspectRatioMode::IgnoreAspectRatio, Qt::TransformationMode::SmoothTransformation));
	this->m_acceptanceLineEdit = new QLineEdit();
	this->m_acceptanceLineEdit->setFixedWidth(100);
	this->m_acceptanceLineEdit->setText(QString("0"));
	this->m_acceptanceLineEdit->setEnabled(false);
	this->m_rejectionLabel = new QLabel(tr("Rejection"));
	this->m_rejectionLabel->setPixmap(QPixmap(QString(":/resource/toolbar2/section_rejection.png")).scaled(this->iconSize(), Qt::AspectRatioMode::IgnoreAspectRatio, Qt::TransformationMode::SmoothTransformation));
	this->m_rejectionLineEdit = new QLineEdit();
	this->m_rejectionLineEdit->setFixedWidth(100);
	this->m_rejectionLineEdit->setText(QString("0"));
	this->m_rejectionLineEdit->setEnabled(false);

	// Layouts.
	this->addAction(this->m_activateAction);
	this->addSeparator();
	this->addControllerWidget(this->m_modeLabel, this->m_modeComboBox);
	this->addSeparator();
	this->addControllerWidget(this->m_sourceLabel, this->m_sourceComboBox);
	this->addControllerWidget(this->m_targetLabel, this->m_targetComboBox);
	this->addSeparator();
	this->addControllerWidget(this->m_cullingLabel, this->m_cullingCheckBox);
	this->addSeparator();
	this->addControllerWidget(this->m_acceptanceLabel, this->m_acceptanceLineEdit);
	this->addControllerWidget(this->m_rejectionLabel, this->m_rejectionLineEdit);

	// Signals and slots.
	connect(this->m_activateAction, SIGNAL(toggled(bool)), this, SLOT(updateWidget()));
	connect(this->m_activateAction, SIGNAL(toggled(bool)), this, SLOT(activate()));
	connect(this->m_modeComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(modify()));
	connect(this->m_sourceComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(modify()));
	connect(this->m_targetComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(modify()));
	connect(this->m_cullingCheckBox, SIGNAL(clicked(bool)), this, SLOT(update()));
	connect(this->m_acceptanceLineEdit, SIGNAL(textChanged(const QString &)), this, SLOT(update()));
	connect(this->m_rejectionLineEdit, SIGNAL(textChanged(const QString &)), this, SLOT(update()));
}

const Settings &QVGRSectionModule::getSettings() const {
	static SectionSettings settings;
	settings.activated = this->m_activateAction->isChecked();
	settings.mode = this->m_modeComboBox->currentIndex();
	settings.source = this->m_sourceComboBox->currentIndex();
	settings.target = this->m_targetComboBox->currentIndex();
	settings.culling = this->m_cullingCheckBox->isChecked();
	settings.acceptance = this->m_acceptanceLineEdit->text().toDouble();
	settings.rejection = this->m_rejectionLineEdit->text().toDouble();
	return (settings);
}

void QVGRSectionModule::updateWidget() {
	bool enabled = this->m_activateAction->isChecked();
	this->m_modeComboBox->setEnabled(enabled);
	this->m_sourceComboBox->setEnabled(enabled);
	this->m_targetComboBox->setEnabled(enabled);
	this->m_cullingCheckBox->setEnabled(enabled);
	this->m_acceptanceLineEdit->setEnabled(enabled);
	this->m_rejectionLineEdit->setEnabled(enabled);
}



#pragma warning (pop)
