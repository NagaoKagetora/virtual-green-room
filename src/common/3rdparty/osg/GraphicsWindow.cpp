/* -*-c++-*- OpenSceneGraph - Copyright (C) 2009 Wang Rui
 *
 * This library is open source and may be redistributed and/or modified under
 * the terms of the OpenSceneGraph Public License (OSGPL) version 0.0 or
 * (at your option) any later version.  The full license is in LICENSE file
 * included with this distribution, and on the openscenegraph.org website.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * OpenSceneGraph Public License for more details.
*/

#include "GraphicsWindow.hpp"
#include <QtCore/QUrl>
#include <QtGui/QInputEvent>

#pragma warning (push)
#pragma warning (disable:4482)	/* nonstandard extension used: enum 'enum' used in qualified name */



static const QString MIME_DATA("application/x-qabstractitemmodeldatalist");



GraphWidget::GraphWidget(const QGLFormat &format, QWidget *parent, const QGLWidget *shareWidget, Qt::WindowFlags f) : QGLWidget(format, parent, shareWidget, f) {
	this->setAutoBufferSwap(false);
	this->setMouseTracking(true);
	this->setAcceptDrops(true);

	this->m_window = NULL;
	this->m_viewer = NULL;
	this->m_widget = NULL;
	this->m_resizeCallback = NULL;
	this->m_keyPressCallback = NULL;
	this->m_keyReleaseCallback = NULL;
	this->m_mousePressCallback = NULL;
	this->m_mouseReleaseCallback = NULL;
	this->m_mouseDoubleClickCallback = NULL;
	this->m_mouseMoveCallback = NULL;
	this->m_wheelCallback = NULL;
}

void GraphWidget::resizeEvent(QResizeEvent *event) {
	const QSize &size = event->size();
	this->m_window->getEventQueue()->windowResize(0, 0, size.width(), size.height());
	this->m_window->resized(0, 0, size.width(), size.height());
	if (this->m_resizeCallback) this->m_resizeCallback->operator ()(event);
	this->refresh();
}

void GraphWidget::keyPressEvent(QKeyEvent *event) {
	this->setKeyboardModifiers(event);
	this->m_window->getEventQueue()->keyPress((osgGA::GUIEventAdapter::KeySymbol)(event->text().toAscii().data()[0]));
	if (this->m_keyPressCallback) this->m_keyPressCallback->operator ()(event);
	this->refresh();
}

void GraphWidget::keyReleaseEvent(QKeyEvent *event) {
	this->setKeyboardModifiers(event);
	this->m_window->getEventQueue()->keyRelease((osgGA::GUIEventAdapter::KeySymbol)(event->text().toAscii().data()[0]));
	if (this->m_keyReleaseCallback) this->m_keyReleaseCallback->operator ()(event);
	this->refresh();
}

void GraphWidget::mousePressEvent(QMouseEvent *event) {
	this->setKeyboardModifiers(event);
	this->m_window->getEventQueue()->mouseButtonPress(event->x(), event->y(), this->setMouseButtons(event));
	if (this->m_mousePressCallback) this->m_mousePressCallback->operator ()(event);
	this->refresh();
}

void GraphWidget::mouseReleaseEvent(QMouseEvent *event) {
	this->setKeyboardModifiers(event);
	this->m_window->getEventQueue()->mouseButtonRelease(event->x(), event->y(), this->setMouseButtons(event));
	if (this->m_mouseReleaseCallback) this->m_mouseReleaseCallback->operator ()(event);
	this->refresh();
}

void GraphWidget::mouseDoubleClickEvent(QMouseEvent *event) {
	this->setKeyboardModifiers(event);
	this->m_window->getEventQueue()->mouseDoubleButtonPress(event->x(), event->y(), this->setMouseButtons(event));
	if (this->m_mouseDoubleClickCallback) this->m_mouseDoubleClickCallback->operator ()(event);
	this->refresh();
}

void GraphWidget::mouseMoveEvent(QMouseEvent *event) {
	this->setKeyboardModifiers(event);
	this->m_window->getEventQueue()->mouseMotion(event->x(), event->y());
	if (this->m_mouseMoveCallback) this->m_mouseMoveCallback->operator ()(event);
	this->refresh();
}

void GraphWidget::wheelEvent(QWheelEvent *event) {
	this->setKeyboardModifiers(event);
	if (event->delta() > 0) this->m_window->getEventQueue()->mouseScroll(osgGA::GUIEventAdapter::ScrollingMotion::SCROLL_UP);
	else this->m_window->getEventQueue()->mouseScroll(osgGA::GUIEventAdapter::ScrollingMotion::SCROLL_DOWN);
	if (this->m_wheelCallback) this->m_wheelCallback->operator ()(event);
	this->refresh();
}

void GraphWidget::dragEnterEvent(QDragEnterEvent *event) {
	event->accept();
}

void GraphWidget::dragLeaveEvent(QDragLeaveEvent *event) {
	event->accept();
}

void GraphWidget::dropEvent(QDropEvent *event) {
	QString format(event->format());
	if (format == MIME_DATA) {
		QByteArray data = event->mimeData()->data(MIME_DATA);
		QDataStream stream(&data, QIODevice::ReadOnly);
		int row = 0;
		int col = 0;
		QMap<int,  QVariant> variants;
		stream >> row >> col >> variants;
		if (variants.contains(Qt::ItemDataRole::UserRole)) {
			emit itemDropped(this, variants[Qt::ItemDataRole::UserRole].toInt());
			event->accept();
		}
	} else {
		if (event->mimeData() && event->mimeData()->hasUrls()) {
			QStringList fileNames;
			foreach (QUrl url, event->mimeData()->urls()) fileNames << url.toLocalFile();
			emit filesDropped(fileNames);
			event->accept();
		}
	}
}

void GraphWidget::setKeyboardModifiers(QInputEvent *event) {
	int mask = 0;
	if (event->modifiers() & Qt::KeyboardModifier::ShiftModifier)		mask |= osgGA::GUIEventAdapter::ModKeyMask::MODKEY_SHIFT;
	if (event->modifiers() & Qt::KeyboardModifier::ControlModifier)		mask |= osgGA::GUIEventAdapter::ModKeyMask::MODKEY_CTRL;
	if (event->modifiers() & Qt::KeyboardModifier::AltModifier)			mask |= osgGA::GUIEventAdapter::ModKeyMask::MODKEY_ALT;
	this->m_window->getEventQueue()->getCurrentEventState()->setModKeyMask(mask);
}

int GraphWidget::setMouseButtons(QMouseEvent *event) {
	switch (event->button()) {
	case Qt::MouseButton::LeftButton:	return (1);
	case Qt::MouseButton::MidButton:	return (2);
	case Qt::MouseButton::RightButton:	return (3);
	case Qt::MouseButton::NoButton:		return (0);
	}
	return (0);
}



GraphicsWindow::GraphicsWindow(osg::GraphicsContext::Traits *traits) : m_widget(NULL), m_initialized(false), m_realized(false) {

	this->setTraits(traits);
	this->m_initialized = init();

	if (this->valid()) {
		this->setState(new osg::State());
		this->getState()->setGraphicsContext(this);
		if (this->traits() && this->traits()->sharedContext) {
			this->getState()->setContextID(this->traits()->sharedContext->getState()->getContextID());
			this->incrementContextIDUsageCount(this->getState()->getContextID());
		} else {
			this->getState()->setContextID(osg::GraphicsContext::createNewContextID());
		}
	}
}

GraphicsWindow::~GraphicsWindow() {
	this->close();
}

bool GraphicsWindow::init() {
	QGLFormat format(QGLFormat::defaultFormat());
	format.setAlphaBufferSize(this->traits()->alpha);
	format.setRedBufferSize(this->traits()->red);
	format.setGreenBufferSize(this->traits()->green);
	format.setBlueBufferSize(this->traits()->blue);
	format.setDepthBufferSize(this->traits()->depth);
	format.setStencilBufferSize(this->traits()->stencil);
	format.setSampleBuffers(this->traits()->sampleBuffers);
	format.setSamples(this->traits()->samples);
	format.setAlpha(this->traits()->alpha > 0);
	format.setDepth(this->traits()->depth > 0);
	format.setStencil(this->traits()->stencil > 0);
	format.setDoubleBuffer(this->traits()->doubleBuffer);
	format.setSwapInterval(this->traits()->vsync ? 1 : 0);
	
	WindowData *windowData = (this->traits()) ? dynamic_cast<WindowData *>(this->traits()->inheritedWindowData.get()) : NULL;
	this->m_widget = windowData ? windowData->widget : NULL;
	if (!this->m_widget) {
		GraphicsWindow *sharedContext = dynamic_cast<GraphicsWindow *>(this->traits()->sharedContext);
		QGLWidget *shareWidget = sharedContext ? sharedContext->graphWidget() : NULL;
		Qt::WindowFlags flags = Qt::WindowType::Window | Qt::WindowType::CustomizeWindowHint;
		if (this->traits()->windowDecoration) {
			flags |= Qt::WindowType::WindowTitleHint | Qt::WindowType::WindowMinMaxButtonsHint | Qt::WindowType::WindowSystemMenuHint;
		}		
		this->m_widget = new GraphWidget(format, 0, shareWidget, flags);
	}
	
	this->m_widget->setWindowTitle(QString::fromStdString(this->traits()->windowName));
	this->m_widget->move(this->traits()->x, this->traits()->y);
	if (!this->traits()->supportsResize) this->m_widget->setFixedSize(this->traits()->width, this->traits()->height);
	else this->m_widget->resize(this->traits()->width, this->traits()->height);
	this->m_widget->setFocusPolicy(Qt::FocusPolicy::WheelFocus);
	this->m_widget->setWindow(this);
	this->useCursor(this->traits()->useCursor);
	return (true);
}

bool GraphicsWindow::setWindowRectangleImplementation(int x, int y, int width, int height) {
	if (!this->m_widget) return (false);
	this->m_widget->setGeometry(x, y, width, height);
	return (true);
}

void GraphicsWindow::getWindowRectangle(int &x, int &y, int &width, int &height) {
	if (!this->m_widget) return;
	const QRect &geometry(this->m_widget->geometry());
	x = geometry.x();
	y = geometry.y();
	width = geometry.width();
	height = geometry.height();
}

bool GraphicsWindow::setWindowDecorationImplementation(bool windowDecoration) {
	if (!this->m_widget) return (false);
	this->traits()->windowDecoration = windowDecoration;
	Qt::WindowFlags flags = Qt::WindowType::Window | Qt::WindowType::CustomizeWindowHint;
	if (this->traits()->windowDecoration) {
		flags |= Qt::WindowType::WindowTitleHint | Qt::WindowType::WindowMinMaxButtonsHint | Qt::WindowType::WindowSystemMenuHint;
	}	

	/// <Todo>
	/// Calling setWindowFlags or reparent widget will recreate the window handle,
	/// which makes QGLContext no longer work.
	/// </Todo>
#if 0
	this->m_widget->setWindowFlags(flags);
#endif
	return (false);
}

bool GraphicsWindow::getWindowDecoration() const {
	return (this->traits()->windowDecoration);
}

void GraphicsWindow::grabFocus() {
	if (!this->m_widget) return;
	this->m_widget->setFocus(Qt::FocusReason::ActiveWindowFocusReason);
}

void GraphicsWindow::grabFocusIfPointerInWindow() {
	if (!this->m_widget) return;
	if (this->m_widget->underMouse()) this->m_widget->setFocus(Qt::FocusReason::ActiveWindowFocusReason);
}

void GraphicsWindow::raiseWindow() {
	if (!this->m_widget) return;
	this->m_widget->raise();
}

void GraphicsWindow::setWindowName(const std::string &name) {
	if (!this->m_widget) return;
	this->m_widget->setWindowTitle(QString::fromStdString(name));
}

std::string GraphicsWindow::getWindowName() {
	if (!this->m_widget) return (std::string());
	return (this->m_widget->windowTitle().toStdString());
}

void GraphicsWindow::useCursor(bool cursorOn) {
	if (!this->m_widget) return;
	this->traits()->useCursor = cursorOn;
	if (!cursorOn) this->m_widget->setCursor(Qt::CursorShape::BlankCursor);
	else this->m_widget->setCursor(this->m_cursor);
}

void GraphicsWindow::setCursor(MouseCursor cursor) {
	if (!this->m_widget) return;

	if (cursor == MouseCursor::InheritCursor) this->m_widget->unsetCursor();

	switch (cursor) {
	case MouseCursor::NoCursor:				this->m_cursor = Qt::CursorShape::BlankCursor; break;
	case MouseCursor::RightArrowCursor:
	case MouseCursor::LeftArrowCursor:		this->m_cursor = Qt::CursorShape::ArrowCursor; break;
	case MouseCursor::InfoCursor:			this->m_cursor = Qt::CursorShape::SizeAllCursor; break;
	case MouseCursor::DestroyCursor:		this->m_cursor = Qt::CursorShape::ForbiddenCursor; break;
	case MouseCursor::HelpCursor:			this->m_cursor = Qt::CursorShape::WhatsThisCursor; break;
	case MouseCursor::CycleCursor:			this->m_cursor = Qt::CursorShape::ForbiddenCursor; break;
	case MouseCursor::SprayCursor:			this->m_cursor = Qt::CursorShape::SizeAllCursor; break;
	case MouseCursor::WaitCursor:			this->m_cursor = Qt::CursorShape::WaitCursor; break;
	case MouseCursor::TextCursor:			this->m_cursor = Qt::CursorShape::IBeamCursor; break;
	case MouseCursor::CrosshairCursor:		this->m_cursor = Qt::CursorShape::CrossCursor; break;
	case MouseCursor::HandCursor:			this->m_cursor = Qt::CursorShape::OpenHandCursor; break;
	case MouseCursor::UpDownCursor:			this->m_cursor = Qt::CursorShape::SizeVerCursor; break;
	case MouseCursor::LeftRightCursor:		this->m_cursor = Qt::CursorShape::SizeHorCursor; break;
	case MouseCursor::TopSideCursor:
	case MouseCursor::BottomSideCursor:		this->m_cursor = Qt::CursorShape::UpArrowCursor; break;
	case MouseCursor::LeftSideCursor:
	case MouseCursor::RightSideCursor:		this->m_cursor = Qt::CursorShape::SizeHorCursor; break;
	case MouseCursor::TopLeftCorner:		this->m_cursor = Qt::CursorShape::SizeBDiagCursor; break;
	case MouseCursor::TopRightCorner:		this->m_cursor = Qt::CursorShape::SizeFDiagCursor; break;
	case MouseCursor::BottomRightCorner:	this->m_cursor = Qt::CursorShape::SizeBDiagCursor; break;
	case MouseCursor::BottomLeftCorner:		this->m_cursor = Qt::CursorShape::SizeFDiagCursor; break;
	default:								break;
	};
	this->m_widget->setCursor(this->m_cursor);
}

bool GraphicsWindow::valid() const {
	return (this->m_widget && this->m_widget->isValid());
}

bool GraphicsWindow::realizeImplementation() {
	if (!this->m_initialized) {
		if (!(this->m_initialized = this->init())) return (false);
	}
	if (!this->m_widget) return (false);

	/* A makeCurrent() / doneCurrent() seems to be required for
	 * realizing the context(?) before starting drawing.
	 */
#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
	this->m_widget->makeCurrent();
	this->m_widget->doneCurrent();
	this->m_realized = true;
	return (true);
#else
	this->m_realized = this->m_widget->makeCurrent() && this->m_widget->doneCurrent();
	return (this->m_realized);
#endif
}

bool GraphicsWindow::isRealizedImplementation() const {
	return (this->m_realized);
}

void GraphicsWindow::closeImplementation() {
	if (!this->m_widget) return;
	this->m_widget->close();
}

bool GraphicsWindow::makeCurrentImplementation() {
	if (!this->m_widget) return (false);
#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
	this->m_widget->makeCurrent();
	return (true);
#else
	return (this->m_widget->makeCurrent());
#endif
}

bool GraphicsWindow::releaseContextImplementation() {
	if (!this->m_widget) return (false);
#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
	this->m_widget->doneCurrent();
	return (true);
#else
	return (this->m_widget->doneCurrent());
#endif
}

void GraphicsWindow::swapBuffersImplementation() {
	if (!this->m_widget) return;
	this->m_widget->swapBuffers();
}

void GraphicsWindow::requestWarpPointer(float x, float y) {
	if (!this->m_widget) return;
	QCursor::setPos(this->m_widget->mapToGlobal(QPoint((int)x, (int)y)));
}



#pragma warning (pop)
