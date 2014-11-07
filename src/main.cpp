#include <osgDB/Registry>
#include <QtCore/QFile>
#include <QtCore/QLibraryInfo>
#include <QtGui/QApplication>
#include <QtGui/QSplashScreen>
#include <QtOpenGL/QGLFormat>
#include <gui/QVGRMainWindow.h>
#include <common/global.hpp>



int main(int argc, char *argv[]) {

	////////////////////////////////////////////////////////////////////////////////////////////////////
	// Anti-alias
	// http://lists.openscenegraph.org/pipermail/osg-users-openscenegraph.org/2009-November/034806.html
	QGLFormat format;
	format.setSampleBuffers(true);
	format.setSamples(16);
	QGLFormat::setDefaultFormat(format);

	//osg::ref_ptr<osg::DisplaySettings> settings = osg::DisplaySettings::instance();
	//settings->setNumMultiSamples(16);

	////////////////////////////////////////////////////////////////////////////////////////////////////
	// Change cursor with object under mouse
	// http://forum.openscenegraph.org/viewtopic.php?t=2507
	osgDB::Registry::instance()->setBuildKdTreesHint(osgDB::ReaderWriter::Options::BUILD_KDTREES);
	osgDB::Registry::instance()->setLibraryFilePathList("./osgplugins");

	QApplication a(argc, argv);

	QCoreApplication::addLibraryPath(QString("./qtplugins"));

	QSplashScreen *splash = new QSplashScreen;
	splash->setPixmap(QPixmap(QString(":/resource/misc/splash.png")));
	splash->show();

	QVGRCentralWindow w;
	QFile qss(QString("./runtime/style/stylesheet.qss"));
	qss.open(QFile::ReadOnly);
	if (qss.isOpen()) {
		w.setStyleSheet(qss.readAll());
		qss.close();
	}
	w.setMinimumSize(600, 480);
	w.showMaximized();
	splash->finish(&w);
	delete splash;

	return (a.exec());
}
