#include "QVGRColorizationModule.h"
#include <QtGui/QAction>
#include <QtGui/QComboBox>
#include <QtGui/QDoubleSpinBox>
#include <QtGui/QLabel>
#include <QtGui/QLayout>
#include <QtGui/QLineEdit>

#pragma warning (push)
#pragma warning (disable:4482)	/* nonstandard extension used: enum 'enum' used in qualified name */



QVGRColorizationModule::QVGRColorizationModule(const QString &title, QWidget *parent) : QVGRModule(title, parent) {

	// Widgets.
	this->m_activateAction = new QAction(QIcon(QString(":/resource/toolbar2/colorization_activate.png")), tr("Activate"), this);
	this->m_activateAction->setCheckable(true);
	this->m_activateAction->setToolTip(tr("Colorization"));
	this->m_modeLabel = new QLabel(tr("Mode"));
	this->m_modeLabel->setPixmap(QPixmap(QString(":/resource/toolbar2/colorization_mode.png")).scaled(this->iconSize(), Qt::AspectRatioMode::IgnoreAspectRatio, Qt::TransformationMode::SmoothTransformation));
	this->m_modeComboBox = new QComboBox();
	this->m_modeComboBox->setFixedWidth(100);
	this->m_modeComboBox->addItem(tr("N Component"));
	this->m_modeComboBox->addItem(tr("X Component"));
	this->m_modeComboBox->addItem(tr("Y Component"));
	this->m_modeComboBox->addItem(tr("Z Component"));
	this->m_modeComboBox->addItem(tr("N Deviation"));
	this->m_modeComboBox->addItem(tr("Curvature"));
	this->m_modeComboBox->setEnabled(false);
	this->m_sourceLabel = new QLabel(tr("Source"));
	this->m_sourceLabel->setPixmap(QPixmap(QString(":/resource/toolbar2/colorization_source.png")).scaled(this->iconSize(), Qt::AspectRatioMode::IgnoreAspectRatio, Qt::TransformationMode::SmoothTransformation));
	this->m_sourceComboBox = new QComboBox();
	this->m_sourceComboBox->setFixedWidth(100);
	this->m_sourceComboBox->setEnabled(false);
	this->makeModelSelection(this->m_sourceComboBox);
	this->m_targetLabel = new QLabel(tr("Target"));
	this->m_targetLabel->setPixmap(QPixmap(QString(":/resource/toolbar2/colorization_target.png")).scaled(this->iconSize(), Qt::AspectRatioMode::IgnoreAspectRatio, Qt::TransformationMode::SmoothTransformation));
	this->m_targetComboBox = new QComboBox();
	this->m_targetComboBox->setFixedWidth(100);
	this->makeModelSelection(this->m_targetComboBox);
	this->m_targetComboBox->setEnabled(false);
	this->m_resolutionLabel = new QLabel(tr("Resolution"));
	this->m_resolutionLabel->setPixmap(QPixmap(QString(":/resource/toolbar2/colorization_resolution.png")).scaled(this->iconSize(), Qt::AspectRatioMode::IgnoreAspectRatio, Qt::TransformationMode::SmoothTransformation));
	this->m_resolutionDoubleSpinBox = new QDoubleSpinBox();
	this->m_resolutionDoubleSpinBox->setFixedWidth(100);
	this->m_resolutionDoubleSpinBox->setSingleStep(1.0);
	this->m_resolutionDoubleSpinBox->setDecimals(0);
	this->m_resolutionDoubleSpinBox->setMinimum(1);
	this->m_resolutionDoubleSpinBox->setValue(10);
	this->m_resolutionDoubleSpinBox->setEnabled(false);
	this->m_rangeLabel = new QLabel(tr("Range"));
	this->m_rangeLabel->setPixmap(QPixmap(QString(":/resource/toolbar2/colorization_range.png")).scaled(this->iconSize(), Qt::AspectRatioMode::IgnoreAspectRatio, Qt::TransformationMode::SmoothTransformation));
	this->m_rangeLineEdit = new QLineEdit();
	this->m_rangeLineEdit->setFixedWidth(100);
	this->m_rangeLineEdit->setText(QString("1"));
	this->m_rangeLineEdit->setEnabled(false);
	this->m_toleranceLabel = new QLabel(tr("Tolerance"));
	this->m_toleranceLabel->setPixmap(QPixmap(QString(":/resource/toolbar2/colorization_tolerance.png")).scaled(this->iconSize(), Qt::AspectRatioMode::IgnoreAspectRatio, Qt::TransformationMode::SmoothTransformation));
	this->m_toleranceLineEdit = new QLineEdit();
	this->m_toleranceLineEdit->setFixedWidth(100);
	this->m_toleranceLineEdit->setText(QString("0"));
	this->m_toleranceLineEdit->setEnabled(false);
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
	this->addSeparator();
	this->addControllerWidget(this->m_resolutionLabel, this->m_resolutionDoubleSpinBox);
	this->addControllerWidget(this->m_rangeLabel, this->m_rangeLineEdit);
	this->addControllerWidget(this->m_toleranceLabel, this->m_toleranceLineEdit);

	// Signals and slots.
	connect(this->m_activateAction, SIGNAL(toggled(bool)), this, SLOT(updateWidget()));
	connect(this->m_activateAction, SIGNAL(toggled(bool)), this, SLOT(activate()));
	connect(this->m_modeComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(modify()));
	connect(this->m_sourceComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(modify()));
	connect(this->m_targetComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(modify()));
	connect(this->m_resolutionDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(update()));
	connect(this->m_rangeLineEdit, SIGNAL(textChanged(const QString &)), this, SLOT(update()));
	connect(this->m_toleranceLineEdit, SIGNAL(textChanged(const QString &)), this, SLOT(update()));
	connect(this->m_startAction, SIGNAL(triggered(bool)), this, SLOT(execute()));
}

const Settings &QVGRColorizationModule::getSettings() const {
	static ColorizationSettings settings;
	settings.activated = this->m_activateAction->isChecked();
	settings.mode = this->m_modeComboBox->currentIndex();
	settings.source = this->m_sourceComboBox->currentIndex();
	settings.target = this->m_targetComboBox->currentIndex();
	settings.resolution = this->m_resolutionDoubleSpinBox->value();
	settings.range = this->m_rangeLineEdit->text().toDouble();
	settings.tolerance = this->m_toleranceLineEdit->text().toDouble();
	return (settings);
}

void QVGRColorizationModule::applyResults(const Results &results) {
	if (typeid (results) != typeid(ColorizationResults)) return;
	const ColorizationResults &instance(dynamic_cast<const ColorizationResults &>(results));
	this->m_rangeLineEdit->setText(QString::number(std::min<double>(20.0, instance.peak)));
}

void QVGRColorizationModule::updateWidget() {
	bool enabled = this->m_activateAction->isChecked();
	this->m_modeComboBox->setEnabled(enabled);
	this->m_sourceComboBox->setEnabled(enabled);
	this->m_targetComboBox->setEnabled(enabled);
	this->m_resolutionDoubleSpinBox->setEnabled(enabled);
	this->m_rangeLineEdit->setEnabled(enabled);
	this->m_toleranceLineEdit->setEnabled(enabled);
	this->m_startAction->setEnabled(enabled);
}



#pragma warning (pop)
