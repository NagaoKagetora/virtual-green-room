#ifndef _VGR_GUI_QVGRVIEWERWIDGET_H
#define _VGR_GUI_QVGRVIEWERWIDGET_H

#include <QtCore/QMap>
#include <QtCore/QThread>
#include <QtGui/QWidget>
#include <osgViewer/View>
#include <osgViewer/ViewerEventHandlers>
#include <common/global.hpp>
#include <core/basic/Scene.hpp>
#include <core/basic/Function.hpp>
#include <gui/QVGRManager.h>
#include <gui/module/QVGRModule.h>



class QGridLayout;
class QVGRModule;
class QVGRExecutionThread;
class QVGRViewerWidget;



class QVGRExecutionThread : public QThread {

	Q_OBJECT

private:
	QVGRModule							*m_module;

public:
	QVGRExecutionThread() : m_module(NULL) { }

public:
	QVGRModule *module() const { return (this->m_module); }
	void setModule(QVGRModule *module) { this->m_module = module; }

protected:
	virtual void run() {
		if (!this->m_module) return;
		this->m_module->getFunction().execute(this->m_module->getSettings());
		this->m_module->applyResults(this->m_module->getFunction().getResults());
	}
};



class QVGRRenderingSettings {

private:
	osg::ref_ptr<osg::Uniform>			m_pointSize;
	osg::ref_ptr<osg::Uniform>			m_pointColor;
	osg::ref_ptr<osg::Uniform>			m_lineWidth;
	osg::ref_ptr<osg::Uniform>			m_lineColor;
	osg::ref_ptr<osg::Uniform>			m_lightPosition;
	osg::ref_ptr<osg::Uniform>			m_lightIntensity;
	osg::ref_ptr<osg::Uniform>			m_frontMaterialAmbient;
	osg::ref_ptr<osg::Uniform>			m_frontMaterialDiffuse;
	osg::ref_ptr<osg::Uniform>			m_frontMaterialSpecular;
	osg::ref_ptr<osg::Uniform>			m_frontMaterialShininess;
	osg::ref_ptr<osg::Uniform>			m_backMaterialAmbient;
	osg::ref_ptr<osg::Uniform>			m_backMaterialDiffuse;
	osg::ref_ptr<osg::Uniform>			m_backMaterialSpecular;
	osg::ref_ptr<osg::Uniform>			m_backMaterialShininess;
	osg::ref_ptr<osg::Uniform>			m_effectMaterialAmbient;
	osg::ref_ptr<osg::Uniform>			m_effectMaterialDiffuse;
	osg::ref_ptr<osg::Uniform>			m_effectMaterialSpecular;
	osg::ref_ptr<osg::Uniform>			m_effectMaterialShininess;
	osg::ref_ptr<osg::Uniform>			m_orthographicMode;
	osg::ref_ptr<osg::Uniform>			m_vertexMode;
	osg::ref_ptr<osg::Uniform>			m_edgeMode;
	osg::ref_ptr<osg::Uniform>			m_faceMode;

public:
	QVGRRenderingSettings();

public:
	const osg::ref_ptr<osg::Uniform> &pointSize() const { return (this->m_pointSize); }
	osg::ref_ptr<osg::Uniform> &pointSize() { return (this->m_pointSize); }
	const osg::ref_ptr<osg::Uniform> &pointColor() const { return (this->m_pointColor); }
	osg::ref_ptr<osg::Uniform> &pointColor() { return (this->m_pointColor); }
	const osg::ref_ptr<osg::Uniform> &lineWidth() const { return (this->m_lineWidth); }
	osg::ref_ptr<osg::Uniform> &lineWidth() { return (this->m_lineWidth); }
	const osg::ref_ptr<osg::Uniform> &lineColor() const { return (this->m_lineColor); }
	osg::ref_ptr<osg::Uniform> &lineColor() { return (this->m_lineColor); }
	const osg::ref_ptr<osg::Uniform> &lightPosition() const { return (this->m_lightPosition); }
	osg::ref_ptr<osg::Uniform> &lightPosition() { return (this->m_lightPosition); }
	const osg::ref_ptr<osg::Uniform> &lightIntensity() const { return (this->m_lightIntensity); }
	osg::ref_ptr<osg::Uniform> &lightIntensity() { return (this->m_lightIntensity); }
	const osg::ref_ptr<osg::Uniform> &frontMaterialAmbient() const { return (this->m_frontMaterialAmbient); }
	osg::ref_ptr<osg::Uniform> &frontMaterialAmbient() { return (this->m_frontMaterialAmbient); }
	const osg::ref_ptr<osg::Uniform> &frontMaterialDiffuse() const { return (this->m_frontMaterialDiffuse); }
	osg::ref_ptr<osg::Uniform> &frontMaterialDiffuse() { return (this->m_frontMaterialDiffuse); }
	const osg::ref_ptr<osg::Uniform> &frontMaterialSpecular() const { return (this->m_frontMaterialSpecular); }
	osg::ref_ptr<osg::Uniform> &frontMaterialSpecular() { return (this->m_frontMaterialSpecular); }
	const osg::ref_ptr<osg::Uniform> &frontMaterialShininess() const { return (this->m_frontMaterialShininess); }
	osg::ref_ptr<osg::Uniform> &frontMaterialShininess() { return (this->m_frontMaterialShininess); }
	const osg::ref_ptr<osg::Uniform> &backMaterialAmbient() const { return (this->m_backMaterialAmbient); }
	osg::ref_ptr<osg::Uniform> &backMaterialAmbient() { return (this->m_backMaterialAmbient); }
	const osg::ref_ptr<osg::Uniform> &backMaterialDiffuse() const { return (this->m_backMaterialDiffuse); }
	osg::ref_ptr<osg::Uniform> &backMaterialDiffuse() { return (this->m_backMaterialDiffuse); }
	const osg::ref_ptr<osg::Uniform> &backMaterialSpecular() const { return (this->m_backMaterialSpecular); }
	osg::ref_ptr<osg::Uniform> &backMaterialSpecular() { return (this->m_backMaterialSpecular); }
	const osg::ref_ptr<osg::Uniform> &backMaterialShininess() const { return (this->m_backMaterialShininess); }
	osg::ref_ptr<osg::Uniform> &backMaterialShininess() { return (this->m_backMaterialShininess); }
	const osg::ref_ptr<osg::Uniform> &effectMaterialAmbient() const { return (this->m_effectMaterialAmbient); }
	osg::ref_ptr<osg::Uniform> &effectMaterialAmbient() { return (this->m_effectMaterialAmbient); }
	const osg::ref_ptr<osg::Uniform> &effectMaterialDiffuse() const { return (this->m_effectMaterialDiffuse); }
	osg::ref_ptr<osg::Uniform> &effectMaterialDiffuse() { return (this->m_effectMaterialDiffuse); }
	const osg::ref_ptr<osg::Uniform> &effectMaterialSpecular() const { return (this->m_effectMaterialSpecular); }
	osg::ref_ptr<osg::Uniform> &effectMaterialSpecular() { return (this->m_effectMaterialSpecular); }
	const osg::ref_ptr<osg::Uniform> &effectMaterialShininess() const { return (this->m_effectMaterialShininess); }
	osg::ref_ptr<osg::Uniform> &effectMaterialShininess() { return (this->m_effectMaterialShininess); }
	const osg::ref_ptr<osg::Uniform> &orthographicMode() const { return (this->m_orthographicMode); }
	osg::ref_ptr<osg::Uniform> &orthographicMode() { return (this->m_orthographicMode); }
	const osg::ref_ptr<osg::Uniform> &vertexMode() const { return (this->m_vertexMode); }
	osg::ref_ptr<osg::Uniform> &vertexMode() { return (this->m_vertexMode); }
	const osg::ref_ptr<osg::Uniform> &edgeMode() const { return (this->m_edgeMode); }
	osg::ref_ptr<osg::Uniform> &edgeMode() { return (this->m_edgeMode); }
	const osg::ref_ptr<osg::Uniform> &faceMode() const { return (this->m_faceMode); }
	osg::ref_ptr<osg::Uniform> &faceMode() { return (this->m_faceMode); }

public:
	void refresh();
	void regist(StandardScene *scene);
};



class QVGRViewerWidget : public QWidget, public osgViewer::CompositeViewer {

	Q_OBJECT

private:
	static size_t						ScenesCount;

private:
	QVGRManager							*m_manager;
	QVGRRenderingSettings				m_settings;
	QVector<StandardScene *>			m_standards;
	QVector<Scene *>					m_scenes;
	QGridLayout							*m_layout;
	QVGRExecutionThread					*m_thread;
	QMap<QString, QVGRModule *>			m_modules;

public:
	static size_t scenesCount() { return (QVGRViewerWidget::ScenesCount); }

public:
	QVGRViewerWidget(QVGRManager *manager, QWidget *parent = 0);
	~QVGRViewerWidget() { }

public:
	const QVGRRenderingSettings &settings() const { return (this->m_settings); }
	QVGRRenderingSettings &settings() { return (this->m_settings); }
	const QVector<StandardScene *> &standards() const { return (this->m_standards); }
	QVector<StandardScene *> &standards() { return (this->m_standards); }
	const QVector<Scene *> &scenes() const { return (this->m_scenes); }
	QVector<Scene *> &scenes() { return (this->m_scenes); }
	const QMap<QString, QVGRModule *> &modules() const { return (this->m_modules); }
	QMap<QString, QVGRModule *> &modules() { return (this->m_modules); }

public:
	void setScenesAcceptDrops(bool on);
	void regist(QVGRModule *module);
	void dispose();
	void displayProject();
	void updateSettings();

protected:
	virtual void paintEvent(QPaintEvent *event);
	virtual void resizeEvent(QResizeEvent *event);

private:
	void resizeLayout();
	void clearLayout();
	void setupLayout();
	void addScenes();
	void removeScenes();
	void addScene(Scene *scene);
	void removeScene(Scene *scene);
	void addWindow(Scene *scene, int row, int column);
	void removeWindow(Scene *scene);

public slots:
	void refresh();
	void activateModule(QString name);
	void modifyModule(QString name);
	void updateModule(QString name);
	void executeModule(QString name);

private slots:
	void onItemDropped(QWidget *widget, int index);

signals:
	void taskStarted();
	void taskFinished();
	void filesDropped(const QStringList &fileNames);
};



#endif /* _VGR_GUI_QVGRVIEWERWIDGET_H */
