#include "SectionScene.hpp"
#include <QtCore/QString>
#include <osg/LineWidth>
#include <common/3rdparty/eigen.hpp>
#include <common/algo/util.hpp>
#include <core/basic/Model.hpp>
#include <core/basic/Configuration.hpp>

#pragma warning (push)
#pragma warning (disable:4482)	/* nonstandard extension used: enum 'enum' used in qualified name */



SectionScene::Sprite::Sprite() {

	this->m_titleText = new Text();
	this->m_titleText->setFont(Configuration::instance().spriteConfiguration().mediumFontName().toStdString());
	this->m_titleText->setFontSize(Configuration::instance().spriteConfiguration().mediumFontSize());
	this->m_titleText->setColor(Converter::toVec4(Configuration::instance().spriteConfiguration().mediumFontColor()));
	this->m_titleText->setAlignment(osgText::Text::AlignmentType::CENTER_BOTTOM_BASE_LINE);
	this->m_sourceNameText = new Text();
	this->m_sourceNameText->setFont(Configuration::instance().spriteConfiguration().smallFontName().toStdString());
	this->m_sourceNameText->setFontSize(Configuration::instance().spriteConfiguration().smallFontSize());
	this->m_sourceNameText->setColor(Converter::toVec4(Configuration::instance().spriteConfiguration().smallFontColor()));
	this->m_sourceNameText->setAlignment(osgText::Text::AlignmentType::RIGHT_BOTTOM_BASE_LINE);
	this->m_targetNameText = new Text();
	this->m_targetNameText->setFont(Configuration::instance().spriteConfiguration().smallFontName().toStdString());
	this->m_targetNameText->setFontSize(Configuration::instance().spriteConfiguration().smallFontSize());
	this->m_targetNameText->setColor(Converter::toVec4(Configuration::instance().spriteConfiguration().smallFontColor()));
	this->m_targetNameText->setAlignment(osgText::Text::AlignmentType::RIGHT_BOTTOM_BASE_LINE);
	this->m_xText = new Text();
	this->m_xText->setFont(Configuration::instance().spriteConfiguration().smallFontName().toStdString());
	this->m_xText->setFontSize(Configuration::instance().spriteConfiguration().smallFontSize());
	this->m_xText->setColor(Converter::toVec4(Configuration::instance().spriteConfiguration().smallFontColor()));
	this->m_xText->setAlignment(osgText::Text::AlignmentType::RIGHT_BOTTOM_BASE_LINE);
	this->m_yMaxText = new Text();
	this->m_yMaxText->setFont(Configuration::instance().spriteConfiguration().smallFontName().toStdString());
	this->m_yMaxText->setFontSize(Configuration::instance().spriteConfiguration().smallFontSize());
	this->m_yMaxText->setColor(Converter::toVec4(Configuration::instance().spriteConfiguration().smallFontColor()));
	this->m_yMinText = new Text();
	this->m_yMinText->setFont(Configuration::instance().spriteConfiguration().smallFontName().toStdString());
	this->m_yMinText->setFontSize(Configuration::instance().spriteConfiguration().smallFontSize());
	this->m_yMinText->setColor(Converter::toVec4(Configuration::instance().spriteConfiguration().smallFontColor()));
	this->m_sourceLegendRect = new Rect();
	this->m_sourceLegendRect->setFillColor(osg::Vec4(1.0f, 1.0f, 1.0f, 0.5f));
	this->m_sourceLegendRect->setBorderColor(osg::Vec4(0.0f, 0.0f, 0.0f, 0.0f));
	this->m_sourceLegendTransform = new Transform2D();
	this->m_sourceLegendTransform->addChild(this->m_sourceLegendRect);
	this->m_targetLegendRect = new Rect();
	this->m_targetLegendRect->setFillColor(osg::Vec4(1.0f, 1.0f, 1.0f, 0.5f));
	this->m_targetLegendRect->setBorderColor(osg::Vec4(0.0f, 0.0f, 0.0f, 0.0f));
	this->m_targetLegendTransform = new Transform2D();
	this->m_targetLegendTransform->addChild(this->m_targetLegendRect);
	this->m_gridVertices = new osg::Vec3Array();
	this->m_gridColors = new osg::Vec4Array();
	this->m_gridColors->push_back(osg::Vec4(1.0f, 1.0f, 1.0f, 0.5f));
	this->m_gridPrimitives = new osg::DrawArrays();
	this->m_gridGeometry = new osg::Geometry();
	this->m_gridGeometry->setVertexArray(this->m_gridVertices);
	this->m_gridGeometry->setColorArray(this->m_gridColors);
	this->m_gridGeometry->setColorBinding(osg::Geometry::AttributeBinding::BIND_OVERALL);
	this->m_gridGeometry->addPrimitiveSet(this->m_gridPrimitives);
	this->m_zeroLineVertices = new osg::Vec3Array();
	this->m_zeroLineColors = new osg::Vec4Array();
	this->m_zeroLineColors->push_back(osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f));
	this->m_zeroLinePrimitives = new osg::DrawArrays();
	this->m_zeroLineGeometry = new osg::Geometry();
	this->m_zeroLineGeometry->setVertexArray(this->m_zeroLineVertices);
	this->m_zeroLineGeometry->setColorArray(this->m_zeroLineColors);
	this->m_zeroLineGeometry->setColorBinding(osg::Geometry::AttributeBinding::BIND_OVERALL);
	this->m_zeroLineGeometry->addPrimitiveSet(this->m_zeroLinePrimitives);
	this->m_zeroLineGeometry->getOrCreateStateSet()->setAttribute(new osg::LineWidth(2.0f), osg::StateAttribute::Values::ON);
	this->m_gridGeode = new osg::Geode();
	this->m_gridGeode->addDrawable(this->m_gridGeometry);
	this->m_gridGeode->addDrawable(this->m_zeroLineGeometry);
	this->m_gridTransform = new Transform2D();
	this->m_gridTransform->addChild(this->m_gridGeode);
	this->m_hud = new Hud();
	this->m_hud->addChild(this->m_titleText);
	this->m_hud->addChild(this->m_sourceNameText);
	this->m_hud->addChild(this->m_targetNameText);
	this->m_hud->addChild(this->m_xText);
	this->m_hud->addChild(this->m_yMaxText);
	this->m_hud->addChild(this->m_yMinText);
	this->m_hud->addChild(this->m_sourceLegendTransform);
	this->m_hud->addChild(this->m_targetLegendTransform);
	this->m_hud->addChild(this->m_gridTransform);
	this->m_node = new osg::Group();
	this->m_node->addChild(this->m_hud);

	this->setCaption(std::string());
	this->setNumbers(0, 0, 0);
	this->setGrid(10, 10);
	this->setZeroLine(true);
}

void SectionScene::Sprite::setCaption(const std::string &title, const std::string &sourceName, const std::string &targetName) {
	this->m_titleText->setText(title);
	this->m_sourceNameText->setText(sourceName);
	this->m_targetNameText->setText(targetName);
	this->m_sourceLegendRect->setFillColor(osg::Vec4(0.0f, 0.5f, 1.0f, 0.5f));
	this->m_sourceLegendTransform->dirtyBound();
	this->m_targetLegendRect->setFillColor(osg::Vec4(1.0f, 0.5f, 0.0f, 0.5f));
	this->m_targetLegendTransform->dirtyBound();
	this->m_hud->dirtyBound();
	this->m_node->dirtyBound();
}

void SectionScene::Sprite::setCaption(const std::string &title) {
	this->m_titleText->setText(title);
	this->m_sourceNameText->setText(std::string());
	this->m_targetNameText->setText(std::string());
	this->m_sourceLegendRect->setFillColor(osg::Vec4(0.0f, 0.0f, 0.0f, 0.0f));
	this->m_sourceLegendTransform->dirtyBound();
	this->m_targetLegendRect->setFillColor(osg::Vec4(0.0f, 0.0f, 0.0f, 0.0f));
	this->m_targetLegendTransform->dirtyBound();
	this->m_hud->dirtyBound();
	this->m_node->dirtyBound();
}

void SectionScene::Sprite::setNumbers(real_t x, real_t yMax, real_t yMin) {
	this->m_xText->setText(x);
	this->m_yMaxText->setText(yMax);
	this->m_yMinText->setText(yMin);
	this->m_hud->dirtyBound();
	this->m_node->dirtyBound();
}

void SectionScene::Sprite::setGrid(int verticalSlices, int horizontalSlices) {
	this->m_gridVertices->clear();
	for (int i = 0; i <= horizontalSlices; ++i) {
		this->m_gridVertices->push_back(osg::Vec3(0.0f, (float)i / horizontalSlices, 0.0f));
		this->m_gridVertices->push_back(osg::Vec3(1.0f, (float)i / horizontalSlices, 0.0f));
	}
	for (int i = 0; i <= verticalSlices; ++i) {
		this->m_gridVertices->push_back(osg::Vec3((float)i / verticalSlices, 0.0f, 0.0f));
		this->m_gridVertices->push_back(osg::Vec3((float)i / verticalSlices, 1.0f, 0.0f));
	}
	this->m_gridVertices->dirty();
	this->m_gridPrimitives->set(osg::PrimitiveSet::Mode::LINES, 0, this->m_gridVertices->size());
	this->m_gridPrimitives->dirty();
	this->m_gridGeometry->dirtyDisplayList();
	this->m_gridGeometry->dirtyBound();
	this->m_gridGeode->dirtyBound();
	this->m_gridTransform->dirtyBound();
	this->m_hud->dirtyBound();
	this->m_node->dirtyBound();
}

void SectionScene::Sprite::setZeroLine(bool symmetric) {
	this->m_zeroLineVertices->clear();
	if (symmetric) {
		this->m_zeroLineVertices->push_back(osg::Vec3(0.0f, 0.5f, 0.0f));
		this->m_zeroLineVertices->push_back(osg::Vec3(1.0f, 0.5f, 0.0f));
	} else {
		this->m_zeroLineVertices->push_back(osg::Vec3(0.0f, 0.0f, 0.0f));
		this->m_zeroLineVertices->push_back(osg::Vec3(1.0f, 0.0f, 0.0f));
	}
	this->m_zeroLineVertices->dirty();
	this->m_zeroLinePrimitives->set(osg::PrimitiveSet::Mode::LINES, 0, this->m_zeroLineVertices->size());
	this->m_zeroLinePrimitives->dirty();
	this->m_zeroLineGeometry->dirtyDisplayList();
	this->m_zeroLineGeometry->dirtyBound();
	this->m_gridGeode->dirtyBound();
	this->m_gridTransform->dirtyBound();
	this->m_hud->dirtyBound();
	this->m_node->dirtyBound();
}

void SectionScene::Sprite::setViewport(int width, int height) {
	this->m_hud->setProjectionMatrixAsOrtho2D(0, width, 0, height);
	this->m_hud->dirtyBound();
	this->m_titleText->setPosition(width / 2.0f, height - 30.0f);
	this->m_sourceNameText->setPosition(width - 45.0f, height - 20.0f);
	this->m_targetNameText->setPosition(width - 45.0f, height - 35.0f);
	this->m_xText->setPosition(width - 20.0f, 5.0f);
	this->m_yMaxText->setPosition(20.0f, height - 35.0f);
	this->m_yMinText->setPosition(20.0f, 5.0f);
	this->m_sourceLegendTransform->setPosition(width - 40.0f, height - 20.0f);
	this->m_sourceLegendTransform->setSize(20.0f, 8.0f);
	this->m_sourceLegendTransform->dirtyBound();
	this->m_targetLegendTransform->setPosition(width - 40.0f, height - 35.0f);
	this->m_targetLegendTransform->setSize(20.0f, 8.0f);
	this->m_targetLegendTransform->dirtyBound();
	this->m_gridTransform->setPosition(20.0f, 20.0f);
	this->m_gridTransform->setSize(width - 40.0f, height - 60.0f);
	this->m_gridTransform->dirtyBound();
	this->m_node->dirtyBound();
}



SectionScene::Content::Content() {
	this->m_geode = new osg::Geode();
	this->m_camera = new osg::Camera();
	this->m_camera->addChild(this->m_geode);
	this->m_camera->setReferenceFrame(osg::Transform::ReferenceFrame::ABSOLUTE_RF);
	this->m_camera->setRenderOrder(osg::Camera::RenderOrder::POST_RENDER);
	this->m_camera->setProjectionMatrixAsOrtho2D(0, 1000, 0, 1000);
	this->m_camera->setViewMatrix(osg::Matrix::identity());
	this->m_camera->setClearMask(GL_DEPTH_BUFFER_BIT);
	this->m_camera->getOrCreateStateSet()->setAttribute(new osg::LineWidth(2.0f), osg::StateAttribute::Values::ON);
	this->m_camera->getOrCreateStateSet()->setMode(GL_LINE_SMOOTH, osg::StateAttribute::Values::ON);
	this->m_camera->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::Values::OFF);
	this->m_camera->getOrCreateStateSet()->setMode(GL_DEPTH_TEST, osg::StateAttribute::Values::OFF);
	this->m_camera->getOrCreateStateSet()->setMode(GL_BLEND, osg::StateAttribute::Values::ON);
	this->m_node = new osg::Group();
	this->m_node->addChild(this->m_camera);
}

void SectionScene::Content::updateCurves(const Curve &source, const Curve &target) {
	this->m_geode->removeDrawables(0, this->m_geode->getNumDrawables());
	this->addLineCurve(source, osg::Vec4(0.0f, 0.5f, 1.0f, 1.0f));
	this->addLineCurve(target, osg::Vec4(1.0f, 0.5f, 0.0f, 1.0f));
	this->m_geode->dirtyBound();
	this->m_camera->dirtyBound();
	this->m_node->dirtyBound();
}

void SectionScene::Content::updateCurves(const Curve &source, real_t acceptance, real_t rejection) {
	this->m_geode->removeDrawables(0, this->m_geode->getNumDrawables());
	this->addBarCurve(source, acceptance, rejection);
	this->m_geode->dirtyBound();
	this->m_camera->dirtyBound();
	this->m_node->dirtyBound();
}

void SectionScene::Content::addLineCurve(const Curve &curve, osg::Vec4 color) {
	if (!curve.valid()) return;

	osg::Vec3Array *vertices = new osg::Vec3Array(curve.knotsCount());
	for (size_t i = 0, c = 0; i < curve.segmentsCount(); ++i) {
		for (size_t j = 0; j < curve.segment(i).cols(); ++j) {
			vertices->at(c++) = osg::Vec3(curve.segment(i)(0, j), curve.segment(i)(1, j), 0.0f);
		}
	}
	osg::Vec4Array *colors = new osg::Vec4Array();
	colors->push_back(color);
	osg::Geometry *geometry = new osg::Geometry();
	geometry->setVertexArray(vertices);
	geometry->setColorArray(colors);
	geometry->setColorBinding(osg::Geometry::AttributeBinding::BIND_OVERALL);
	for (size_t i = 0, c = 0; i < curve.segmentsCount(); ++i) {
		geometry->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::Mode::LINE_STRIP, c, curve.segment(i).cols()));
		c += curve.segment(i).cols();
	}
	this->m_geode->addDrawable(geometry);
}

void SectionScene::Content::addBarCurve(const Curve &curve, real_t acceptance, real_t rejection) {
	if (!curve.valid()) return;

	if (rejection < acceptance) rejection = acceptance;

	osg::Vec3Array *vertices = new osg::Vec3Array(2 * curve.knotsCount());
	for (size_t i = 0, c = 0 * curve.knotsCount(); i < curve.segmentsCount(); ++i) {
		for (size_t j = 0; j < curve.segment(i).cols(); ++j) {
			vertices->at(c++) = osg::Vec3(curve.segment(i)(0, j), 500.0f, 0.0f);
			vertices->at(c++) = osg::Vec3(curve.segment(i)(0, j), curve.segment(i)(1, j), 0.0f);
		}
	}
	osg::Vec4Array *colors = new osg::Vec4Array(curve.knotsCount());
	for (size_t i = 0, c = 0 * curve.knotsCount(); i < curve.segmentsCount(); ++i) {
		for (size_t j = 0; j < curve.segment(i).cols(); ++j) {
			float value = std::abs(500.0f - curve.segment(i)(1, j));
			colors->at(c++) = osg::Vec4(1.0f * (value > acceptance), 1.0f * !(value > rejection), 0.0f, 1.0f);
		}
	}
	osg::Geometry *geometry = new osg::Geometry();
	geometry->setVertexArray(vertices);
	geometry->setColorArray(colors);
	geometry->setColorBinding(osg::Geometry::AttributeBinding::BIND_PER_PRIMITIVE);
	geometry->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::Mode::LINES, 0, 2 * curve.knotsCount()));
	this->m_geode->addDrawable(geometry);
}

void SectionScene::Content::setViewport(int width, int height) {
	this->m_camera->setViewport(20, 20, width - 40, height - 60);
	this->m_camera->dirtyBound();
	this->m_node->dirtyBound();
}



SectionScene::SectionScene() : Scene() {
	this->m_content = new Content();
	this->m_sprite = new Sprite();
	this->m_root = new osg::Group();
	this->m_root->addChild(this->m_sprite->node());
	this->m_root->addChild(this->m_content->node());
	this->screen().widget()->setAcceptDrops(false);
	this->screen().view()->setSceneData(this->m_root);
}

void SectionScene::updateCaption(const std::string &title, const std::string &sourceName, const std::string &targetName) {
	this->m_sprite->setCaption(title, sourceName, targetName);
	this->m_root->dirtyBound();
	this->m_caption = title;
}

void SectionScene::updateCaption(const std::string &title) {
	this->m_sprite->setCaption(title);
	this->m_root->dirtyBound();
	this->m_caption = title;
}

void SectionScene::updateScene(const Curve &source, const Curve &target, Curve::NormalizeOption unifyOption) {
	eigen::Vector3 min = source.min().cwiseMin(target.min());
	eigen::Vector3 max = source.max().cwiseMax(target.max());
	real_t x = (max - min).x();
	real_t yMin = 0, yMax = 0;
	if (unifyOption == Curve::NormalizeOption::Unilateral) {
		yMin = 0;
		yMax = (max - min).y();
	} else {
		yMin = -std::max(std::abs(max.y()), std::abs(min.y()));
		yMax =  std::max(std::abs(max.y()), std::abs(min.y()));
	}
	this->m_content->updateCurves(
			source.normalized(min, max, eigen::Vector3(1000.0, 1000.0, 0.0), unifyOption),
			target.normalized(min, max, eigen::Vector3(1000.0, 1000.0, 0.0), unifyOption));
	this->m_sprite->setNumbers(x, yMax, yMin);
	this->m_sprite->setZeroLine(unifyOption == Curve::NormalizeOption::Bilateral);
}

void SectionScene::updateScene(const Curve &source, real_t acceptance, real_t rejection) {
	eigen::Vector3 min = source.min();
	eigen::Vector3 max = source.max();
	real_t x = (max - min).x();
	real_t yMin = -std::max(std::abs(max.y()), std::abs(min.y()));
	real_t yMax =  std::max(std::abs(max.y()), std::abs(min.y()));
	acceptance = std::min<real_t>(1, acceptance / yMax);
	rejection = std::min<real_t>(1, rejection / yMax);
	this->m_content->updateCurves(source.normalized(eigen::Vector3(1000.0, 1000.0, 0.0), Curve::NormalizeOption::Bilateral), acceptance * 500.0, rejection * 500.0);
	this->m_sprite->setNumbers(x, yMax, yMin);
	this->m_sprite->setZeroLine(true);
}

void SectionScene::setViewport(int width, int height) {
	if (width < 0) width = this->screen().widget()->width();
	if (height < 0) height = this->screen().widget()->height();

	this->m_content->setViewport(width, height);
	this->m_sprite->setViewport(width, height);

	Scene::setViewport(width, height);
}



#pragma warning (pop)
