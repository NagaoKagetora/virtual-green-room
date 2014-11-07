#include "QVGRRegistrationModule.h"
#include <QtGui/QAction>
#include <QtGui/QComboBox>
#include <QtGui/QLabel>
#include <QtGui/QLayout>

#pragma warning (push)
#pragma warning (disable:4482)	/* nonstandard extension used: enum 'enum' used in qualified name */



QVGRRegistrationModule::QVGRRegistrationModule(const QString &title, QWidget *parent) : QVGRModule(title, parent) {

	// Widgets.
	this->m_activateAction = new QAction(QIcon(QString(":/resource/toolbar2/registration_activate.png")), tr("Activate"), this);
	this->m_activateAction->setCheckable(true);
	this->m_activateAction->setToolTip(tr("Registration"));
	this->m_activateAction->setChecked(false);
	this->m_modeLabel = new QLabel(tr("Mode"));
	this->m_modeLabel->setPixmap(QPixmap(QString(":/resource/toolbar2/registration_mode.png")).scaled(this->iconSize(), Qt::AspectRatioMode::IgnoreAspectRatio, Qt::TransformationMode::SmoothTransformation));
	this->m_modeComboBox = new QComboBox();
	this->m_modeComboBox->setFixedWidth(100);
	this->m_modeComboBox->addItem(tr("Three-Point"));
	this->m_modeComboBox->addItem(tr("Least Squares"));
	this->m_modeComboBox->addItem(tr("Iterative"));
	this->m_modeComboBox->setEnabled(false);
	this->m_sourceLabel = new QLabel(tr("Source"));
	this->m_sourceLabel->setPixmap(QPixmap(QString(":/resource/toolbar2/registration_source.png")).scaled(this->iconSize(), Qt::AspectRatioMode::IgnoreAspectRatio, Qt::TransformationMode::SmoothTransformation));
	this->m_sourceComboBox = new QComboBox();
	this->m_sourceComboBox->setFixedWidth(100);
	this->m_sourceComboBox->setEnabled(false);
	this->makeModelSelection(this->m_sourceComboBox);
	this->m_targetLabel = new QLabel(tr("Target"));
	this->m_targetLabel->setPixmap(QPixmap(QString(":/resource/toolbar2/registration_target.png")).scaled(this->iconSize(), Qt::AspectRatioMode::IgnoreAspectRatio, Qt::TransformationMode::SmoothTransformation));
	this->m_targetComboBox = new QComboBox();
	this->m_targetComboBox->setFixedWidth(100);
	this->m_targetComboBox->setEnabled(false);
	this->makeModelSelection(this->m_targetComboBox);
	this->m_startAction = new QAction(QIcon(QString(":/resource/toolbar2/start.png")), tr("Start"), this);
	this->m_startAction->setEnabled(false);

	// Layouts.
	this->addAction(this->m_activateAction);
	this->addSeparator();
	this->addControllerWidget(this->m_modeLabel, this->m_modeComboBox);
	this->addSeparator();
	this->addControllerWidget(this->m_sourceLabel, this->m_sourceComboBox);
	this->addControllerWidget(this->m_targetLabel, this->m_targetComboBox);
	this->addSeparator();
	this->addAction(this->m_startAction);

	// Signals and slots.
	connect(this->m_activateAction, SIGNAL(toggled(bool)), this, SLOT(updateWidget()));
	connect(this->m_activateAction, SIGNAL(toggled(bool)), this, SLOT(activate()));
	connect(this->m_modeComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(modify()));
	connect(this->m_sourceComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(modify()));
	connect(this->m_targetComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(modify()));
	connect(this->m_startAction, SIGNAL(triggered(bool)), this, SLOT(execute()));
}

const Settings &QVGRRegistrationModule::getSettings() const {
	static RegistrationSettings settings;
	settings.activated = this->m_activateAction->isChecked();
	settings.mode = this->m_modeComboBox->currentIndex();
	settings.source = this->m_sourceComboBox->currentIndex();
	settings.target = this->m_targetComboBox->currentIndex();
	return (settings);
}

void QVGRRegistrationModule::updateWidget() {
	bool enabled = this->m_activateAction->isChecked();
	this->m_modeComboBox->setEnabled(enabled);
	this->m_sourceComboBox->setEnabled(enabled);
	this->m_targetComboBox->setEnabled(enabled);
	this->m_startAction->setEnabled(enabled);
}



#pragma warning (pop)
