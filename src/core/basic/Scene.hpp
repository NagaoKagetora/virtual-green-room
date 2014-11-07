#pragma once
#ifndef _VGR_CORE_BASIC_SCENE_HPP
#define _VGR_CORE_BASIC_SCENE_HPP

#include <osg/Camera>
#include <osg/Group>
#include <osg/Switch>
#include <osgGA/GUIEventHandler>
#include <osgGA/TrackballManipulator>
#include <osgViewer/View>
#include <common/global.hpp>
#include <common/3rdparty/osg/GraphicsWindow.hpp>
#include <common/3rdparty/vcglib.hpp>
#include <core/basic/sprite/Hud.hpp>
#include <core/basic/sprite/Text.hpp>
#include <core/basic/sprite/Compass.hpp>
#include <core/basic/Handler.hpp>

using namespace vgr;

#pragma warning (push)
#pragma warning (disable:4482)	/* nonstandard extension used: enum 'enum' used in qualified name */



class Model;
class GraphicsWindow;
class GraphWidget;
class Scene;
class StandardScene;



class Scene {

public:
	class Screen {

	public:
		typedef GraphicsWindow			Window;
		typedef GraphWidget				Widget;
		typedef osg::Camera				Camera;
		typedef osgViewer::View			View;

	private:
		osg::ref_ptr<Window>			m_window;
		Widget							*m_widget;
		osg::ref_ptr<Camera>			m_camera;
		osg::ref_ptr<View>				m_view;

	public:
		Screen();

	public:
		const osg::ref_ptr<Window> &window() const { return (this->m_window); }
		osg::ref_ptr<Window> &window() { return (this->m_window); }
		const Widget *widget() const { return (this->m_widget); }
		Widget *widget() { return (this->m_widget); }
		const osg::ref_ptr<Camera> &camera() const { return (this->m_camera); }
		osg::ref_ptr<Camera> &camera() { return (this->m_camera); }
		const osg::ref_ptr<View> &view() const { return (this->m_view); }
		osg::ref_ptr<View> &view() { return (this->m_view); }

	public:
		void setViewport(int width, int height);
	};

public:
	Screen								m_screen;

public:
	Scene() : m_screen() { }

public:
	const Screen &screen() const { return (this->m_screen); }
	Screen &screen() { return (this->m_screen); }

public:
	int width() const { return (this->m_screen.widget()->width()); }
	int height() const { return (this->m_screen.widget()->height()); }

public:
	virtual int serial() const { return (-1); }
	virtual std::string caption() const { return (std::string()); }
	virtual bool ready() const { return (false); }
	virtual void setViewport(int width, int height) {
		this->screen().setViewport(width, height);
	}
};



class StandardScene : public Scene {

public:
	class Program : public osg::Referenced {

	private:
		osg::ref_ptr<osg::Shader>		m_vertShader;
		osg::ref_ptr<osg::Shader>		m_fragShader;
		osg::ref_ptr<osg::Shader>		m_geomShader;
		osg::ref_ptr<osg::Program>		m_program;

	public:
		Program();

	public:
		const osg::ref_ptr<osg::Program> &program() const { return (this->m_program); }
		osg::ref_ptr<osg::Program> &program() { return (this->m_program); }
	};

	enum NodeMask {
		All = -1,
		None = 0,
		Visible = 0x1,
		Pickable = 0x2
	};

	class Sprite : public osg::Referenced {

	private:
		static int						CurrentSerial;

	private:
		osg::ref_ptr<Text>				m_serialText;
		osg::ref_ptr<Text>				m_nameText;
		osg::ref_ptr<Hud>				m_textHud;
		osg::ref_ptr<Compass>			m_compass;
		osg::ref_ptr<osg::Switch>		m_fixture;
		osg::ref_ptr<osg::Group>		m_indicator;
		osg::ref_ptr<osg::Group>		m_decorator;
		osg::ref_ptr<osg::Group>		m_node;

	public:
		Sprite();
		Sprite(Model *model);

	public:
		const osg::ref_ptr<Compass> &compass() const { return (this->m_compass); }
		osg::ref_ptr<Compass> &compass() { return (this->m_compass); }
		const osg::ref_ptr<osg::Group> &indicator() const { return (this->m_indicator); }
		osg::ref_ptr<osg::Group> &indicator() { return (this->m_indicator); }
		const osg::ref_ptr<osg::Group> &decorator() const { return (this->m_decorator); }
		osg::ref_ptr<osg::Group> &decorator() { return (this->m_decorator); }
		const osg::ref_ptr<osg::Group> &node() const { return (this->m_node); }
		osg::ref_ptr<osg::Group> &node() { return (this->m_node); }

	public:
		void setupModel(Model *model);
		void clearModel();
		void setCaptionEnabled(bool enabled);
		void setCompassEnabled(bool enabled);
		void addIndicator(osg::Node *indicator);
		void addIndicator(const std::string &name, osg::Node *indicator);
		void removeIndicator(osg::Node *indicator);
		void removeIndicator(const std::string &name);
		void addDecorator(osg::Node *decorator);
		void addDecorator(const std::string &name, osg::Node *decorator);
		void removeDecorator(osg::Node *decorator);
		void removeDecorator(const std::string &name);

	private:
		void init();

	public:
		virtual void setViewport(int width, int height);
	};

	class Content : public osg::Referenced {

	private:
		osg::ref_ptr<osg::Group>		m_vertex;
		osg::ref_ptr<osg::Group>		m_edge;
		osg::ref_ptr<osg::Group>		m_face;
		osg::ref_ptr<osg::Group>		m_node;

	public:
		Content();
		Content(Model *model);

	public:
		const osg::ref_ptr<osg::Group> &vertex() const { return (this->m_vertex); }
		osg::ref_ptr<osg::Group> &vertex() { return (this->m_vertex); }
		const osg::ref_ptr<osg::Group> &edge() const { return (this->m_edge); }
		osg::ref_ptr<osg::Group> &edge() { return (this->m_edge); }
		const osg::ref_ptr<osg::Group> &face() const { return (this->m_face); }
		osg::ref_ptr<osg::Group> &face() { return (this->m_face); }
		const osg::ref_ptr<osg::Group> &node() const { return (this->m_node); }
		osg::ref_ptr<osg::Group> &node() { return (this->m_node); }

	public:
		void setupModel(Model *model);
		void clearModel();

	private:
		void init();

	public:
		virtual void setViewport(int width, int height) { }
	};

	class Manipulator : public osgGA::TrackballManipulator {

	public:
		enum ProjectionMode {
			Perspective,
			Orthographic
		};

	private:
		ProjectionMode					m_mode;
		real_t							m_fov;
		std::vector<osg::Camera *>		m_cameras;
		bool							m_rotationEnabled;
		bool							m_translationEnabled;
		bool							m_zoomingEnabled;

	public:
		Manipulator() : osgGA::TrackballManipulator(), m_mode(ProjectionMode::Perspective), m_fov(30.0) { this->reset(); }

	public:
		bool mode() const { return (this->m_mode); }
		real_t fov() const { return (this->m_fov); }
		bool rotationEnabled() const { return (this->m_rotationEnabled); }
		void setRotationEnabled(bool value) { this->m_rotationEnabled = value; }
		bool translationEnabled() const { return (this->m_translationEnabled); }
		void setTranslationEnabled(bool value) { this->m_translationEnabled = value; }
		bool zoomingEnabled() const { return (this->m_zoomingEnabled); }
		void setZoomingEnabled(bool value) { this->m_zoomingEnabled = value; }

	public:
		void addCamera(osg::Camera *camera) {
			this->m_cameras.push_back(camera);
		}
		void changeMode(ProjectionMode mode);
		void reset();
		void updateOrthography();
		void updatePerspective();
		void fitScene(const StandardScene *scene);

	public:
		virtual void setTransformation(const osg::Vec3d &eye, const osg::Quat &rotation);
		virtual void setTransformation(const osg::Vec3d &eye, const osg::Vec3d &center, const osg::Vec3d &up);

	protected:
		virtual bool handle(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &us);
	};

private:
	static int							CurrentSerial;

private:
	int									m_serial;
	std::string							m_caption;
	bool								m_ready;
	Model								*m_model;
	osg::ref_ptr<Program>				m_program;
	osg::ref_ptr<Sprite>				m_sprite;
	osg::ref_ptr<Content>				m_content;
	osg::ref_ptr<osg::Group>			m_root;

public:
	static osg::ref_ptr<Manipulator> sharedManipulator() {
		static osg::ref_ptr<Manipulator> manipulator = new Manipulator();
		return (manipulator);
	}

public:
	StandardScene();

public:
	const Model *model() const { return (this->m_model); }
	Model *model() { return (this->m_model); }
	const osg::ref_ptr<Sprite> &sprite() const { return (this->m_sprite); }
	osg::ref_ptr<Sprite> &sprite() { return (this->m_sprite); }
	const osg::ref_ptr<Content> &content() const { return (this->m_content); }
	osg::ref_ptr<Content> &content() { return (this->m_content); }
	const osg::ref_ptr<osg::Group> &root() const { return (this->m_root); }
	osg::ref_ptr<osg::Group> &root() { return (this->m_root); }

public:
	void setupModel(Model *model);
	void clearModel();
	void addHandler(Handler *handler);
	void removeHandler(Handler *handler);

public:
	virtual int serial() const { return (this->m_serial); }
	virtual std::string caption() const { return (this->m_caption); }
	virtual bool ready() const { return (this->m_ready); }
	virtual void setViewport(int width, int height);
};



#pragma warning (pop)

#endif /* _VGR_CORE_BASIC_SCENE_HPP */
