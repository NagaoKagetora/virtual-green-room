#pragma once
#ifndef _VGR_CORE_BASIC_MODEL_HPP
#define _VGR_CORE_BASIC_MODEL_HPP

#include <osg/Node>
#include <osg/Geometry>
#include <osg/Geode>
#include <osg/Group>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <common/global.hpp>
#include <common/3rdparty/vcglib.hpp>

using namespace vgr;



class Model;
class ModelCanonicalizer;



class Model {

public:
	friend class ModelCanonicalizer;

private:
	QString								m_path;
	QString								m_name;
	QString								m_format;
	bool								m_usingFlippedNormal;
	bool								m_usingSharedVertex;
	pcvcg::TriMesh						*m_mesh;
	osg::ref_ptr<osg::Vec3Array>		m_faceVertexArray;
	osg::ref_ptr<osg::Vec3Array>		m_faceNormalArray;
	osg::ref_ptr<osg::Vec3Array>		m_vertexArray;
	osg::ref_ptr<osg::Vec3Array>		m_normalArray;
	osg::ref_ptr<osg::Vec4Array>		m_colorArray;
	osg::ref_ptr<osg::Geometry>			m_vertexGeometry;
	osg::ref_ptr<osg::Geode>			m_vertexGeode;
	osg::ref_ptr<osg::Geometry>			m_edgeGeometry;
	osg::ref_ptr<osg::Geode>			m_edgeGeode;
	osg::ref_ptr<osg::Geometry>			m_faceGeometry;
	osg::ref_ptr<osg::Geode>			m_faceGeode;
	osg::ref_ptr<osg::Group>			m_node;

public:
	Model() :
			m_path(), m_name(), m_format(), m_usingFlippedNormal(false), m_usingSharedVertex(true), m_mesh(NULL),
			m_faceVertexArray(), m_faceNormalArray(), m_vertexArray(), m_normalArray(), m_colorArray(),
			m_vertexGeometry(), m_vertexGeode(), m_edgeGeometry(), m_edgeGeode(), m_faceGeometry(), m_faceGeode(), m_node() { }
	Model(QString path) :
			m_path(), m_name(), m_format(), m_usingFlippedNormal(false), m_usingSharedVertex(true), m_mesh(NULL),
			m_faceVertexArray(), m_faceNormalArray(), m_vertexArray(), m_normalArray(), m_colorArray(),
			m_vertexGeometry(), m_vertexGeode(), m_edgeGeometry(), m_edgeGeode(), m_faceGeometry(), m_faceGeode(), m_node() {
		this->readFile(path);
	}
	~Model() { this->dispose(); }

public:
	QString path() const { return (this->m_path); }
	QString name() const { return (this->m_name); }
	QString format() const { return (this->m_format); }
	bool usingFlippedNormal() const { return (this->m_usingFlippedNormal); }
	void setUsingFlippedNormal(bool value);
	bool usingSharedVertex() const { return (this->m_usingSharedVertex); }
	void setUsingSharedVertex(bool value);
	const pcvcg::TriMesh *mesh() const { return (this->m_mesh); }
	pcvcg::TriMesh *mesh() { return (this->m_mesh); }
	const osg::ref_ptr<osg::Node> node() const { return (this->m_node); }
	osg::ref_ptr<osg::Node> node() { return (this->m_node); }

public:
	const osg::ref_ptr<osg::Node> vertexNode() const { return (this->m_vertexGeode); }
	osg::ref_ptr<osg::Node> vertexNode() { return (this->m_vertexGeode); }
	const osg::ref_ptr<osg::Node> edgeNode() const { return (this->m_edgeGeode); }
	osg::ref_ptr<osg::Node> edgeNode() { return (this->m_edgeGeode); }
	const osg::ref_ptr<osg::Node> faceNode() const { return (this->m_faceGeode); }
	osg::ref_ptr<osg::Node> faceNode() { return (this->m_faceGeode); }
	bool readFile(QString path);
	bool writeFile(QString path);
	bool preprocess();
	void dispose();
	void pruneMesh();
	void updateMesh();
	void buildGeometry();
	void updateGeometry();
	void arrangeVertexColors(const osg::Vec4Array &colors);
	void arrangeFaceColors(const osg::Vec4Array &colors);
};



class ModelCanonicalizer {

public:
	bool repairVertexConnectivities(Model *model);
	bool repairFaceConnectivities(Model *model);
	void reverseWindings(Model *model);
	void copyTopologies(Model *source, Model *target);
};



#endif /* _VGR_CORE_BASIC_MODEL_HPP */
