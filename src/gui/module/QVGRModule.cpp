#include "QVGRModule.h"
#include <QtGui/QComboBox>
#include <QtGui/QLayout>
#include <gui/QVGRViewerWidget.h>

#pragma warning (push)
#pragma warning (disable:4482)	/* nonstandard extension used: enum 'enum' used in qualified name */



QVGRModule::QVGRModule(const QString &title, QWidget *parent) : QToolBar(title, parent) {

	connect(this, SIGNAL(orientationChanged(Qt::Orientation)), this, SLOT(updateLayout()));
}

void QVGRModule::makeModelSelection(QComboBox *modelComboBox) {
	if (!modelComboBox) return;
	for (size_t i = 0; i < QVGRViewerWidget::scenesCount(); ++i) {
		modelComboBox->addItem(QString("View %1").arg(i + 1));
	}
}

QAction *QVGRModule::addControllerWidget(QWidget *indicator, QWidget *controller) {
	QHBoxLayout *layout = new QHBoxLayout();
	layout->setContentsMargins(5, 3, 5, 3);
	layout->addWidget(indicator);
	layout->addWidget(controller);
	layout->setSizeConstraint(QLayout::SizeConstraint::SetFixedSize);
	QWidget *widget = new QWidget();
	widget->setLayout(layout);
	return (this->addWidget(widget));
}

QAction *QVGRModule::addControllerWidget(QWidget *indicator, QWidget *horizontalController, QWidget *verticalController) {
	QHBoxLayout *layout = new QHBoxLayout();
	layout->setContentsMargins(5, 3, 5, 3);
	layout->addWidget(indicator);
	layout->addWidget(horizontalController);
	layout->addWidget(verticalController);
	layout->setSizeConstraint(QLayout::SizeConstraint::SetFixedSize);
	QWidget *widget = new QWidget();
	widget->setLayout(layout);
	return (this->addWidget(widget));
}



void QVGRModuleGroup::setEnabled(bool enabled) {
	foreach (QVGRModule *module, *this) {
		module->setEnabled(enabled);
	}
}

bool QVGRModuleGroup::activated() const {
	bool activated = false;
	foreach (QVGRModule *module, *this) {
		activated |= module->getSettings().activated;
	}
	return (activated);
}



#pragma warning (pop)
