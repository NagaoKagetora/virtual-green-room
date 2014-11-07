#ifndef _VGR_GUI_QVGRINFODIALOG_H
#define _VGR_GUI_QVGRINFODIALOG_H

#include <QtGui/QDialog>



class QLabel;
class QPushButton;
class QVGRInfoDialog;



class QVGRInfoDialog : public QDialog {

	Q_OBJECT

private:
	QLabel								*m_pictureLabel;
	QLabel								*m_contextLabel;
	QPushButton							*m_confirmPushButton;
	QPushButton							*m_okPushButton;

public:
	QVGRInfoDialog(QWidget *parent, Qt::WFlags flags = 0);
	virtual ~QVGRInfoDialog() { }
};



#endif /* _VGR_GUI_QVGRINFODIALOG_H */
