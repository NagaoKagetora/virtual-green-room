#include "QVGRReflectionModule.h"
#include <QtGui/QAction>
#include <QtGui/QComboBox>
#include <QtGui/QDial>
#include <QtGui/QLabel>
#include <QtGui/QLayout>
#include <QtGui/QSlider>

#pragma warning (push)
#pragma warning (disable:4482)	/* nonstandard extension used: enum 'enum' used in qualified name */



QVGRReflectionModule::QVGRReflectionModule(const QString &title, QWidget *parent) : QVGRModule(title, parent) {

	// Widgets.
	this->m_activateAction = new QAction(QIcon(QString(":/resource/toolbar2/reflection_activate.png")), tr("Activate"), this);
	this->m_activateAction->setCheckable(true);
	this->m_activateAction->setToolTip(tr("Reflection"));
	this->m_modeLabel = new QLabel(tr("Mode"));
	this->m_modeLabel->setPixmap(QPixmap(QString(":/resource/toolbar2/reflection_mode.png")).scaled(this->iconSize(), Qt::AspectRatioMode::IgnoreAspectRatio, Qt::TransformationMode::SmoothTransformation));
	this->m_modeComboBox = new QComboBox();
	this->m_modeComboBox->setFixedWidth(100);
	this->m_modeComboBox->addItem(tr("Zebra"));
	this->m_modeComboBox->addItem(tr("Chessboard"));
	this->m_modeComboBox->addItem(tr("Circle Grid"));
	this->m_modeComboBox->setEnabled(false);
	this->m_scaleLabel = new QLabel(tr("Scale"));
	this->m_scaleLabel->setPixmap(QPixmap(QString(":/resource/toolbar2/reflection_scale.png")).scaled(this->iconSize(), Qt::AspectRatioMode::IgnoreAspectRatio, Qt::TransformationMode::SmoothTransformation));
	this->m_scaleSlider = new QSlider();
	this->m_scaleSlider->setFixedWidth(80);
	this->m_scaleSlider->setOrientation(Qt::Orientation::Horizontal);
	this->m_scaleSlider->setRange(0, 100);
	this->m_scaleSlider->setValue(50);
	this->m_scaleSlider->setEnabled(false);
	this->m_scaleDial = new QDial();
	this->m_scaleDial->setWrapping(true);
	this->m_scaleDial->setRange(0, 100);
	this->m_scaleDial->setValue(50);
	this->m_scaleDial->setEnabled(false);
	this->m_scaleDial->setVisible(false);
	this->m_rotationLabel = new QLabel(tr("Rotation"));
	this->m_rotationLabel->setPixmap(QPixmap(QString(":/resource/toolbar2/reflection_rotation.png")).scaled(this->iconSize(), Qt::AspectRatioMode::IgnoreAspectRatio, Qt::TransformationMode::SmoothTransformation));
	this->m_rotationSlider = new QSlider();
	this->m_rotationSlider->setFixedWidth(80);
	this->m_rotationSlider->setOrientation(Qt::Orientation::Horizontal);
	this->m_rotationSlider->setRange(0, 360);
	this->m_rotationSlider->setValue(0);
	this->m_rotationSlider->setEnabled(false);
	this->m_rotationDial = new QDial();
	this->m_rotationDial->setWrapping(true);
	this->m_rotationDial->setRange(0, 360);
	this->m_rotationDial->setValue(0);
	this->m_rotationDial->setEnabled(false);
	this->m_rotationDial->setVisible(false);

	// Layouts.
	this->addAction(this->m_activateAction);
	this->addSeparator();
	this->addControllerWidget(this->m_modeLabel, this->m_modeComboBox);
	this->addSeparator();
	this->addControllerWidget(this->m_scaleLabel, this->m_scaleSlider, this->m_scaleDial);
	this->addControllerWidget(this->m_rotationLabel, this->m_rotationSlider, this->m_rotationDial);

	// Signals and slots.
	connect(this->m_activateAction, SIGNAL(toggled(bool)), this, SLOT(updateWidget()));
	connect(this->m_activateAction, SIGNAL(toggled(bool)), this, SLOT(activate()));
	connect(this->m_modeComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(modify()));
	connect(this->m_scaleSlider, SIGNAL(valueChanged(int)), this, SLOT(update()));
	connect(this->m_scaleSlider, SIGNAL(valueChanged(int)), this->m_scaleDial, SLOT(setValue(int)));
	connect(this->m_scaleDial, SIGNAL(valueChanged(int)), this->m_scaleSlider, SLOT(setValue(int)));
	connect(this->m_rotationSlider, SIGNAL(valueChanged(int)), this, SLOT(update()));
	connect(this->m_rotationSlider, SIGNAL(valueChanged(int)), this->m_rotationDial, SLOT(setValue(int)));
	connect(this->m_rotationDial, SIGNAL(valueChanged(int)), this->m_rotationSlider, SLOT(setValue(int)));
}

const Settings &QVGRReflectionModule::getSettings() const {
	static ReflectionSettings settings;
	settings.activated = this->m_activateAction->isChecked();
	settings.mode = this->m_modeComboBox->currentIndex();
	settings.scale = this->m_scaleSlider->value();
	settings.rotation = -this->m_rotationSlider->value();
	return (settings);
}

void QVGRReflectionModule::updateWidget() {
	bool enabled = this->m_activateAction->isChecked();
	this->m_modeComboBox->setEnabled(enabled);
	this->m_scaleSlider->setEnabled(enabled);
	this->m_rotationSlider->setEnabled(enabled);
	this->m_scaleDial->setEnabled(enabled);
	this->m_rotationDial->setEnabled(enabled);
}

void QVGRReflectionModule::updateLayout() {
	this->m_scaleSlider->blockSignals(true);
	this->m_scaleDial->blockSignals(true);
	this->m_rotationSlider->blockSignals(true);
	this->m_rotationDial->blockSignals(true);

	this->m_scaleSlider->setEnabled(true);
	this->m_scaleDial->setEnabled(true);
	this->m_rotationSlider->setEnabled(true);
	this->m_rotationDial->setEnabled(true);

	bool vertical = (this->orientation() == Qt::Orientation::Horizontal);
	this->m_scaleSlider->setVisible(vertical);
	this->m_scaleDial->setVisible(!vertical);
	this->m_rotationSlider->setVisible(vertical);
	this->m_rotationDial->setVisible(!vertical);
	bool enabled = this->m_activateAction->isChecked();
	this->m_scaleSlider->setEnabled(enabled);
	this->m_scaleDial->setEnabled(enabled);
	this->m_rotationSlider->setEnabled(enabled);
	this->m_rotationDial->setEnabled(enabled);

	this->m_scaleSlider->blockSignals(false);
	this->m_scaleDial->blockSignals(false);
	this->m_rotationSlider->blockSignals(false);
	this->m_rotationDial->blockSignals(false);
}



#pragma warning (pop)
