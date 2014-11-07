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

#pragma once
#ifndef _VGR_COMMON_3RDPARTY_OSG_GRAPHICSWINDOW_HPP
#define _VGR_COMMON_3RDPARTY_OSG_GRAPHICSWINDOW_HPP

#include <osgViewer/GraphicsWindow>
#include <osgViewer/CompositeViewer>
#include <QtGui/QWidget>
#include <QtOpenGL/QGLWidget>

#ifdef min
#undef min
#endif

#ifdef max
#undef max
#endif



class QInputEvent;
class GraphWidget;
class GraphicsWindow;



class GraphWidget : public QGLWidget {

	Q_OBJECT

public:
	template <typename EventType>
	class EventCallback : public osg::Referenced {

	public:
		virtual void operator ()(EventType *event) { }
	};

	typedef EventCallback<QResizeEvent>	ResizeCallback; 
	typedef EventCallback<QKeyEvent>	KeyCallback;
	typedef EventCallback<QMouseEvent>	MouseCallback;
	typedef EventCallback<QWheelEvent>	WheelCallback;

	typedef osgViewer::GraphicsWindow	Window;
	typedef osgViewer::ViewerBase		Viewer;
	typedef QWidget						Widget;

protected:
	Window								*m_window;
	Viewer								*m_viewer;
	Widget								*m_widget;
	osg::ref_ptr<ResizeCallback>		m_resizeCallback;
	osg::ref_ptr<KeyCallback>			m_keyPressCallback;
	osg::ref_ptr<KeyCallback>			m_keyReleaseCallback;
	osg::ref_ptr<MouseCallback>			m_mousePressCallback;
	osg::ref_ptr<MouseCallback>			m_mouseReleaseCallback;
	osg::ref_ptr<MouseCallback>			m_mouseDoubleClickCallback;
	osg::ref_ptr<MouseCallback>			m_mouseMoveCallback;
	osg::ref_ptr<WheelCallback>			m_wheelCallback;

public:
	GraphWidget(const QGLFormat &format, QWidget *parent = 0, const QGLWidget *shareWidget = 0, Qt::WindowFlags f = 0);
	
public:
	inline void setWindow(Window *value) { this->m_window = value; }
	inline void setViewer(Viewer *value) { this->m_viewer = value; }
	inline void setWidget(Widget *value) { this->m_widget = value; }
	inline void setResizeCallback(osg::ref_ptr<ResizeCallback> value) { this->m_resizeCallback = value; }
	inline void setKeyPressCallback(osg::ref_ptr<KeyCallback> value) { this->m_keyPressCallback = value; }
	inline void setKeyReleaseCallback(osg::ref_ptr<KeyCallback> value) { this->m_keyReleaseCallback = value; }
	inline void setMousePressCallback(osg::ref_ptr<MouseCallback> value) { this->m_mousePressCallback = value; }
	inline void setMouseReleaseCallback(osg::ref_ptr<MouseCallback> value) { this->m_mouseReleaseCallback = value; }
	inline void setMouseDoubleClickCallback(osg::ref_ptr<MouseCallback> value) { this->m_mouseDoubleClickCallback = value; }
	inline void setMouseMoveCallback(osg::ref_ptr<MouseCallback> value) { this->m_mouseMoveCallback = value; }
	inline void setWheelCallback(osg::ref_ptr<WheelCallback> value) { this->m_wheelCallback = value; }
	
public:
	virtual void resizeEvent(QResizeEvent *event);
	virtual void keyPressEvent(QKeyEvent *event);
	virtual void keyReleaseEvent(QKeyEvent *event);
	virtual void mousePressEvent(QMouseEvent *event);
	virtual void mouseReleaseEvent(QMouseEvent *event);
	virtual void mouseDoubleClickEvent(QMouseEvent *event);
	virtual void mouseMoveEvent(QMouseEvent *event);
	virtual void wheelEvent(QWheelEvent *event);
	virtual void dragEnterEvent(QDragEnterEvent *event);
	virtual void dragLeaveEvent(QDragLeaveEvent *event);
	virtual void dropEvent(QDropEvent *event);
	
private:
	void setKeyboardModifiers(QInputEvent *event);
	int setMouseButtons(QMouseEvent *event);
	inline void refresh() {
		if (this->m_viewer) this->m_viewer->frame();
		if (this->m_widget) this->m_widget->update();
	}

signals:
	void itemDropped(QWidget *widget, int index);
	void filesDropped(const QStringList &fileNames);
};



class GraphicsWindow : public osgViewer::GraphicsWindow {

public:
	struct WindowData : public osg::Referenced {
		GraphWidget						*widget;
	};

protected:
	GraphWidget							*m_widget;
	QCursor								m_cursor;
	bool								m_initialized;
	bool								m_realized;

public:
	GraphicsWindow(osg::GraphicsContext::Traits *traits);
	virtual ~GraphicsWindow();

public:
	inline const GraphWidget *graphWidget() const { return (this->m_widget); }
	inline GraphWidget *graphWidget() { return (this->m_widget); }

private:
	inline const osg::ref_ptr<Traits> &traits() const { return (this->_traits); }
	inline osg::ref_ptr<Traits> &traits() { return (this->_traits); }
	inline void setTraits(osg::ref_ptr<Traits> value) { this->_traits = value; }
	
public:
	bool init();
	
public:
	virtual bool setWindowRectangleImplementation(int x, int y, int width, int height);
	virtual void getWindowRectangle(int &x, int &y, int &width, int &height);
	virtual bool setWindowDecorationImplementation(bool windowDecoration);
	virtual bool getWindowDecoration() const;
	virtual void grabFocus();
	virtual void grabFocusIfPointerInWindow();
	virtual void raiseWindow();
	virtual void setWindowName(const std::string &name);
	virtual std::string getWindowName();
	virtual void useCursor(bool cursorOn);
	virtual void setCursor(MouseCursor cursor);
	virtual bool valid() const;
	virtual bool realizeImplementation();
	virtual bool isRealizedImplementation() const;
	virtual void closeImplementation();
	virtual bool makeCurrentImplementation();
	virtual bool releaseContextImplementation();
	virtual void swapBuffersImplementation();
	virtual void requestWarpPointer(float x, float y);
};



#endif /* _VGR_COMMON_3RDPARTY_OSG_GRAPHICSWINDOW_HPP */
