#include "QVGRInfoDialog.h"
#include <QtGui/QLabel>
#include <QtGui/QLayout>
#include <QtGui/QPushButton>

#pragma warning (push)
#pragma warning (disable:4482)	/* nonstandard extension used: enum 'enum' used in qualified name */



QVGRInfoDialog::QVGRInfoDialog(QWidget *parent, Qt::WFlags flags) : QDialog(parent, flags | Qt::WindowType::FramelessWindowHint) {

	// Widgets.
	this->m_pictureLabel = new QLabel();
	this->m_pictureLabel->setPixmap(QPixmap(QString(":/resource/misc/splash.png")));
	this->m_contextLabel = new QLabel();
	this->m_contextLabel->setText(QString(
			"VIRTUAL GREEN ROOM (Build 20130601)<br />"
			"Wu Yang (wu.yang@gm.com)<br />"
			"Manufacturing Systems Research Lab<br />"
			"General Motors Research & Development"));
	this->m_okPushButton = new QPushButton(tr("OK"));
	this->m_okPushButton->setMinimumWidth(90);
	this->m_okPushButton->setSizePolicy(QSizePolicy(QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Fixed));

	// Layouts.
	QHBoxLayout *contentsLayout = new QHBoxLayout();
	contentsLayout->setMargin(5);
	contentsLayout->addWidget(this->m_contextLabel);
	contentsLayout->addStretch();
	contentsLayout->addWidget(this->m_okPushButton);
	QVBoxLayout *layout = new QVBoxLayout();
	layout->setMargin(2);
	layout->addStretch();
	layout->addWidget(this->m_pictureLabel);
	layout->addLayout(contentsLayout);

	// Main frame.
	this->setFont(QFont(QString("Arial"), 9));
	this->resize(600, 400);
	this->setMinimumWidth(300);
	this->setSizePolicy(QSizePolicy(QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Preferred));
	this->setWindowTitle(tr("Configuration"));
	this->setWindowIcon(QIcon(QString(":/resource/logo/logo.png")));
	this->setLayout(layout);
	
	// Signals and slots.
	connect(this->m_okPushButton, SIGNAL(clicked()), this, SLOT(accept()));
}



#pragma warning (pop)
