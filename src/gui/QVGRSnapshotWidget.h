#ifndef _VGR_GUI_QVGRSNAPSHOTWIDGET_H
#define _VGR_GUI_QVGRSNAPSHOTWIDGET_H

#include <QtCore/QDateTime>
#include <QtCore/QVector>
#include <QtGui/QWidget>



class QLabel;
class QModelIndex;
class QPushButton;
class QStandardItem;
class QStandardItemModel;
class QTreeView;
class QVGRManager;
class QVGRSnapshotWidget;



class QVGRSnapshotWidget : public QWidget {

	Q_OBJECT

private:
	struct Snapshot {

	public:
		int								id;
		QImage							image;
		QString							name;
		QDateTime						stamp;

	private:
		static int						Serial;

	public:
		Snapshot() : id(-1), image(), name(), stamp() { }
		Snapshot(const Snapshot &snapshot) : id(snapshot.id), image(snapshot.image), name(snapshot.name), stamp(snapshot.stamp) { }
		Snapshot(const QImage &image, const QString &name, const QDateTime &stamp) : id(++Snapshot::Serial), image(image), name(name), stamp(stamp) { }
	};

private:
	QVector<Snapshot>					m_snapshots;
	QStandardItemModel					*m_itemModel;
	QTreeView							*m_treeView;
	QLabel								*m_previewLabel;
	QPushButton							*m_removePushButton;
	QPushButton							*m_clearPushButton;
	QPushButton							*m_exportPushButton;

public:
	QVGRSnapshotWidget(QWidget *parent);
	virtual ~QVGRSnapshotWidget() { }

public:
	void addSnapshot(QImage image, QString name, QDateTime stamp = QDateTime::currentDateTime());

private:
	void removeSnapshot(int index);

private slots:
	void onTreeViewClicked(const QModelIndex &index);
	void onRemovePushButtonClicked();
	void onClearPushButtonClicked();
	void onExportPushButtonClicked();
};



#endif /* _VGR_GUI_QVGRSNAPSHOTWIDGET_H */
