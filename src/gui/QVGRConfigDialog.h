#ifndef _VGR_GUI_QVGRCONFIGDIALOG_H
#define _VGR_GUI_QVGRCONFIGDIALOG_H

#include <QtGui/QDialog>



class QColor;
class QGroupBox;
class QCheckBox;
class QDoubleSpinBox;
class QLabel;
class QLineEdit;
class QPushButton;
class QTabWidget;
class QVGRConfigDialog;



class QVGRConfigDialog : public QDialog {

	Q_OBJECT

private:
	QTabWidget							*m_settingsTabWidget;
	QGroupBox							*m_workspaceGroupBox;
	QLabel								*m_workspacePathLabel;
	QLineEdit							*m_workspacePathLineEdit;
	QPushButton							*m_workspacePathPushButton;
	QCheckBox							*m_usingRecentDirectoryCheckBox;
	QGroupBox							*m_sceneGroupBox;
	QLabel								*m_sceneBackgroundColorLabel;
	QLabel								*m_sceneBackgroundColorIconLabel;
	QLabel								*m_sceneBackgroundColorTextLabel;
	QPushButton							*m_sceneBackgroundColorPushButton;
	QGroupBox							*m_primitiveGroupBox;
	QLabel								*m_pointSizeLabel;
	QDoubleSpinBox						*m_pointSizeDoubleSpinBox;
	QLabel								*m_pointColorLabel;
	QLabel								*m_pointColorIconLabel;
	QLabel								*m_pointColorTextLabel;
	QPushButton							*m_pointColorPushButton;
	QLabel								*m_lineWidthLabel;
	QDoubleSpinBox						*m_lineWidthDoubleSpinBox;
	QLabel								*m_lineColorLabel;
	QLabel								*m_lineColorIconLabel;
	QLabel								*m_lineColorTextLabel;
	QPushButton							*m_lineColorPushButton;
	QGroupBox							*m_lightGroupBox;
	QLabel								*m_lightIntensityLabel;
	QLabel								*m_lightIntensityIconLabel;
	QLabel								*m_lightIntensityTextLabel;
	QPushButton							*m_lightIntensityPushButton;
	QGroupBox							*m_frontMaterialGroupBox;
	QLabel								*m_frontMaterialAmbientLabel;
	QLabel								*m_frontMaterialAmbientIconLabel;
	QLabel								*m_frontMaterialAmbientTextLabel;
	QPushButton							*m_frontMaterialAmbientPushButton;
	QLabel								*m_frontMaterialDiffuseLabel;
	QLabel								*m_frontMaterialDiffuseIconLabel;
	QLabel								*m_frontMaterialDiffuseTextLabel;
	QPushButton							*m_frontMaterialDiffusePushButton;
	QLabel								*m_frontMaterialSpecularLabel;
	QLabel								*m_frontMaterialSpecularIconLabel;
	QLabel								*m_frontMaterialSpecularTextLabel;
	QPushButton							*m_frontMaterialSpecularPushButton;
	QLabel								*m_frontMaterialShininessLabel;
	QDoubleSpinBox						*m_frontMaterialShininessDoubleSpinBox;
	QGroupBox							*m_backMaterialGroupBox;
	QLabel								*m_backMaterialAmbientLabel;
	QLabel								*m_backMaterialAmbientIconLabel;
	QLabel								*m_backMaterialAmbientTextLabel;
	QPushButton							*m_backMaterialAmbientPushButton;
	QLabel								*m_backMaterialDiffuseLabel;
	QLabel								*m_backMaterialDiffuseIconLabel;
	QLabel								*m_backMaterialDiffuseTextLabel;
	QPushButton							*m_backMaterialDiffusePushButton;
	QLabel								*m_backMaterialSpecularLabel;
	QLabel								*m_backMaterialSpecularIconLabel;
	QLabel								*m_backMaterialSpecularTextLabel;
	QPushButton							*m_backMaterialSpecularPushButton;
	QLabel								*m_backMaterialShininessLabel;
	QDoubleSpinBox						*m_backMaterialShininessDoubleSpinBox;
	QGroupBox							*m_effectMaterialGroupBox;
	QLabel								*m_effectMaterialAmbientLabel;
	QLabel								*m_effectMaterialAmbientIconLabel;
	QLabel								*m_effectMaterialAmbientTextLabel;
	QPushButton							*m_effectMaterialAmbientPushButton;
	QLabel								*m_effectMaterialDiffuseLabel;
	QLabel								*m_effectMaterialDiffuseIconLabel;
	QLabel								*m_effectMaterialDiffuseTextLabel;
	QPushButton							*m_effectMaterialDiffusePushButton;
	QLabel								*m_effectMaterialSpecularLabel;
	QLabel								*m_effectMaterialSpecularIconLabel;
	QLabel								*m_effectMaterialSpecularTextLabel;
	QPushButton							*m_effectMaterialSpecularPushButton;
	QLabel								*m_effectMaterialShininessLabel;
	QDoubleSpinBox						*m_effectMaterialShininessDoubleSpinBox;
	QPushButton							*m_confirmPushButton;
	QPushButton							*m_cancelPushButton;

private:
	static QColor toColor(QString string);
	static QString toString(QColor color);
	static void updateColorLabel(const QColor &color, QLabel *iconLabel, QLabel *textLabel);

public:
	QVGRConfigDialog(QWidget *parent, Qt::WFlags flags = 0);
	virtual ~QVGRConfigDialog() { }

public slots:
	void loadSettings();
	void updateSettings() const;

private slots:
	void onWorkspacePathPushButtonClicked();
	void onSceneBackgroundColorPushButtonClicked();
	void onLightIntensityPushButtonClicked();
	void onPointColorPushButtonClicked();
	void onLineColorPushButtonClicked();
	void onFrontMaterialAmbientPushButtonClicked();
	void onFrontMaterialDiffusePushButtonClicked();
	void onFrontMaterialSpecularPushButtonClicked();
	void onBackMaterialAmbientPushButtonClicked();
	void onBackMaterialDiffusePushButtonClicked();
	void onBackMaterialSpecularPushButtonClicked();
	void onEffectMaterialAmbientPushButtonClicked();
	void onEffectMaterialDiffusePushButtonClicked();
	void onEffectMaterialSpecularPushButtonClicked();
};



#endif /* _VGR_GUI_QVGRCONFIGDIALOG_H */
