#include "QVGRExplorerWidget.h"
#include <core/basic/Model.hpp>
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



class QVGRExplorerHelper;



class QVGRExplorerHelper : public QObject {

public:
	static QStandardItem *createStandartItem() {
		QStandardItem *item = new QStandardItem();
		item->setEditable(false);
		return (item);
	}
	static QStandardItem *createStandartItem(const QString &text) {
		QStandardItem *item = new QStandardItem(text);
		item->setEditable(false);
		return (item);
	}
	static QStandardItem*createStandartItem(const QIcon &icon, const QString &text) {
		QStandardItem *item = new QStandardItem(icon, text);
		item->setEditable(false);
		return (item);
	}
	static QStandardItem *createFileItem(const QFileInfo &fileInfo) {
		QStandardItem *fileItem = new QStandardItem(QIcon(QString(":/resource/widget/explorer_file.png")), tr("File"));
		fileItem->setEditable(false);
		fileItem->appendRow(QList<QStandardItem *>()
				<< QVGRExplorerHelper::createStandartItem(QIcon(QString(":/resource/widget/explorer_file_name.png")), tr("Name"))
				<< QVGRExplorerHelper::createStandartItem(fileInfo.fileName()));
		fileItem->appendRow(QList<QStandardItem *>()
				<< QVGRExplorerHelper::createStandartItem(QIcon(QString(":/resource/widget/explorer_file_path.png")), tr("Path"))
				<< QVGRExplorerHelper::createStandartItem(fileInfo.canonicalPath()));
		return (fileItem);
	}
	static QStandardItem *createMeshItem(const pcvcg::TriMesh *model) {
		QStandardItem *meshItem = new QStandardItem(QIcon(QString(":/resource/widget/explorer_mesh.png")), tr("Mesh"));
		meshItem->setEditable(false);
		meshItem->appendRow(QList<QStandardItem *>()
				<< QVGRExplorerHelper::createStandartItem(QIcon(QString(":/resource/widget/explorer_mesh_vertex.png")), tr("Vertex"))
				<< QVGRExplorerHelper::createStandartItem(QString::number(model->vn)));
		meshItem->appendRow(QList<QStandardItem *>()
				<< QVGRExplorerHelper::createStandartItem(QIcon(QString(":/resource/widget/explorer_mesh_edge.png")), tr("Edge"))
				<< QVGRExplorerHelper::createStandartItem(QString::number(model->en)));
		meshItem->appendRow(QList<QStandardItem *>()
				<< QVGRExplorerHelper::createStandartItem(QIcon(QString(":/resource/widget/explorer_mesh_face.png")), tr("Face"))
				<< QVGRExplorerHelper::createStandartItem(QString::number(model->fn)));
		return (meshItem);
	}
	static QStandardItem *createGeometryItem(const pcvcg::TriMesh *model) {
		QStandardItem *geometryItem = new QStandardItem(QIcon(QString(":/resource/widget/explorer_geometry.png")), tr("Geometry"));
		geometryItem->setEditable(false);
		geometryItem->appendRow(QList<QStandardItem *>()
				<< QVGRExplorerHelper::createStandartItem(QIcon(QString(":/resource/widget/explorer_geometry_center.png")), tr("Center"))
				<< QVGRExplorerHelper::createStandartItem(QString("(%1,%2,%3)").arg(
						QString::number(model->bbox.Center()[0]),
						QString::number(model->bbox.Center()[1]),
						QString::number(model->bbox.Center()[2]))));
		geometryItem->appendRow(QList<QStandardItem *>()
				<< QVGRExplorerHelper::createStandartItem(QIcon(QString(":/resource/widget/explorer_geometry_box.png")), tr("Box"))
				<< QVGRExplorerHelper::createStandartItem(QString("(%1,%2,%3)").arg(
						QString::number(model->bbox.Dim()[0]),
						QString::number(model->bbox.Dim()[1]),
						QString::number(model->bbox.Dim()[2]))));
		geometryItem->appendRow(QList<QStandardItem *>()
				<< QVGRExplorerHelper::createStandartItem(QIcon(QString(":/resource/widget/explorer_geometry_sphere.png")), tr("Sphere"))
				<< QVGRExplorerHelper::createStandartItem(QString::number(model->bbox.Diag())));
		return (geometryItem);
	}
};



QVGRExplorerWidget::QVGRExplorerWidget(QVGRManager *manager, QWidget *parent) : QWidget(parent), m_manager(manager) {

	this->setAcceptDrops(true);

	// Widgets.
	this->m_itemModel = new QStandardItemModel();
	this->m_itemModel->setHorizontalHeaderLabels(QStringList() << tr("Property") << tr("Value") << tr("Flip") << tr("Share"));
	this->m_treeView = new QTreeView();
	this->m_treeView->setUniformRowHeights(false);
	this->m_treeView->setModel(this->m_itemModel);
	this->m_treeView->setAlternatingRowColors(true);
	this->m_treeView->setExpandsOnDoubleClick(false);
	this->m_treeView->setDragEnabled(true);
	this->m_treeView->setEditTriggers(QAbstractItemView::EditTrigger::DoubleClicked | QAbstractItemView::EditTrigger::CurrentChanged | QAbstractItemView::EditTrigger::SelectedClicked);
	this->m_treeView->setColumnWidth(2, 40);
	this->m_treeView->setColumnWidth(3, 40);
	this->m_exportPushButton = new QPushButton(QIcon(QString(":/resource/widget/export.png")), tr("Export"));
	this->m_exportPushButton->setSizePolicy(QSizePolicy(QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Fixed));
	this->m_exportPushButton->setMinimumWidth(90);

	// Layouts.
	QHBoxLayout *buttonsLayout = new QHBoxLayout();
	buttonsLayout->setContentsMargins(QMargins(5, 5, 5, 5));
	buttonsLayout->addStretch();
	buttonsLayout->addWidget(this->m_exportPushButton);
	QVBoxLayout *layout = new QVBoxLayout();
	layout->setContentsMargins(QMargins(0, 0, 0, 0));
	layout->addWidget(this->m_treeView);
	layout->addLayout(buttonsLayout);
	this->setLayout(layout);

	// Signals and slots.
	connect(this->m_manager, SIGNAL(loadingFinished()), this, SLOT(onManagerLoadingFinished()));
	connect(this->m_itemModel, SIGNAL(itemChanged(QStandardItem *)), this, SLOT(onItemModelItemChanged(QStandardItem *)));
	connect(this->m_exportPushButton, SIGNAL(clicked()), this, SLOT(onExportPushButtonClicked()));
}

bool QVGRExplorerWidget::allowDrags() const {
	return (this->m_treeView->dragEnabled());
}

void QVGRExplorerWidget::setAllowDrags(bool on) {
	this->m_treeView->setDragEnabled(on);
}

void QVGRExplorerWidget::onManagerLoadingFinished() {
	this->m_itemModel->removeRows(0, this->m_itemModel->rowCount());
	for (int i = 0; i < this->m_manager->size(); ++i) {
		int size = this->m_itemModel->rowCount();
		QStandardItem *root = this->m_itemModel->invisibleRootItem();
		const QFileInfo &fileInfo(this->m_manager->item(i)->path());
		const pcvcg::TriMesh *model(this->m_manager->item(i)->mesh());
		QStandardItem *modelItem = new QStandardItem(QIcon(QString(":/resource/widget/explorer_model_deactive.png")), this->m_manager->item(i)->name());
		modelItem->setEditable(false);
		modelItem->setData(i);
		modelItem->appendRow(QList<QStandardItem *>() << QVGRExplorerHelper::createFileItem(fileInfo) << QVGRExplorerHelper::createStandartItem());
		modelItem->appendRow(QList<QStandardItem *>() << QVGRExplorerHelper::createMeshItem(model) << QVGRExplorerHelper::createStandartItem());
		modelItem->appendRow(QList<QStandardItem *>() << QVGRExplorerHelper::createGeometryItem(model) << QVGRExplorerHelper::createStandartItem());
		modelItem->setData(i, Qt::ItemDataRole::UserRole);
		QStandardItem *flippedNormalItem = new QStandardItem();
		flippedNormalItem->setEditable(false);
		flippedNormalItem->setCheckable(true);
		QStandardItem *sharedVertexItem = new QStandardItem();
		sharedVertexItem->setEditable(false);
		sharedVertexItem->setCheckable(true);
		sharedVertexItem->setCheckState(Qt::CheckState::Checked);
		root->appendRow(QList<QStandardItem *>() << modelItem << QVGRExplorerHelper::createStandartItem() << flippedNormalItem << sharedVertexItem);
	}
}

void QVGRExplorerWidget::onItemModelItemChanged(QStandardItem *item) {
	if (!item) return;
	if (item->column() == 2) {
		QStandardItem *modelItem = item->model()->item(item->index().row(), 0);
		bool checked = item->checkState() == Qt::CheckState::Checked;
		int index = modelItem->data(Qt::ItemDataRole::UserRole).toInt();
		this->m_manager->item(index)->setUsingFlippedNormal(checked);
		emit modelChanged(index);
	}
	if (item->column() == 3) {
		QStandardItem *modelItem = item->model()->item(item->index().row(), 0);
		bool checked = item->checkState() == Qt::CheckState::Checked;
		int index = modelItem->data(Qt::ItemDataRole::UserRole).toInt();
		this->m_manager->item(index)->setUsingSharedVertex(checked);
		emit modelChanged(index);
	}
}

void QVGRExplorerWidget::onExportPushButtonClicked() {
	QModelIndex index = this->m_treeView->currentIndex();
	if (!index.isValid()) return;
	while (index.parent() != this->m_treeView->rootIndex()) index = index.parent();
	QStandardItem *item = this->m_itemModel->item(index.row(), index.column());
	if (!item) return;
	Model *model = this->m_manager->item(item->data().toInt());
	if (!model) return;
	QString fileName = QFileDialog::getSaveFileName(this, tr("Save Model"), model->name(),
			(QStringList()
			<< QString("OBJ (*.obj)")
			<< QString("STL (*.stl)")
			<< QString("NAS (*.nas)")).join(QString(";;")));
	if (fileName.length()) model->writeFile(fileName);
}



#pragma warning (pop)
