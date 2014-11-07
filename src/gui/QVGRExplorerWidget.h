#ifndef _VGR_GUI_QVGREXPLORERWIDGET_H
#define _VGR_GUI_QVGREXPLORERWIDGET_H

#include <QtGui/QWidget>



class Model;
class QModelIndex;
class QPushButton;
class QStandardItem;
class QStandardItemModel;
class QTreeView;
class QVGRManager;
class QVGRExplorerWidget;



class QVGRExplorerWidget : public QWidget {

	Q_OBJECT

private:
	QVGRManager							*m_manager;
	QStandardItemModel					*m_itemModel;
	QTreeView							*m_treeView;
	QPushButton							*m_exportPushButton;

public:
	QVGRExplorerWidget(QVGRManager *manager, QWidget *parent = 0);

public:
	bool allowDrags() const;
	void setAllowDrags(bool on);

private slots:
	void onManagerLoadingFinished();
	void onItemModelItemChanged(QStandardItem *item);
	void onExportPushButtonClicked();

signals:
	void modelChanged(int index);
};




#endif /* _VGR_GUI_QVGREXPLORERWIDGET_H */
