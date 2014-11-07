#include "QVGRCreateProjectDialog.h"
#include <QtCore/QFileInfo>
#include <QtGui/QFileDialog>
#include <QtGui/QLabel>
#include <QtGui/QLayout>
#include <QtGui/QLineEdit>
#include <QtGui/QListWidget>
#include <QtGui/QPushButton>
#include <core/basic/Configuration.hpp>

#pragma warning (push)
#pragma warning (disable:4482)	/* nonstandard extension used: enum 'enum' used in qualified name */



QVGRCreateProjectDialog::QVGRCreateProjectDialog(QWidget *parent, Qt::WFlags flags) : QDialog(parent, flags) {

	// Widgets.
	this->m_projectNameLabel = new QLabel(tr("Project Name:"));
	this->m_projectNameLineEdit = new QLineEdit();
	this->m_fileNamesListWidget = new QListWidget();
	this->m_addPushButton = new QPushButton(tr("Add"));
	this->m_addPushButton->setMinimumWidth(90);
	this->m_addPushButton->setSizePolicy(QSizePolicy(QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Fixed));
	this->m_removePushButton = new QPushButton(tr("Remove"));
	this->m_removePushButton->setMinimumWidth(90);
	this->m_removePushButton->setSizePolicy(QSizePolicy(QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Fixed));
	this->m_removePushButton->setEnabled(false);
	this->m_clearPushButton = new QPushButton(tr("Clear"));
	this->m_clearPushButton->setMinimumWidth(90);
	this->m_clearPushButton->setSizePolicy(QSizePolicy(QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Fixed));
	this->m_upPushButton = new QPushButton(tr("Up"));
	this->m_upPushButton->setMinimumWidth(90);
	this->m_upPushButton->setSizePolicy(QSizePolicy(QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Fixed));
	this->m_upPushButton->setEnabled(false);
	this->m_downPushButton = new QPushButton(tr("Down"));
	this->m_downPushButton->setMinimumWidth(90);
	this->m_downPushButton->setSizePolicy(QSizePolicy(QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Fixed));
	this->m_downPushButton->setEnabled(false);
	this->m_confirmPushButton = new QPushButton(tr("Confirm"));
	this->m_confirmPushButton->setMinimumWidth(90);
	this->m_confirmPushButton->setSizePolicy(QSizePolicy(QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Fixed));
	this->m_cancelPushButton = new QPushButton(tr("Cancel"));
	this->m_cancelPushButton->setMinimumWidth(90);
	this->m_cancelPushButton->setSizePolicy(QSizePolicy(QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Fixed));

	// Layouts.
	QHBoxLayout *projectNameLayout = new QHBoxLayout();
	projectNameLayout->addWidget(this->m_projectNameLabel);
	projectNameLayout->addWidget(this->m_projectNameLineEdit);
	QVBoxLayout *fileNamesButtonsLayout = new QVBoxLayout();
	fileNamesButtonsLayout->addWidget(this->m_addPushButton);
	fileNamesButtonsLayout->addWidget(this->m_removePushButton);
	fileNamesButtonsLayout->addWidget(this->m_clearPushButton);
	fileNamesButtonsLayout->addWidget(this->m_upPushButton);
	fileNamesButtonsLayout->addWidget(this->m_downPushButton);
	fileNamesButtonsLayout->addStretch();
	QHBoxLayout *fileNamesLayout = new QHBoxLayout();
	fileNamesLayout->addWidget(this->m_fileNamesListWidget);
	fileNamesLayout->addLayout(fileNamesButtonsLayout);
	QHBoxLayout *dialogButtonsLayout = new QHBoxLayout();
	dialogButtonsLayout->addStretch();
	dialogButtonsLayout->addWidget(this->m_confirmPushButton);
	dialogButtonsLayout->addWidget(this->m_cancelPushButton);
	QVBoxLayout *layout = new QVBoxLayout();
	layout->addLayout(projectNameLayout);
	layout->addLayout(fileNamesLayout);
	layout->addStretch();
	layout->addLayout(dialogButtonsLayout);
	this->setLayout(layout);

	this->setFont(QFont(QString("Arial"), 9));
	this->setWindowTitle(tr("Create Project"));
	this->setMinimumSize(600, 300);

	// Signals and slots.
	connect(this->m_fileNamesListWidget, SIGNAL(currentRowChanged(int)), this, SLOT(onFileNamesListWidgetCurrentRowChanged(int)));
	connect(this->m_addPushButton, SIGNAL(clicked()), this, SLOT(onAddPushButtonClicked()));
	connect(this->m_removePushButton, SIGNAL(clicked()), this, SLOT(onRemovePushButtonClicked()));
	connect(this->m_clearPushButton, SIGNAL(clicked()), this, SLOT(onClearPushButtonClicked()));
	connect(this->m_upPushButton, SIGNAL(clicked()), this, SLOT(onUpPushButtonClicked()));
	connect(this->m_downPushButton, SIGNAL(clicked()), this, SLOT(onDownPushButtonClicked()));
	connect(this->m_confirmPushButton, SIGNAL(clicked()), this, SLOT(accept()));
	connect(this->m_cancelPushButton, SIGNAL(clicked()), this, SLOT(reject()));
}

QString QVGRCreateProjectDialog::projectName() const {
	return (this->m_projectNameLineEdit->text());
}

QStringList QVGRCreateProjectDialog::fileNames() const {
	QStringList fileNames;
	for (int i = 0; i < this->m_fileNamesListWidget->count(); ++i) {
		fileNames.append(this->m_fileNamesListWidget->item(i)->text());
	}
	return (fileNames);
}

void QVGRCreateProjectDialog::onFileNamesListWidgetCurrentRowChanged(int index) {
	this->m_removePushButton->setEnabled(true);
	this->m_clearPushButton->setEnabled(true);
	this->m_upPushButton->setEnabled(true);
	this->m_downPushButton->setEnabled(true);
	if (index == 0)
		this->m_upPushButton->setEnabled(false);
	if (index == (this->m_fileNamesListWidget->count() - 1))
		this->m_downPushButton->setEnabled(false);
}

void QVGRCreateProjectDialog::onAddPushButtonClicked() {
	QStringList fileNames = QFileDialog::getOpenFileNames(this, tr("Select Model Files"),
			Configuration::instance().applicationConfiguration().recentDirectory(),
			(QStringList()
					<< QString("All Supported Formats (*.nas *.stl *.obj)")
					<< QString("NAS File (*.nas)")
					<< QString("STL File (*.stl)")
					<< QString("OBJ File (*.obj)")
					<< QString("All Files (*.*)")).join(QString(";;")));
	foreach (QString fileName, fileNames) {
		QFileInfo fileInfo(fileName);
		Configuration::instance().applicationConfiguration().setRecentDirectory(fileInfo.absoluteDir().absolutePath());
		this->m_fileNamesListWidget->addItem(fileInfo.absoluteFilePath());
	}
	if (fileNames.count() > 0)
		this->m_fileNamesListWidget->setCurrentRow(this->m_fileNamesListWidget->count() - 1);
}

void QVGRCreateProjectDialog::onRemovePushButtonClicked() {
	int row = this->m_fileNamesListWidget->currentRow();
	delete this->m_fileNamesListWidget->currentItem();
	if (this->m_fileNamesListWidget->count() == 0) {
		this->m_upPushButton->setEnabled(false);
		this->m_downPushButton->setEnabled(false);
		this->m_removePushButton->setEnabled(false);
	} else {
		this->m_fileNamesListWidget->setCurrentRow(std::min<int>(row,
				this->m_fileNamesListWidget->count() - 1));
	}
}

void QVGRCreateProjectDialog::onClearPushButtonClicked() {
	this->m_fileNamesListWidget->clear();
	this->m_upPushButton->setEnabled(false);
	this->m_downPushButton->setEnabled(false);
	this->m_removePushButton->setEnabled(false);
}

void QVGRCreateProjectDialog::onUpPushButtonClicked() {
	int row = this->m_fileNamesListWidget->currentRow() - 1;
	QString str = this->m_fileNamesListWidget->currentItem()->text();
	delete this->m_fileNamesListWidget->currentItem();
	this->m_fileNamesListWidget->insertItem(row, str);
	this->m_fileNamesListWidget->setCurrentRow(row);
}

void QVGRCreateProjectDialog::onDownPushButtonClicked() {
	int row = this->m_fileNamesListWidget->currentRow() + 1;
	QString str = this->m_fileNamesListWidget->currentItem()->text();
	delete this->m_fileNamesListWidget->currentItem();
	this->m_fileNamesListWidget->insertItem(row, str);
	this->m_fileNamesListWidget->setCurrentRow(row);
}



#pragma warning (pop)
