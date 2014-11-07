#ifndef _VGR_GUI_QVGRCREATEPROJECTDIALOG_H
#define _VGR_GUI_QVGRCREATEPROJECTDIALOG_H

#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtGui/QDialog>



class QLabel;
class QLineEdit;
class QListWidget;
class QPushButton;
class QVGRCreateProjectDialog;



class QVGRCreateProjectDialog : public QDialog {

	Q_OBJECT

private:
	QLabel								*m_projectNameLabel;
	QLineEdit							*m_projectNameLineEdit;
	QListWidget							*m_fileNamesListWidget;
	QPushButton							*m_addPushButton;
	QPushButton							*m_removePushButton;
	QPushButton							*m_clearPushButton;
	QPushButton							*m_upPushButton;
	QPushButton							*m_downPushButton;
	QPushButton							*m_confirmPushButton;
	QPushButton							*m_cancelPushButton;

public:
	QVGRCreateProjectDialog(QWidget *parent, Qt::WFlags flags = 0);
	~QVGRCreateProjectDialog() { }

public:
	QString projectName() const;
	QStringList fileNames() const;

private slots:
	void onFileNamesListWidgetCurrentRowChanged(int index);
	void onAddPushButtonClicked();
	void onRemovePushButtonClicked();
	void onClearPushButtonClicked();
	void onUpPushButtonClicked();
	void onDownPushButtonClicked();
};



#endif /* _VGR_GUI_QVGRCREATEPROJECTDIALOG_H */
