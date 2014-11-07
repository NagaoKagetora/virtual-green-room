#include "QVGRSnapshotWidget.h"
#include <core/basic/Configuration.hpp>
#include <gui/QVGRManager.h>
#include <QtCore/QFileInfo>
#include <QtGui/QAction>
#include <QtGui/QFileDialog>
#include <QtGui/QIcon>
#include <QtGui/QLabel>
#include <QtGui/QLayout>
#include <QtGui/QPushButton>
#include <QtGui/QStandardItem>
#include <QtGui/QStandardItemModel>
#include <QtGui/QTreeView>

#pragma warning (push)
#pragma warning (disable:4482)	/* nonstandard extension used: enum 'enum' used in qualified name */



int QVGRSnapshotWidget::Snapshot::Serial = 0;

QVGRSnapshotWidget::QVGRSnapshotWidget(QWidget *parent) : QWidget(parent) {

	this->m_snapshots.clear();

	// Widgets.
	this->m_itemModel = new QStandardItemModel();
	this->m_itemModel->setHorizontalHeaderLabels(QStringList() << tr("Name") << tr("Stamp"));
	this->m_treeView = new QTreeView();
	this->m_treeView->setUniformRowHeights(false);
	this->m_treeView->setRootIsDecorated(false);
	this->m_treeView->setModel(this->m_itemModel);
	this->m_treeView->setAlternatingRowColors(true);
	this->m_treeView->setEditTriggers(QAbstractItemView::EditTrigger::NoEditTriggers);
	this->m_treeView->setColumnWidth(0, 150);
	this->m_previewLabel = new QLabel();
	this->m_previewLabel->setSizePolicy(QSizePolicy(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Fixed));
	this->m_previewLabel->setFixedSize(200, 150);
	this->m_clearPushButton = new QPushButton(tr("Clear"));
	this->m_removePushButton = new QPushButton(tr("Remove"));
	this->m_removePushButton->setMinimumWidth(70);
	this->m_removePushButton->setSizePolicy(QSizePolicy(QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Fixed));
	this->m_clearPushButton = new QPushButton(tr("Clear"));
	this->m_clearPushButton->setMinimumWidth(70);
	this->m_clearPushButton->setSizePolicy(QSizePolicy(QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Fixed));
	this->m_exportPushButton = new QPushButton(QIcon(QString(":/resource/widget/export.png")), tr("Export"));
	this->m_exportPushButton->setMinimumWidth(90);
	this->m_exportPushButton->setSizePolicy(QSizePolicy(QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Fixed));

	// Layouts.
	QHBoxLayout *previewLayout = new QHBoxLayout();
	previewLayout->addStretch();
	previewLayout->addWidget(this->m_previewLabel);
	previewLayout->addStretch();
	QHBoxLayout *buttonsLayout = new QHBoxLayout();
	buttonsLayout->setContentsMargins(QMargins(5, 5, 5, 5));
	buttonsLayout->addWidget(this->m_removePushButton);
	buttonsLayout->addWidget(this->m_clearPushButton);
	buttonsLayout->addStretch();
	buttonsLayout->addWidget(this->m_exportPushButton);
	QVBoxLayout *layout = new QVBoxLayout();
	layout->setContentsMargins(QMargins(0, 0, 0, 0));
	layout->addWidget(this->m_treeView);
	layout->addLayout(previewLayout);
	layout->addLayout(buttonsLayout);
	this->setLayout(layout);

	// Signals and slots.
	connect(this->m_treeView, SIGNAL(clicked(const QModelIndex &)), this, SLOT(onTreeViewClicked(const QModelIndex &)));
	connect(this->m_removePushButton, SIGNAL(clicked()), this, SLOT(onRemovePushButtonClicked()));
	connect(this->m_clearPushButton, SIGNAL(clicked()), this, SLOT(onClearPushButtonClicked()));
	connect(this->m_exportPushButton, SIGNAL(clicked()), this, SLOT(onExportPushButtonClicked()));
}

void QVGRSnapshotWidget::addSnapshot(QImage image, QString name, QDateTime stamp) {
	this->m_snapshots.append(Snapshot(image, name, stamp));
	QStandardItem *nameItem = new QStandardItem(name);
	nameItem->setEditable(false);
	nameItem->setData(this->m_snapshots.back().id, Qt::ItemDataRole::UserRole);
	QStandardItem *stampItem = new QStandardItem(stamp.toString(QString("hh:mm:ss yyyy-MM-dd")));
	stampItem->setEditable(false);
	this->m_itemModel->invisibleRootItem()->appendRow(QList<QStandardItem *>() << nameItem << stampItem);
}

void QVGRSnapshotWidget::removeSnapshot(int id) {
	for (int i = 0; i < this->m_snapshots.size(); ++i) {
		if (this->m_snapshots[i].id == id) {
			this->m_snapshots.remove(i);
			this->m_itemModel->invisibleRootItem()->removeRow(i);
		}
	}
}

void QVGRSnapshotWidget::onTreeViewClicked(const QModelIndex &index) {
	this->m_previewLabel->setPixmap(QPixmap::fromImage(this->m_snapshots[index.row()].image).scaled(this->m_previewLabel->size(), Qt::AspectRatioMode::IgnoreAspectRatio, Qt::TransformationMode::SmoothTransformation));
}

void QVGRSnapshotWidget::onRemovePushButtonClicked() {
	QVector<int> ids;
	QModelIndexList indices = this->m_treeView->selectionModel()->selectedRows();
	foreach (QModelIndex index, indices) {
		ids.append(index.data(Qt::ItemDataRole::UserRole).toInt());
	}
	for (int i = 0; i < ids.size(); ++i) this->removeSnapshot(ids[i]);
}

void QVGRSnapshotWidget::onClearPushButtonClicked() {
	this->m_snapshots.clear();
	this->m_itemModel->removeRows(0, this->m_itemModel->rowCount());
}

void QVGRSnapshotWidget::onExportPushButtonClicked() {
	QString directory = QFileDialog::getExistingDirectory(this, tr("Export"), QString("."));
	if (!directory.length()) return;

	int width = 0, size = this->m_snapshots.size();
	for (int size = this->m_snapshots.size(); size; ++width, size /= 10);

	for (int i = 0; i < this->m_snapshots.size(); ++i) {
		const Snapshot &snapshot(this->m_snapshots[i]);
		QString fileName = directory + QString("/%1_%2_%3.bmp").arg(i + 1, width, 10, QChar('0')).arg(snapshot.stamp.toString(QString("yyyy-MM-dd hh-mm-ss"))).arg(snapshot.name);
		QPixmap::fromImage(snapshot.image).save(fileName);
	}
}



#pragma warning (pop)
