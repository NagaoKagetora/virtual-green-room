#include "QVGRLoggerWidget.h"
#include <QtGui/QApplication>
#include <QtGui/QClipboard>
#include <QtGui/QComboBox>
#include <QtGui/QIcon>
#include <QtGui/QLabel>
#include <QtGui/QLayout>
#include <QtGui/QPixmap>
#include <QtGui/QPushButton>
#include <QtGui/QSortFilterProxyModel>
#include <QtGui/QTreeView>

#pragma warning (push)
#pragma warning (disable:4482)	/* nonstandard extension used: enum 'enum' used in qualified name */



void QVGRLogManager::append(const Item &item) {
	while (this->m_loggingMutex.tryLock());
	int index = this->m_itemModel->rowCount();
	this->m_logSignal->beginLogging(index);

	this->m_itemModel->insertRow(index);
	this->m_itemModel->setData(this->m_itemModel->index(index, 0), QString(item.sender().c_str()));
	this->m_itemModel->setData(this->m_itemModel->index(index, 1), QString(item.content().c_str()));
	switch (item.level()) {
		case Item::Level::Message:
			this->m_itemModel->setData(this->m_itemModel->index(index, 0), QString("MSG"), Qt::ItemDataRole::UserRole);
			break;
		case Item::Level::Warning:
			this->m_itemModel->setData(this->m_itemModel->index(index, 0), QString("WRN"), Qt::ItemDataRole::UserRole);
			break;
		case Item::Level::Error:
			this->m_itemModel->setData(this->m_itemModel->index(index, 0), QString("ERR"), Qt::ItemDataRole::UserRole);
			break;
	}
	this->m_logSignal->endLogging(index);
	this->m_loggingMutex.unlock();
}



void QVGRLoggerAnimationThread::run() {
	for (int i = 0; i < this->m_loopsCount; ++i) {
		emit blink(true);
		this->msleep(this->m_interval);
		emit blink(false);
		this->msleep(this->m_interval);
	}
}



QVGRLoggerWidget::QVGRLoggerWidget(QWidget *parent) : QDialog(parent) {

	// Widgets.
	this->m_filterComboBox = new QComboBox();
	this->m_filterComboBox->addItem(QIcon(QString(":/resource/widget/logger_view_all.png")), tr("All"), QString());
	this->m_filterComboBox->addItem(QIcon(QString(":/resource/widget/logger_view_message.png")), tr("Messages"), QString("MSG"));
	this->m_filterComboBox->addItem(QIcon(QString(":/resource/widget/logger_view_warning.png")), tr("Warnings"), QString("WRN"));
	this->m_filterComboBox->addItem(QIcon(QString(":/resource/widget/logger_view_error.png")), tr("Errors"), QString("ERR"));
	this->m_clearPushButton = new QPushButton(QIcon(QString(":/resource/widget/logger_button_clear.png")), tr(""));
	this->m_clearPushButton->setSizePolicy(QSizePolicy(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Fixed));
	this->m_clearPushButton->setFlat(true);
	this->m_copyPushButton = new QPushButton(QIcon(QString(":/resource/widget/logger_button_copy.png")), tr(""));
	this->m_copyPushButton->setSizePolicy(QSizePolicy(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Fixed));
	this->m_logSignal = new QVGRLogSignal();
	this->m_progressBar = new QProgressBar();
	this->m_progressBar->setVisible(false);
	this->m_copyPushButton->setFlat(true);
	this->m_itemModel = new QStandardItemModel();
	this->m_itemModel->setHorizontalHeaderLabels(QStringList() << tr("Sender") << tr("Information"));
	this->m_proxyModel = new QSortFilterProxyModel();
	this->m_proxyModel->setFilterRole(Qt::ItemDataRole::UserRole);
	this->m_proxyModel->setFilterKeyColumn(0);
	this->m_proxyModel->setSourceModel(this->m_itemModel);
	this->m_treeView = new QTreeView();
	this->m_treeView->setModel(this->m_proxyModel);
	this->m_treeView->setHeaderHidden(true);
	this->m_treeView->setAlternatingRowColors(true);
	this->m_treeView->setRootIsDecorated(false);
	this->m_treeView->setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOn);
	this->m_treeView->setVerticalScrollMode(QAbstractItemView::ScrollMode::ScrollPerPixel);
	this->m_treeView->setEditTriggers(QAbstractItemView::EditTrigger::NoEditTriggers);
	this->m_statusIconLabel = new QLabel();
	this->m_statusIconLabel->setFixedSize(16, 16);
	this->m_statusTextLabel = new QLabel();
	this->m_statusTextLabel->setFixedWidth(800);
	this->m_statusWidget = new QWidget();
	this->m_statusProgressBars.resize(QThread::idealThreadCount());
	for (int i = 0; i < this->m_statusProgressBars.size(); ++i) {
		this->m_statusProgressBars[i] = new QProgressBar();
		this->m_statusProgressBars[i]->setVisible(false);
		this->m_statusProgressBars[i]->setFixedHeight(16);
	}
	this->m_statusPermanentWidget = new QWidget();
	this->m_animationThread = new QVGRLoggerAnimationThread();
	this->m_animationThread->setInterval(200);
	this->m_animationThread->setLoopsCount(5);
	this->m_hostWidget = NULL;
	this->m_logManager = new QVGRLogManager(this->m_itemModel, this->m_logSignal);

	util::Logger::instance().setLogManager(this->m_logManager);

	// Layouts.
	QHBoxLayout *topLayout = new QHBoxLayout();
	topLayout->setContentsMargins(QMargins(5, 5, 5, 0));
	topLayout->setSpacing(0);
	topLayout->addWidget(this->m_filterComboBox);
	topLayout->addSpacing(5);
	topLayout->addWidget(this->m_clearPushButton);
	topLayout->addWidget(this->m_copyPushButton);
	topLayout->addStretch();
	topLayout->addWidget(this->m_progressBar);
	QWidget *topLayoutWidget = new QWidget();
	topLayoutWidget->setLayout(topLayout);
	QHBoxLayout *statusWidgetLayout = new QHBoxLayout();
	statusWidgetLayout->setContentsMargins(0, 0, 0, 0);
	statusWidgetLayout->addWidget(this->m_statusIconLabel);
	statusWidgetLayout->addWidget(this->m_statusTextLabel);
	this->m_statusWidget->setLayout(statusWidgetLayout);
	QHBoxLayout *statusPermanentWidgetLayout = new QHBoxLayout();
	statusPermanentWidgetLayout->setContentsMargins(0, 0, 0, 0);
	for (int i = 0; i < this->m_statusProgressBars.size(); ++i) {
		statusPermanentWidgetLayout->addWidget(this->m_statusProgressBars[i]);
	}
	this->m_statusPermanentWidget->setLayout(statusPermanentWidgetLayout);
	QVBoxLayout *layout = new QVBoxLayout();
	layout->setContentsMargins(QMargins(0, 0, 0, 0));
	layout->addWidget(topLayoutWidget);
	layout->addWidget(this->m_treeView);

	// Main frame.
	this->setFont(QFont(QString("Arial"), 9));
	this->resize(800, 200);
	this->setMinimumHeight(200);
	this->setSizePolicy(QSizePolicy(QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Preferred));
	this->setWindowTitle(tr("Log History"));
	this->setWindowIcon(QIcon(QString(":/resource/logo/logo.png")));
	this->setLayout(layout);

	// Signals and slots.
	connect(this->m_filterComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onFilterComboBoxCurrentIndexChanged(int)));
	connect(this->m_clearPushButton, SIGNAL(clicked()), this, SLOT(onClearPushButtonClicked()));
	connect(this->m_copyPushButton, SIGNAL(clicked()), this, SLOT(onCopyPushButtonClicked()));
	connect(this->m_logSignal, SIGNAL(loggingStarted(int)), this, SLOT(onLogSignalLoggingStarted(int)));
	connect(this->m_logSignal, SIGNAL(loggingStopped(int)), this, SLOT(onLogSignalLoggingStopped(int)));
	connect(this->m_logSignal, SIGNAL(progressStarted(int)), this, SLOT(onLogSignalProgressStarted(int)));
	connect(this->m_logSignal, SIGNAL(progressStopped(int)), this, SLOT(onLogSignalProgressStopped(int)));
	connect(this->m_logSignal, SIGNAL(progressChanged(int, qreal)), this, SLOT(onLogSignalProgressChanged(int, qreal)));
	connect(this->m_itemModel, SIGNAL(rowsInserted(const QModelIndex &, int, int)), this, SLOT(onItemModelRowsInserted(const QModelIndex &, int, int)));
	connect(this->m_animationThread, SIGNAL(blink(bool)), this, SLOT(onAnimationBlink(bool)));
}

void QVGRLoggerWidget::blink() {
	if (!this->m_animationThread->isFinished()) this->m_animationThread->terminate();
	this->m_animationThread->start();
}

void QVGRLoggerWidget::onFilterComboBoxCurrentIndexChanged(int index) {
	this->m_proxyModel->setFilterRegExp(QRegExp(
			this->m_filterComboBox->itemData(index).toString(),
			Qt::CaseSensitivity::CaseSensitive,
			QRegExp::PatternSyntax::RegExp));
}

void QVGRLoggerWidget::onClearPushButtonClicked() {
	while (this->m_logManager->LoggingMutex().tryLock());
	this->m_itemModel->removeRows(0, this->m_itemModel->rowCount());
	this->m_logManager->LoggingMutex().unlock();
}

void QVGRLoggerWidget::onCopyPushButtonClicked() {
	while (this->m_logManager->LoggingMutex().tryLock());
	QStringList information;
	for (int i = 0; i < this->m_itemModel->rowCount(); ++i) {
		information.append(QString("[%1] <%2> %3").arg(
				this->m_itemModel->data(this->m_itemModel->index(i, 0), Qt::ItemDataRole::UserRole).toString(),
				this->m_itemModel->data(this->m_itemModel->index(i, 0)).toString(),
				this->m_itemModel->data(this->m_itemModel->index(i, 1)).toString()));
	}
	QApplication::clipboard()->setText(information.join(QString("\n")));
	this->m_logManager->LoggingMutex().unlock();
}

void QVGRLoggerWidget::onLogSignalLoggingStarted(int index) { }

void QVGRLoggerWidget::onLogSignalLoggingStopped(int index) {
	QString type = this->m_itemModel->data(this->m_itemModel->index(index, 0), Qt::ItemDataRole::UserRole).toString();
	this->m_statusTextLabel->setText(QString("<%1> %2").arg(
			this->m_itemModel->data(this->m_itemModel->index(index, 0)).toString(),
			this->m_itemModel->data(this->m_itemModel->index(index, 1)).toString()));
	if (type == QString("MSG")) {
		this->m_itemModel->setData(this->m_itemModel->index(index, 0), QIcon(QString(":/resource/widget/logger_view_message.png")), Qt::ItemDataRole::DecorationRole);
		this->m_statusIconLabel->setPixmap(QPixmap(QString(":/resource/widget/logger_view_message.png")).scaled(QSize(16, 16), Qt::AspectRatioMode::KeepAspectRatio, Qt::TransformationMode::SmoothTransformation));
	}
	if (type == QString("WRN")) {
		this->m_itemModel->setData(this->m_itemModel->index(index, 0), QIcon(QString(":/resource/widget/logger_view_warning.png")), Qt::ItemDataRole::DecorationRole);
		this->m_statusIconLabel->setPixmap(QPixmap(QString(":/resource/widget/logger_view_warning.png")).scaled(QSize(16, 16), Qt::AspectRatioMode::KeepAspectRatio, Qt::TransformationMode::SmoothTransformation));
	}
	if (type == QString("ERR")) {
		this->m_itemModel->setData(this->m_itemModel->index(index, 0), QIcon(QString(":/resource/widget/logger_view_error.png")), Qt::ItemDataRole::DecorationRole);
		this->m_statusIconLabel->setPixmap(QPixmap(QString(":/resource/widget/logger_view_error.png")).scaled(QSize(16, 16), Qt::AspectRatioMode::KeepAspectRatio, Qt::TransformationMode::SmoothTransformation));
	}
}

void QVGRLoggerWidget::onLogSignalProgressStarted(int thread) {
	//this->m_progressBar->setVisible(true);
	this->m_statusProgressBars[thread]->setVisible(true);
}

void QVGRLoggerWidget::onLogSignalProgressStopped(int thread) {
	//this->m_progressBar->setVisible(false);
	this->m_statusProgressBars[thread]->setVisible(false);
}

void QVGRLoggerWidget::onLogSignalProgressChanged(int thread, qreal progress) {
	if (progress < 0) {
		//this->m_progressBar->setMinimum(0);
		//this->m_progressBar->setMaximum(0);
		this->m_statusProgressBars[thread]->setMinimum(0);
		this->m_statusProgressBars[thread]->setMaximum(0);
	} else {
		//this->m_progressBar->setMinimum(0);
		//this->m_progressBar->setMaximum(100);
		//this->m_progressBar->setValue(100 * progress);
		this->m_statusProgressBars[thread]->setMinimum(0);
		this->m_statusProgressBars[thread]->setMaximum(100);
		this->m_statusProgressBars[thread]->setValue(100 * progress);
	}
}

void QVGRLoggerWidget::onItemModelRowsInserted(const QModelIndex &index, int start, int end) {
	this->m_treeView->scrollToBottom();
}

void QVGRLoggerWidget::onAnimationBlink(bool on) {
	if (this->m_hostWidget) {
		if (on) {
			this->m_hostWidgetStyleSheet = this->m_hostWidget->styleSheet();
			QString type = this->m_itemModel->data(this->m_itemModel->index(this->m_itemModel->rowCount() - 1, 0), Qt::ItemDataRole::UserRole).toString();
			if (type == QString("MSG")) this->m_hostWidget->setStyleSheet(QString("background-color: rgb(0, 255, 0);"));
			if (type == QString("WRN")) this->m_hostWidget->setStyleSheet(QString("background-color: rgb(255, 255, 0);"));
			if (type == QString("ERR")) this->m_hostWidget->setStyleSheet(QString("background-color: rgb(255, 0, 0);"));
		} else {
			this->m_hostWidget->setStyleSheet(this->m_hostWidgetStyleSheet);
		}
	}
}



#pragma warning (pop)
