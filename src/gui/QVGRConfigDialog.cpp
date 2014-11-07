#include "QVGRConfigDialog.h"
#include <core/basic/Configuration.hpp>
#include <QtGui/QCheckBox>
#include <QtGui/QColor>
#include <QtGui/QColorDialog>
#include <QtGui/QDoubleSpinBox>
#include <QtGui/QFileDialog>
#include <QtGui/QGroupBox>
#include <QtGui/QLabel>
#include <QtGui/QLayout>
#include <QtGui/QLineEdit>
#include <QtGui/QPainter>
#include <QtGui/QPushButton>
#include <QtGui/QTabWidget>

#pragma warning (push)
#pragma warning (disable:4482)	/* nonstandard extension used: enum 'enum' used in qualified name */



void QVGRConfigDialog::updateColorLabel(const QColor &color, QLabel *iconLabel, QLabel *textLabel) {
	QPixmap pixmap(QSize(16, 16));
	pixmap.fill(color);
	QPainter painter;
	painter.begin(&pixmap);
	painter.setPen(QColor(40, 40, 40));
	painter.drawLine(QPoint( 0,  0), QPoint( 0, 15));
	painter.drawLine(QPoint( 0, 15), QPoint(15, 15));
	painter.drawLine(QPoint(15, 15), QPoint(15,  0));
	painter.drawLine(QPoint(15,  0), QPoint( 0,  0));
	painter.end();
	iconLabel->setPixmap(pixmap);
	textLabel->setText(toString(color));
}

QColor QVGRConfigDialog::toColor(QString string) {
	return (QColor(
			string.mid(1, 2).toInt(NULL, 16),
			string.mid(3, 2).toInt(NULL, 16),
			string.mid(5, 2).toInt(NULL, 16),
			string.mid(7, 2).toInt(NULL, 16)));
}

QString QVGRConfigDialog::toString(QColor color) {
	return (QString("#%1%2%3%4")
			.arg(color.red(), 2, 16, QChar('0'))
			.arg(color.green(), 2, 16, QChar('0'))
			.arg(color.blue(), 2, 16, QChar('0'))
			.arg(color.alpha(), 2, 16, QChar('0')).toUpper());
}

QVGRConfigDialog::QVGRConfigDialog(QWidget *parent, Qt::WFlags flags) : QDialog(parent, flags) {

	// Widgets.
	this->m_settingsTabWidget = new QTabWidget();
	this->m_workspaceGroupBox = new QGroupBox(tr("Workspace"));
	this->m_workspacePathLabel = new QLabel(tr("Directory:"));
	this->m_workspacePathLabel->setSizePolicy(QSizePolicy(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Fixed));
	this->m_workspacePathLineEdit = new QLineEdit();
	this->m_workspacePathPushButton = new QPushButton(QIcon(QString(":/resource/widget/config_button_directory.png")), QString());
	this->m_workspacePathPushButton->setSizePolicy(QSizePolicy(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Fixed));
	this->m_usingRecentDirectoryCheckBox = new QCheckBox(tr("Using recent directory."));
	this->m_sceneGroupBox = new QGroupBox(tr("Scene"));
	this->m_sceneBackgroundColorLabel = new QLabel(tr("Background:"));
	this->m_sceneBackgroundColorIconLabel = new QLabel();
	this->m_sceneBackgroundColorIconLabel->setSizePolicy(QSizePolicy(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Fixed));
	this->m_sceneBackgroundColorTextLabel = new QLabel();
	this->m_sceneBackgroundColorPushButton = new QPushButton(QIcon(QString(":/resource/widget/config_button_color.png")), QString());
	this->m_sceneBackgroundColorPushButton->setSizePolicy(QSizePolicy(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Fixed));
	this->m_primitiveGroupBox = new QGroupBox(tr("Primitive"));
	this->m_pointSizeLabel = new QLabel(tr("Point Size:"));
	this->m_pointSizeDoubleSpinBox = new QDoubleSpinBox();
	this->m_pointColorLabel = new QLabel(tr("Point Color:"));
	this->m_pointColorIconLabel = new QLabel();
	this->m_pointColorIconLabel->setSizePolicy(QSizePolicy(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Fixed));
	this->m_pointColorTextLabel = new QLabel();
	this->m_pointColorPushButton = new QPushButton(QIcon(QString(":/resource/widget/config_button_color.png")), QString());
	this->m_pointColorPushButton->setSizePolicy(QSizePolicy(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Fixed));
	this->m_lineWidthLabel = new QLabel(tr("Line Width:"));
	this->m_lineWidthDoubleSpinBox = new QDoubleSpinBox();
	this->m_lineColorLabel = new QLabel(tr("Line Color:"));
	this->m_lineColorIconLabel = new QLabel();
	this->m_lineColorIconLabel->setSizePolicy(QSizePolicy(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Fixed));
	this->m_lineColorTextLabel = new QLabel();
	this->m_lineColorPushButton = new QPushButton(QIcon(QString(":/resource/widget/config_button_color.png")), QString());
	this->m_lineColorPushButton->setSizePolicy(QSizePolicy(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Fixed));
	this->m_lightGroupBox = new QGroupBox(tr("Light"));
	this->m_lightIntensityLabel = new QLabel(tr("Intensity:"));
	this->m_lightIntensityIconLabel = new QLabel();
	this->m_lightIntensityIconLabel->setSizePolicy(QSizePolicy(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Fixed));
	this->m_lightIntensityTextLabel = new QLabel();
	this->m_lightIntensityPushButton = new QPushButton(QIcon(QString(":/resource/widget/config_button_color.png")), QString());
	this->m_lightIntensityPushButton->setSizePolicy(QSizePolicy(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Fixed));
	this->m_frontMaterialGroupBox = new QGroupBox(tr("Front"));
	this->m_frontMaterialAmbientLabel = new QLabel(tr("Ambient:"));
	this->m_frontMaterialAmbientIconLabel = new QLabel();
	this->m_frontMaterialAmbientIconLabel->setSizePolicy(QSizePolicy(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Fixed));
	this->m_frontMaterialAmbientTextLabel = new QLabel();
	this->m_frontMaterialAmbientPushButton = new QPushButton(QIcon(QString(":/resource/widget/config_button_color.png")), QString());
	this->m_frontMaterialAmbientPushButton->setSizePolicy(QSizePolicy(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Fixed));
	this->m_frontMaterialDiffuseLabel = new QLabel(tr("Diffuse:"));
	this->m_frontMaterialDiffuseIconLabel = new QLabel();
	this->m_frontMaterialDiffuseIconLabel->setSizePolicy(QSizePolicy(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Fixed));
	this->m_frontMaterialDiffuseTextLabel = new QLabel();
	this->m_frontMaterialDiffusePushButton = new QPushButton(QIcon(QString(":/resource/widget/config_button_color.png")), QString());
	this->m_frontMaterialDiffusePushButton->setSizePolicy(QSizePolicy(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Fixed));
	this->m_frontMaterialSpecularLabel = new QLabel(tr("Specular:"));
	this->m_frontMaterialSpecularIconLabel = new QLabel();
	this->m_frontMaterialSpecularIconLabel->setSizePolicy(QSizePolicy(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Fixed));
	this->m_frontMaterialSpecularTextLabel = new QLabel();
	this->m_frontMaterialSpecularPushButton = new QPushButton(QIcon(QString(":/resource/widget/config_button_color.png")), QString());
	this->m_frontMaterialSpecularPushButton->setSizePolicy(QSizePolicy(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Fixed));
	this->m_frontMaterialShininessLabel = new QLabel(tr("Shininess:"));
	this->m_frontMaterialShininessDoubleSpinBox = new QDoubleSpinBox();
	this->m_frontMaterialShininessDoubleSpinBox->setMinimum(0);
	this->m_frontMaterialShininessDoubleSpinBox->setMaximum(10000);
	this->m_backMaterialGroupBox = new QGroupBox(tr("Back"));
	this->m_backMaterialAmbientLabel = new QLabel(tr("Ambient:"));
	this->m_backMaterialAmbientIconLabel = new QLabel();
	this->m_backMaterialAmbientIconLabel->setSizePolicy(QSizePolicy(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Fixed));
	this->m_backMaterialAmbientTextLabel = new QLabel();
	this->m_backMaterialAmbientPushButton = new QPushButton(QIcon(QString(":/resource/widget/config_button_color.png")), QString());
	this->m_backMaterialAmbientPushButton->setSizePolicy(QSizePolicy(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Fixed));
	this->m_backMaterialDiffuseLabel = new QLabel(tr("Diffuse:"));
	this->m_backMaterialDiffuseIconLabel = new QLabel();
	this->m_backMaterialDiffuseIconLabel->setSizePolicy(QSizePolicy(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Fixed));
	this->m_backMaterialDiffuseTextLabel = new QLabel();
	this->m_backMaterialDiffusePushButton = new QPushButton(QIcon(QString(":/resource/widget/config_button_color.png")), QString());
	this->m_backMaterialDiffusePushButton->setSizePolicy(QSizePolicy(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Fixed));
	this->m_backMaterialSpecularLabel = new QLabel(tr("Specular:"));
	this->m_backMaterialSpecularIconLabel = new QLabel();
	this->m_backMaterialSpecularIconLabel->setSizePolicy(QSizePolicy(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Fixed));
	this->m_backMaterialSpecularTextLabel = new QLabel();
	this->m_backMaterialSpecularPushButton = new QPushButton(QIcon(QString(":/resource/widget/config_button_color.png")), QString());
	this->m_backMaterialSpecularPushButton->setSizePolicy(QSizePolicy(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Fixed));
	this->m_backMaterialShininessLabel = new QLabel(tr("Shininess:"));
	this->m_backMaterialShininessDoubleSpinBox = new QDoubleSpinBox();
	this->m_backMaterialShininessDoubleSpinBox->setMinimum(0);
	this->m_backMaterialShininessDoubleSpinBox->setMaximum(10000);
	this->m_effectMaterialGroupBox = new QGroupBox(tr("Effect"));
	this->m_effectMaterialAmbientLabel = new QLabel(tr("Ambient:"));
	this->m_effectMaterialAmbientIconLabel = new QLabel();
	this->m_effectMaterialAmbientIconLabel->setSizePolicy(QSizePolicy(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Fixed));
	this->m_effectMaterialAmbientTextLabel = new QLabel();
	this->m_effectMaterialAmbientPushButton = new QPushButton(QIcon(QString(":/resource/widget/config_button_color.png")), QString());
	this->m_effectMaterialAmbientPushButton->setSizePolicy(QSizePolicy(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Fixed));
	this->m_effectMaterialDiffuseLabel = new QLabel(tr("Diffuse:"));
	this->m_effectMaterialDiffuseIconLabel = new QLabel();
	this->m_effectMaterialDiffuseIconLabel->setSizePolicy(QSizePolicy(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Fixed));
	this->m_effectMaterialDiffuseTextLabel = new QLabel();
	this->m_effectMaterialDiffusePushButton = new QPushButton(QIcon(QString(":/resource/widget/config_button_color.png")), QString());
	this->m_effectMaterialDiffusePushButton->setSizePolicy(QSizePolicy(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Fixed));
	this->m_effectMaterialSpecularLabel = new QLabel(tr("Specular:"));
	this->m_effectMaterialSpecularIconLabel = new QLabel();
	this->m_effectMaterialSpecularIconLabel->setSizePolicy(QSizePolicy(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Fixed));
	this->m_effectMaterialSpecularTextLabel = new QLabel();
	this->m_effectMaterialSpecularPushButton = new QPushButton(QIcon(QString(":/resource/widget/config_button_color.png")), QString());
	this->m_effectMaterialSpecularPushButton->setSizePolicy(QSizePolicy(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Fixed));
	this->m_effectMaterialShininessLabel = new QLabel(tr("Shininess:"));
	this->m_effectMaterialShininessDoubleSpinBox = new QDoubleSpinBox();
	this->m_effectMaterialShininessDoubleSpinBox->setMinimum(0);
	this->m_effectMaterialShininessDoubleSpinBox->setMaximum(10000);
	this->m_confirmPushButton = new QPushButton(tr("Confirm"));
	this->m_confirmPushButton->setMinimumWidth(90);
	this->m_confirmPushButton->setSizePolicy(QSizePolicy(QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Fixed));
	this->m_cancelPushButton = new QPushButton(tr("Cancel"));
	this->m_cancelPushButton->setMinimumWidth(90);
	this->m_cancelPushButton->setSizePolicy(QSizePolicy(QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Fixed));

	// Layouts.
	QGridLayout	*workspaceLayout = new QGridLayout();
	workspaceLayout->addWidget(this->m_workspacePathLabel, 0, 0);
	workspaceLayout->addWidget(this->m_workspacePathLineEdit, 0, 1);
	workspaceLayout->addWidget(this->m_workspacePathPushButton, 0, 2);
	workspaceLayout->addWidget(this->m_usingRecentDirectoryCheckBox, 1, 0, 1, 3);
	this->m_workspaceGroupBox->setLayout(workspaceLayout);
	QGridLayout *sceneLayout = new QGridLayout();
	sceneLayout->addWidget(this->m_sceneBackgroundColorLabel, 0, 0);
	sceneLayout->addWidget(this->m_sceneBackgroundColorIconLabel, 0, 1);
	sceneLayout->addWidget(this->m_sceneBackgroundColorTextLabel, 0, 2);
	sceneLayout->addWidget(this->m_sceneBackgroundColorPushButton, 0, 3);
	this->m_sceneGroupBox->setLayout(sceneLayout);
	QGridLayout *primitiveLayout = new QGridLayout();
	primitiveLayout->addWidget(this->m_pointSizeLabel, 0, 0);
	primitiveLayout->addWidget(this->m_pointSizeDoubleSpinBox, 0, 1, 1, 2);
	primitiveLayout->addWidget(this->m_pointColorLabel, 1, 0);
	primitiveLayout->addWidget(this->m_pointColorIconLabel, 1, 1);
	primitiveLayout->addWidget(this->m_pointColorTextLabel, 1, 2);
	primitiveLayout->addWidget(this->m_pointColorPushButton, 1, 3);
	primitiveLayout->addWidget(this->m_lineWidthLabel, 2, 0);
	primitiveLayout->addWidget(this->m_lineWidthDoubleSpinBox, 2, 1, 1, 2);
	primitiveLayout->addWidget(this->m_lineColorLabel, 3, 0);
	primitiveLayout->addWidget(this->m_lineColorIconLabel, 3, 1);
	primitiveLayout->addWidget(this->m_lineColorTextLabel, 3, 2);
	primitiveLayout->addWidget(this->m_lineColorPushButton, 3, 3);
	this->m_primitiveGroupBox->setLayout(primitiveLayout);
	QGridLayout *lightLayout = new QGridLayout();
	lightLayout->addWidget(this->m_lightIntensityLabel, 0, 0);
	lightLayout->addWidget(this->m_lightIntensityIconLabel, 0, 1);
	lightLayout->addWidget(this->m_lightIntensityTextLabel, 0, 2);
	lightLayout->addWidget(this->m_lightIntensityPushButton, 0, 3);
	this->m_lightGroupBox->setLayout(lightLayout);
	QGridLayout *frontMaterialLayout = new QGridLayout();
	frontMaterialLayout->addWidget(this->m_frontMaterialAmbientLabel, 0, 0);
	frontMaterialLayout->addWidget(this->m_frontMaterialAmbientIconLabel, 0, 1);
	frontMaterialLayout->addWidget(this->m_frontMaterialAmbientTextLabel, 0, 2);
	frontMaterialLayout->addWidget(this->m_frontMaterialAmbientPushButton, 0, 3);
	frontMaterialLayout->addWidget(this->m_frontMaterialDiffuseLabel, 1, 0);
	frontMaterialLayout->addWidget(this->m_frontMaterialDiffuseIconLabel, 1, 1);
	frontMaterialLayout->addWidget(this->m_frontMaterialDiffuseTextLabel, 1, 2);
	frontMaterialLayout->addWidget(this->m_frontMaterialDiffusePushButton, 1, 3);
	frontMaterialLayout->addWidget(this->m_frontMaterialSpecularLabel, 2, 0);
	frontMaterialLayout->addWidget(this->m_frontMaterialSpecularIconLabel, 2, 1);
	frontMaterialLayout->addWidget(this->m_frontMaterialSpecularTextLabel, 2, 2);
	frontMaterialLayout->addWidget(this->m_frontMaterialSpecularPushButton, 2, 3);
	frontMaterialLayout->addWidget(this->m_frontMaterialShininessLabel, 3, 0);
	frontMaterialLayout->addWidget(this->m_frontMaterialShininessDoubleSpinBox, 3, 1, 1, 2);
	this->m_frontMaterialGroupBox->setLayout(frontMaterialLayout);
	QGridLayout *backMaterialLayout = new QGridLayout();
	backMaterialLayout->addWidget(this->m_backMaterialAmbientLabel, 0, 0);
	backMaterialLayout->addWidget(this->m_backMaterialAmbientIconLabel, 0, 1);
	backMaterialLayout->addWidget(this->m_backMaterialAmbientTextLabel, 0, 2);
	backMaterialLayout->addWidget(this->m_backMaterialAmbientPushButton, 0, 3);
	backMaterialLayout->addWidget(this->m_backMaterialDiffuseLabel, 1, 0);
	backMaterialLayout->addWidget(this->m_backMaterialDiffuseIconLabel, 1, 1);
	backMaterialLayout->addWidget(this->m_backMaterialDiffuseTextLabel, 1, 2);
	backMaterialLayout->addWidget(this->m_backMaterialDiffusePushButton, 1, 3);
	backMaterialLayout->addWidget(this->m_backMaterialSpecularLabel, 2, 0);
	backMaterialLayout->addWidget(this->m_backMaterialSpecularIconLabel, 2, 1);
	backMaterialLayout->addWidget(this->m_backMaterialSpecularTextLabel, 2, 2);
	backMaterialLayout->addWidget(this->m_backMaterialSpecularPushButton, 2, 3);
	backMaterialLayout->addWidget(this->m_backMaterialShininessLabel, 3, 0);
	backMaterialLayout->addWidget(this->m_backMaterialShininessDoubleSpinBox, 3, 1, 1, 2);
	this->m_backMaterialGroupBox->setLayout(backMaterialLayout);
	QGridLayout *effectMaterialLayout = new QGridLayout();
	effectMaterialLayout->addWidget(this->m_effectMaterialAmbientLabel, 0, 0);
	effectMaterialLayout->addWidget(this->m_effectMaterialAmbientIconLabel, 0, 1);
	effectMaterialLayout->addWidget(this->m_effectMaterialAmbientTextLabel, 0, 2);
	effectMaterialLayout->addWidget(this->m_effectMaterialAmbientPushButton, 0, 3);
	effectMaterialLayout->addWidget(this->m_effectMaterialDiffuseLabel, 1, 0);
	effectMaterialLayout->addWidget(this->m_effectMaterialDiffuseIconLabel, 1, 1);
	effectMaterialLayout->addWidget(this->m_effectMaterialDiffuseTextLabel, 1, 2);
	effectMaterialLayout->addWidget(this->m_effectMaterialDiffusePushButton, 1, 3);
	effectMaterialLayout->addWidget(this->m_effectMaterialSpecularLabel, 2, 0);
	effectMaterialLayout->addWidget(this->m_effectMaterialSpecularIconLabel, 2, 1);
	effectMaterialLayout->addWidget(this->m_effectMaterialSpecularTextLabel, 2, 2);
	effectMaterialLayout->addWidget(this->m_effectMaterialSpecularPushButton, 2, 3);
	effectMaterialLayout->addWidget(this->m_effectMaterialShininessLabel, 3, 0);
	effectMaterialLayout->addWidget(this->m_effectMaterialShininessDoubleSpinBox, 3, 1, 1, 2);
	this->m_effectMaterialGroupBox->setLayout(effectMaterialLayout);
	QVBoxLayout *applicationSettingsLayout = new QVBoxLayout();
	applicationSettingsLayout->addWidget(this->m_workspaceGroupBox);
	applicationSettingsLayout->addStretch();
	QWidget *applicationSettingsLayoutWidget = new QWidget();
	applicationSettingsLayoutWidget->setLayout(applicationSettingsLayout);
	this->m_settingsTabWidget->addTab(applicationSettingsLayoutWidget, tr("Application"));
	QVBoxLayout *sceneSettingsLayout = new QVBoxLayout();
	sceneSettingsLayout->addWidget(this->m_sceneGroupBox);
	sceneSettingsLayout->addWidget(this->m_primitiveGroupBox);
	sceneSettingsLayout->addWidget(this->m_lightGroupBox);
	sceneSettingsLayout->addStretch();
	QWidget *sceneSettingsLayoutWidget = new QWidget();
	sceneSettingsLayoutWidget->setLayout(sceneSettingsLayout);
	this->m_settingsTabWidget->addTab(sceneSettingsLayoutWidget, tr("Scene"));
	QVBoxLayout *materialSettingsLayout = new QVBoxLayout();
	materialSettingsLayout->addWidget(this->m_frontMaterialGroupBox);
	materialSettingsLayout->addWidget(this->m_backMaterialGroupBox);
	materialSettingsLayout->addWidget(this->m_effectMaterialGroupBox);
	materialSettingsLayout->addStretch();
	QWidget *materialSettingsLayoutWidget = new QWidget();
	materialSettingsLayoutWidget->setLayout(materialSettingsLayout);
	this->m_settingsTabWidget->addTab(materialSettingsLayoutWidget, tr("Material"));
	QHBoxLayout *buttonsLayout = new QHBoxLayout();
	buttonsLayout->addStretch();
	buttonsLayout->addWidget(this->m_confirmPushButton);
	buttonsLayout->addWidget(this->m_cancelPushButton);
	QVBoxLayout *layout = new QVBoxLayout();
	layout->addWidget(this->m_settingsTabWidget);
	layout->addStretch();
	layout->addLayout(buttonsLayout);

	// Main frame.
	this->setFont(QFont(QString("Arial"), 9));
	this->resize(300, 600);
	this->setMinimumWidth(300);
	this->setSizePolicy(QSizePolicy(QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Preferred));
	this->setWindowTitle(tr("Configuration"));
	this->setWindowIcon(QIcon(QString(":/resource/logo/logo.png")));
	this->setLayout(layout);
	
	// Signals and slots.
	connect(this->m_workspacePathPushButton, SIGNAL(clicked()), this, SLOT(onWorkspacePathPushButtonClicked()));
	connect(this->m_sceneBackgroundColorPushButton, SIGNAL(clicked()), this, SLOT(onSceneBackgroundColorPushButtonClicked()));
	connect(this->m_lightIntensityPushButton, SIGNAL(clicked()), this, SLOT(onLightIntensityPushButtonClicked()));
	connect(this->m_pointColorPushButton, SIGNAL(clicked()), this, SLOT(onPointColorPushButtonClicked()));
	connect(this->m_lineColorPushButton, SIGNAL(clicked()), this, SLOT(onLineColorPushButtonClicked()));
	connect(this->m_frontMaterialAmbientPushButton, SIGNAL(clicked()), this, SLOT(onFrontMaterialAmbientPushButtonClicked()));
	connect(this->m_frontMaterialDiffusePushButton, SIGNAL(clicked()), this, SLOT(onFrontMaterialDiffusePushButtonClicked()));
	connect(this->m_frontMaterialSpecularPushButton, SIGNAL(clicked()), this, SLOT(onFrontMaterialSpecularPushButtonClicked()));
	connect(this->m_backMaterialAmbientPushButton, SIGNAL(clicked()), this, SLOT(onBackMaterialAmbientPushButtonClicked()));
	connect(this->m_backMaterialDiffusePushButton, SIGNAL(clicked()), this, SLOT(onBackMaterialDiffusePushButtonClicked()));
	connect(this->m_backMaterialSpecularPushButton, SIGNAL(clicked()), this, SLOT(onBackMaterialSpecularPushButtonClicked()));
	connect(this->m_effectMaterialAmbientPushButton, SIGNAL(clicked()), this, SLOT(onEffectMaterialAmbientPushButtonClicked()));
	connect(this->m_effectMaterialDiffusePushButton, SIGNAL(clicked()), this, SLOT(onEffectMaterialDiffusePushButtonClicked()));
	connect(this->m_effectMaterialSpecularPushButton, SIGNAL(clicked()), this, SLOT(onEffectMaterialSpecularPushButtonClicked()));
	connect(this->m_confirmPushButton, SIGNAL(clicked()), this, SLOT(accept()));
	connect(this->m_cancelPushButton, SIGNAL(clicked()), this, SLOT(reject()));
}

void QVGRConfigDialog::loadSettings() {
	this->m_workspacePathLineEdit->setText(Configuration::instance().applicationConfiguration().workspacePath());
	this->m_usingRecentDirectoryCheckBox->setChecked(Configuration::instance().applicationConfiguration().usingRecentDirectory());
	updateColorLabel(Configuration::instance().sceneConfiguration().sceneBackgroundColor(), this->m_sceneBackgroundColorIconLabel, this->m_sceneBackgroundColorTextLabel);
	updateColorLabel(Configuration::instance().sceneConfiguration().lightIntensity(), this->m_lightIntensityIconLabel, this->m_lightIntensityTextLabel);
	this->m_pointSizeDoubleSpinBox->setValue(Configuration::instance().sceneConfiguration().pointSize());
	updateColorLabel(Configuration::instance().sceneConfiguration().pointColor(), this->m_pointColorIconLabel, this->m_pointColorTextLabel);
	this->m_lineWidthDoubleSpinBox->setValue(Configuration::instance().sceneConfiguration().lineWidth());
	updateColorLabel(Configuration::instance().sceneConfiguration().lineColor(), this->m_lineColorIconLabel, this->m_lineColorTextLabel);
	updateColorLabel(Configuration::instance().sceneConfiguration().frontMaterialAmbient(), this->m_frontMaterialAmbientIconLabel, this->m_frontMaterialAmbientTextLabel);
	updateColorLabel(Configuration::instance().sceneConfiguration().frontMaterialDiffuse(), this->m_frontMaterialDiffuseIconLabel, this->m_frontMaterialDiffuseTextLabel);
	updateColorLabel(Configuration::instance().sceneConfiguration().frontMaterialSpecular(), this->m_frontMaterialSpecularIconLabel, this->m_frontMaterialSpecularTextLabel);
	this->m_frontMaterialShininessDoubleSpinBox->setValue(Configuration::instance().sceneConfiguration().frontMaterialShininess());
	updateColorLabel(Configuration::instance().sceneConfiguration().backMaterialAmbient(), this->m_backMaterialAmbientIconLabel, this->m_backMaterialAmbientTextLabel);
	updateColorLabel(Configuration::instance().sceneConfiguration().backMaterialDiffuse(), this->m_backMaterialDiffuseIconLabel, this->m_backMaterialDiffuseTextLabel);
	updateColorLabel(Configuration::instance().sceneConfiguration().backMaterialSpecular(), this->m_backMaterialSpecularIconLabel, this->m_backMaterialSpecularTextLabel);
	this->m_backMaterialShininessDoubleSpinBox->setValue(Configuration::instance().sceneConfiguration().backMaterialShininess());
	updateColorLabel(Configuration::instance().sceneConfiguration().effectMaterialAmbient(), this->m_effectMaterialAmbientIconLabel, this->m_effectMaterialAmbientTextLabel);
	updateColorLabel(Configuration::instance().sceneConfiguration().effectMaterialDiffuse(), this->m_effectMaterialDiffuseIconLabel, this->m_effectMaterialDiffuseTextLabel);
	updateColorLabel(Configuration::instance().sceneConfiguration().effectMaterialSpecular(), this->m_effectMaterialSpecularIconLabel, this->m_effectMaterialSpecularTextLabel);
	this->m_effectMaterialShininessDoubleSpinBox->setValue(Configuration::instance().sceneConfiguration().effectMaterialShininess());
}

void QVGRConfigDialog::updateSettings() const {
	Configuration::instance().applicationConfiguration().setWorkspacePath(this->m_workspacePathLineEdit->text());
	Configuration::instance().applicationConfiguration().setUsingRecentDirectory(this->m_usingRecentDirectoryCheckBox->isChecked());
	Configuration::instance().sceneConfiguration().setSceneBackgroundColor(toColor(this->m_sceneBackgroundColorTextLabel->text()));
	Configuration::instance().sceneConfiguration().setLightIntensity(toColor(this->m_lightIntensityTextLabel->text()));
	Configuration::instance().sceneConfiguration().setPointSize(this->m_pointSizeDoubleSpinBox->value());
	Configuration::instance().sceneConfiguration().setPointColor(toColor(this->m_pointColorTextLabel->text()));
	Configuration::instance().sceneConfiguration().setLineWidth(this->m_lineWidthDoubleSpinBox->value());
	Configuration::instance().sceneConfiguration().setLineColor(toColor(this->m_lineColorTextLabel->text()));
	Configuration::instance().sceneConfiguration().setFrontMaterialAmbient(toColor(this->m_frontMaterialAmbientTextLabel->text()));
	Configuration::instance().sceneConfiguration().setFrontMaterialDiffuse(toColor(this->m_frontMaterialDiffuseTextLabel->text()));
	Configuration::instance().sceneConfiguration().setFrontMaterialSpecular(toColor(this->m_frontMaterialSpecularTextLabel->text()));
	Configuration::instance().sceneConfiguration().setFrontMaterialShininess(this->m_frontMaterialShininessDoubleSpinBox->value());
	Configuration::instance().sceneConfiguration().setBackMaterialAmbient(toColor(this->m_backMaterialAmbientTextLabel->text()));
	Configuration::instance().sceneConfiguration().setBackMaterialDiffuse(toColor(this->m_backMaterialDiffuseTextLabel->text()));
	Configuration::instance().sceneConfiguration().setBackMaterialSpecular(toColor(this->m_backMaterialSpecularTextLabel->text()));
	Configuration::instance().sceneConfiguration().setBackMaterialShininess(this->m_backMaterialShininessDoubleSpinBox->value());
	Configuration::instance().sceneConfiguration().setEffectMaterialAmbient(toColor(this->m_effectMaterialAmbientTextLabel->text()));
	Configuration::instance().sceneConfiguration().setEffectMaterialDiffuse(toColor(this->m_effectMaterialDiffuseTextLabel->text()));
	Configuration::instance().sceneConfiguration().setEffectMaterialSpecular(toColor(this->m_effectMaterialSpecularTextLabel->text()));
	Configuration::instance().sceneConfiguration().setEffectMaterialShininess(this->m_effectMaterialShininessDoubleSpinBox->value());
}

void QVGRConfigDialog::onWorkspacePathPushButtonClicked() {
	QString directory = QFileDialog::getExistingDirectory(this, tr("Select Workspace Path"),
			Configuration::instance().applicationConfiguration().workspacePath());
	if (directory.length()) {
		this->m_workspacePathLineEdit->setText(directory);
	}
}

void QVGRConfigDialog::onSceneBackgroundColorPushButtonClicked() {
	QColorDialog dialog(toColor(this->m_sceneBackgroundColorTextLabel->text()));
	dialog.setOption(QColorDialog::ColorDialogOption::ShowAlphaChannel);
	if (dialog.exec()) {
		updateColorLabel(dialog.selectedColor(), this->m_sceneBackgroundColorIconLabel, this->m_sceneBackgroundColorTextLabel);
	}
}

void QVGRConfigDialog::onLightIntensityPushButtonClicked() {
	QColorDialog dialog(toColor(this->m_lightIntensityTextLabel->text()));
	dialog.setOption(QColorDialog::ColorDialogOption::ShowAlphaChannel);
	if (dialog.exec()) {
		updateColorLabel(dialog.selectedColor(), this->m_lightIntensityIconLabel, this->m_lightIntensityTextLabel);
	}
}

void QVGRConfigDialog::onPointColorPushButtonClicked() {
	QColorDialog dialog(toColor(this->m_pointColorTextLabel->text()));
	dialog.setOption(QColorDialog::ColorDialogOption::ShowAlphaChannel);
	if (dialog.exec()) {
		updateColorLabel(dialog.selectedColor(), this->m_pointColorIconLabel, this->m_pointColorTextLabel);
	}
}

void QVGRConfigDialog::onLineColorPushButtonClicked() {
	QColorDialog dialog(toColor(this->m_lineColorTextLabel->text()));
	dialog.setOption(QColorDialog::ColorDialogOption::ShowAlphaChannel);
	if (dialog.exec()) {
		updateColorLabel(dialog.selectedColor(), this->m_lineColorIconLabel, this->m_lineColorTextLabel);
	}
}

void QVGRConfigDialog::onFrontMaterialAmbientPushButtonClicked() {
	QColorDialog dialog(toColor(this->m_frontMaterialAmbientTextLabel->text()));
	dialog.setOption(QColorDialog::ColorDialogOption::ShowAlphaChannel);
	if (dialog.exec()) {
		updateColorLabel(dialog.selectedColor(), this->m_frontMaterialAmbientIconLabel, this->m_frontMaterialAmbientTextLabel);
	}
}

void QVGRConfigDialog::onFrontMaterialDiffusePushButtonClicked() {
	QColorDialog dialog(toColor(this->m_frontMaterialDiffuseTextLabel->text()));
	dialog.setOption(QColorDialog::ColorDialogOption::ShowAlphaChannel);
	if (dialog.exec()) {
		updateColorLabel(dialog.selectedColor(), this->m_frontMaterialDiffuseIconLabel, this->m_frontMaterialDiffuseTextLabel);
	}
}

void QVGRConfigDialog::onFrontMaterialSpecularPushButtonClicked() {
	QColorDialog dialog(toColor(this->m_frontMaterialSpecularTextLabel->text()));
	dialog.setOption(QColorDialog::ColorDialogOption::ShowAlphaChannel);
	if (dialog.exec()) {
		updateColorLabel(dialog.selectedColor(), this->m_frontMaterialSpecularIconLabel, this->m_frontMaterialSpecularTextLabel);
	}
}

void QVGRConfigDialog::onBackMaterialAmbientPushButtonClicked() {
	QColorDialog dialog(toColor(this->m_backMaterialAmbientTextLabel->text()));
	dialog.setOption(QColorDialog::ColorDialogOption::ShowAlphaChannel);
	if (dialog.exec()) {
		updateColorLabel(dialog.selectedColor(), this->m_backMaterialAmbientIconLabel, this->m_backMaterialAmbientTextLabel);
	}
}

void QVGRConfigDialog::onBackMaterialDiffusePushButtonClicked() {
	QColorDialog dialog(toColor(this->m_backMaterialDiffuseTextLabel->text()));
	dialog.setOption(QColorDialog::ColorDialogOption::ShowAlphaChannel);
	if (dialog.exec()) {
		updateColorLabel(dialog.selectedColor(), this->m_backMaterialDiffuseIconLabel, this->m_backMaterialDiffuseTextLabel);
	}
}

void QVGRConfigDialog::onBackMaterialSpecularPushButtonClicked() {
	QColorDialog dialog(toColor(this->m_backMaterialSpecularTextLabel->text()));
	dialog.setOption(QColorDialog::ColorDialogOption::ShowAlphaChannel);
	if (dialog.exec()) {
		updateColorLabel(dialog.selectedColor(), this->m_backMaterialSpecularIconLabel, this->m_backMaterialSpecularTextLabel);
	}
}

void QVGRConfigDialog::onEffectMaterialAmbientPushButtonClicked() {
	QColorDialog dialog(toColor(this->m_effectMaterialAmbientTextLabel->text()));
	dialog.setOption(QColorDialog::ColorDialogOption::ShowAlphaChannel);
	if (dialog.exec()) {
		updateColorLabel(dialog.selectedColor(), this->m_effectMaterialAmbientIconLabel, this->m_effectMaterialAmbientTextLabel);
	}
}

void QVGRConfigDialog::onEffectMaterialDiffusePushButtonClicked() {
	QColorDialog dialog(toColor(this->m_effectMaterialDiffuseTextLabel->text()));
	dialog.setOption(QColorDialog::ColorDialogOption::ShowAlphaChannel);
	if (dialog.exec()) {
		updateColorLabel(dialog.selectedColor(), this->m_effectMaterialDiffuseIconLabel, this->m_effectMaterialDiffuseTextLabel);
	}
}

void QVGRConfigDialog::onEffectMaterialSpecularPushButtonClicked() {
	QColorDialog dialog(toColor(this->m_effectMaterialSpecularTextLabel->text()));
	dialog.setOption(QColorDialog::ColorDialogOption::ShowAlphaChannel);
	if (dialog.exec()) {
		updateColorLabel(dialog.selectedColor(), this->m_effectMaterialSpecularIconLabel, this->m_effectMaterialSpecularTextLabel);
	}
}



#pragma warning (pop)
