#include "Model.hpp"
#include <common/algo/util.hpp>
#include <common/util/logger.hpp>
///	<todo>
///	The OBJ importing module has a snippet activated on defining GL macro, where
///	the point coordinates will be coerced to single floting vectors that is not
///	compatible with current mesh definition with double precision points.
///	We should deactive the code snippet, or nofity the author about this ill design.
///	</todo>
#ifdef __gl_h_
#  undef __gl_h_
#  include <vcglib/wrap/io_trimesh/import.h>
#  include <vcglib/wrap/io_trimesh/export.h>
#  define __gl_h_
#else
#  include <vcglib/wrap/io_trimesh/import.h>
#  include <vcglib/wrap/io_trimesh/export.h>
#endif
#include <common/3rdparty/vcg/import_nas.h>
#include <common/3rdparty/vcg/export_nas.h>
#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QFileInfo>

#pragma warning (push)
#pragma warning (disable:4482)	/* nonstandard extension used: enum 'enum' used in qualified name */



void Model::setUsingFlippedNormal(bool value) {
	if (this->m_usingFlippedNormal == value) return;
	this->m_usingFlippedNormal = value;
	ModelCanonicalizer().reverseWindings(this);
	this->updateMesh();
	this->updateGeometry();
}

void Model::setUsingSharedVertex(bool value) {
	if (this->m_usingSharedVertex == value) return;
	this->m_usingSharedVertex = value;
	this->updateGeometry();
}

bool Model::readFile(QString path) {
	this->dispose();
	QFileInfo fileInfo(path);
	if (!fileInfo.exists()) {
		_VGR_SYSTEM_ERR(boost::format("Loading failed, file <%1%> does not exist.") % path.toStdString());
		return (false);
	}

	this->m_path = fileInfo.absoluteFilePath();
	this->m_name = fileInfo.completeBaseName();
	this->m_format = fileInfo.suffix();

	_VGR_SYSTEM_MSG(boost::format("Loading model %1% <%2%>.") % this->m_name.toStdString() % this->m_path.toStdString());
	this->m_mesh = new pcvcg::TriMesh();
	int errCode = 0;
	if (this->m_format.toLower() == QString("nas")) errCode = vcg::tri::io::ImporterNAS<pcvcg::TriMesh>::Open(*this->m_mesh, this->m_path.toStdString().c_str());
	else errCode = vcg::tri::io::Importer<pcvcg::TriMesh>::Open(*this->m_mesh, this->m_path.toStdString().c_str());
	if (errCode) {
		_VGR_SYSTEM_ERR(boost::format("Loading failed, an error [%1%] occurred during loading procedure.") % errCode);
		return (false);
	}
	_VGR_SYSTEM_MSG(boost::format("Loading %1% finished.") % this->m_name.toStdString());
	return (true);
}

bool Model::writeFile(QString path) {
	QFileInfo fileInfo(path);

	_VGR_SYSTEM_MSG(boost::format("Saving model %1% <%2%>.") % fileInfo.completeBaseName().toStdString() % fileInfo.absoluteFilePath().toStdString());
	int errCode = 0;
	if (fileInfo.suffix().toLower() == QString("nas")) errCode = vcg::tri::io::ExporterNAS<pcvcg::TriMesh>::Save(*this->m_mesh, fileInfo.absoluteFilePath().toStdString().c_str());
	else errCode = vcg::tri::io::Exporter<pcvcg::TriMesh>::Save(*this->m_mesh, fileInfo.absoluteFilePath().toStdString().c_str());
	if (errCode) {
		_VGR_SYSTEM_ERR(boost::format("Saving failed, an error [%1%] occurred during saving procedure.") % errCode);
		return (false);
	}
	_VGR_SYSTEM_MSG(boost::format("Saving %1% finished.") % fileInfo.completeBaseName().toStdString());
	return (true);
}

bool Model::preprocess() {
	_VGR_SYSTEM_MSG(boost::format("Preprocessing and building mesh %1%.") % this->m_name.toStdString());
	this->pruneMesh();
	this->updateMesh();
	if (ModelCanonicalizer().repairVertexConnectivities(this)) {
		this->pruneMesh();
		this->updateMesh();
	}
	if (ModelCanonicalizer().repairFaceConnectivities(this)) {
		this->pruneMesh();
		this->updateMesh();
	}
	this->buildGeometry();
	this->updateGeometry();
	_VGR_SYSTEM_MSG(boost::format("Preprocessing and building %1% finished.") % this->m_name.toStdString());
	return (true);
}

void Model::dispose() {
	if (this->m_mesh) {
		this->m_mesh->Clear();
		delete this->m_mesh;
		this->m_mesh = NULL;
	}
	this->m_path = QString();
	this->m_name = QString();
	this->m_format = QString();
	this->m_node.release();
	this->m_vertexGeode.release();
	this->m_vertexGeometry.release();
	this->m_edgeGeode.release();
	this->m_edgeGeometry.release();
	this->m_faceGeode.release();
	this->m_faceGeometry.release();
	this->m_faceVertexArray.release();
	this->m_faceNormalArray.release();
	this->m_vertexArray.release();
	this->m_normalArray.release();
	this->m_colorArray.release();
}

void Model::pruneMesh() {
	if (!this->m_mesh) return;
	vcg::tri::Clean<pcvcg::TriMesh>::RemoveDuplicateVertex(*this->m_mesh);
	vcg::tri::Clean<pcvcg::TriMesh>::RemoveDuplicateFace(*this->m_mesh);
	vcg::tri::Clean<pcvcg::TriMesh>::RemoveUnreferencedVertex(*this->m_mesh);
	vcg::tri::Allocator<pcvcg::TriMesh>::CompactVertexVector(*this->m_mesh);
	vcg::tri::Allocator<pcvcg::TriMesh>::CompactFaceVector(*this->m_mesh);
	vcg::tri::Allocator<pcvcg::TriMesh>::CompactEdgeVector(*this->m_mesh);
}

void Model::updateMesh() {
	if (!this->m_mesh) return;
	vcg::tri::UpdateBounding<pcvcg::TriMesh>::Box(*this->m_mesh);
	pcvcg::UpdateTopology<pcvcg::TriMesh>::VertexFace(*this->m_mesh);
	pcvcg::UpdateTopology<pcvcg::TriMesh>::FaceFace(*this->m_mesh);
	pcvcg::UpdateTopology<pcvcg::TriMesh>::FillEdgeVector(*this->m_mesh);
	pcvcg::UpdateTopology<pcvcg::TriMesh>::EdgeVertex(*this->m_mesh);
	pcvcg::UpdateTopology<pcvcg::TriMesh>::FaceEdge(*this->m_mesh);
	pcvcg::UpdateFlags<pcvcg::TriMesh>::FaceBorderFromFF(*this->m_mesh);
	pcvcg::UpdateFlags<pcvcg::TriMesh>::VertexBorderFromFace(*this->m_mesh);
	pcvcg::UpdateFlags<pcvcg::TriMesh>::EdgeBorderFromFace(*this->m_mesh);
	vcg::tri::UpdateNormal<pcvcg::TriMesh>::PerFaceNormalized(*this->m_mesh);
	vcg::tri::UpdateNormal<pcvcg::TriMesh>::PerVertexNormalized(*this->m_mesh);
}

void Model::buildGeometry() {

	// Builds arrays.
	this->m_faceVertexArray = new osg::Vec3Array();
	this->m_faceNormalArray = new osg::Vec3Array();
	this->m_vertexArray = new osg::Vec3Array();
	this->m_normalArray = new osg::Vec3Array();
	this->m_colorArray = new osg::Vec4Array();

	// Sets up vertex geometry.
	this->m_vertexGeometry = new osg::Geometry();
	this->m_vertexGeometry->setVertexArray(this->m_vertexArray);
	this->m_vertexGeometry->setNormalArray(this->m_normalArray);
	this->m_vertexGeometry->setNormalBinding(osg::Geometry::AttributeBinding::BIND_PER_VERTEX);
	this->m_vertexGeometry->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::Mode::POINTS, 0, this->m_mesh->vn));
	this->m_vertexGeode = new osg::Geode();
	this->m_vertexGeode->addDrawable(this->m_vertexGeometry);

	// Sets up edge geometry.
	osg::DrawElementsUInt *edgePrimitives = new osg::DrawElementsUInt(osg::PrimitiveSet::Mode::LINES, 0);
	edgePrimitives->reserve(this->m_mesh->en * 2);
	for (pcvcg::TriMesh::ConstEdgeIterator ce_iter = this->m_mesh->edge.begin();
			ce_iter != this->m_mesh->edge.end(); ++ce_iter) {
		edgePrimitives->push_back(vcg::tri::Index(*this->m_mesh, ce_iter->V(0)));
		edgePrimitives->push_back(vcg::tri::Index(*this->m_mesh, ce_iter->V(1)));
	}
	this->m_edgeGeometry = new osg::Geometry();
	this->m_edgeGeometry->setVertexArray(this->m_vertexArray);
	this->m_edgeGeometry->setNormalArray(this->m_normalArray);
	this->m_edgeGeometry->setNormalBinding(osg::Geometry::AttributeBinding::BIND_PER_VERTEX);
	this->m_edgeGeometry->addPrimitiveSet(edgePrimitives);
	this->m_edgeGeode = new osg::Geode();
	this->m_edgeGeode->addDrawable(this->m_edgeGeometry);
	
	// Sets up face geometry.
	this->m_faceGeometry = new osg::Geometry();
	this->m_faceGeometry->setVertexArray(this->m_faceVertexArray);
	this->m_faceGeometry->setNormalArray(this->m_faceNormalArray);
	this->m_faceGeometry->setNormalBinding(osg::Geometry::AttributeBinding::BIND_PER_VERTEX);
	this->m_faceGeometry->setColorArray(this->m_colorArray);
	this->m_faceGeometry->setColorBinding(osg::Geometry::AttributeBinding::BIND_PER_VERTEX);
	this->m_faceGeode = new osg::Geode();
	this->m_faceGeode->addDrawable(this->m_faceGeometry);

	// Builds group node.
	this->m_node = new osg::Group();
	this->m_node->addChild(this->m_vertexGeode);
	this->m_node->addChild(this->m_edgeGeode);
	this->m_node->addChild(this->m_faceGeode);
}

void Model::updateGeometry() {
	if (!this->m_mesh || !this->m_vertexArray || !this->m_faceNormalArray || !this->m_normalArray
			|| !this->m_vertexGeometry || !this->m_edgeGeometry || !this->m_faceGeometry) return;
	this->m_vertexArray->clear();
	this->m_normalArray->clear();
	this->m_vertexArray->reserve(this->m_mesh->vn);
	this->m_normalArray->reserve(this->m_mesh->vn);
	for (pcvcg::TriMesh::ConstVertexIterator cv_iter = this->m_mesh->vert.begin();
			cv_iter != this->m_mesh->vert.end(); ++cv_iter) {
		this->m_vertexArray->push_back(algo::Util::cast<osg::Vec3>(cv_iter->cP()));
		this->m_normalArray->push_back(algo::Util::cast<osg::Vec3>(cv_iter->cN()));
	}
	this->m_faceVertexArray->clear();
	this->m_faceNormalArray->clear();
	this->m_colorArray->clear();
	if (this->m_usingSharedVertex) {
		this->m_faceVertexArray->reserve(this->m_mesh->vn);
		this->m_faceNormalArray->reserve(this->m_mesh->vn);
		this->m_colorArray->reserve(this->m_mesh->vn);
		for (pcvcg::TriMesh::ConstVertexIterator cv_iter = this->m_mesh->vert.begin();
				cv_iter != this->m_mesh->vert.end(); ++cv_iter) {
			this->m_faceVertexArray->push_back(algo::Util::cast<osg::Vec3>(cv_iter->cP()));
			this->m_faceNormalArray->push_back(algo::Util::cast<osg::Vec3>(cv_iter->cN()));
			this->m_colorArray->push_back(osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f));
		}
		osg::DrawElementsUInt *facePrimitives = new osg::DrawElementsUInt(osg::PrimitiveSet::Mode::TRIANGLES, 0);
		for (pcvcg::TriMesh::ConstFaceIterator cf_iter = this->m_mesh->face.begin();
				cf_iter != this->m_mesh->face.end(); ++cf_iter) {
			facePrimitives->push_back(vcg::tri::Index(*this->m_mesh, cf_iter->cV(0)));
			facePrimitives->push_back(vcg::tri::Index(*this->m_mesh, cf_iter->cV(1)));
			facePrimitives->push_back(vcg::tri::Index(*this->m_mesh, cf_iter->cV(2)));
		}
		this->m_faceGeometry->removePrimitiveSet(0, this->m_faceGeometry->getNumPrimitiveSets());
		this->m_faceGeometry->addPrimitiveSet(facePrimitives);
	} else {
		this->m_faceVertexArray->reserve(this->m_mesh->fn * 3);
		this->m_faceNormalArray->reserve(this->m_mesh->fn * 3);
		this->m_colorArray->reserve(this->m_mesh->fn * 3);
		for (pcvcg::TriMesh::ConstFaceIterator cf_iter = this->m_mesh->face.begin();
				cf_iter != this->m_mesh->face.end(); ++cf_iter) {
			this->m_faceVertexArray->push_back(algo::Util::cast<osg::Vec3>(this->m_mesh->vert[vcg::tri::Index(*this->m_mesh, cf_iter->cV(0))].cP()));
			this->m_faceVertexArray->push_back(algo::Util::cast<osg::Vec3>(this->m_mesh->vert[vcg::tri::Index(*this->m_mesh, cf_iter->cV(1))].cP()));
			this->m_faceVertexArray->push_back(algo::Util::cast<osg::Vec3>(this->m_mesh->vert[vcg::tri::Index(*this->m_mesh, cf_iter->cV(2))].cP()));
			this->m_faceNormalArray->push_back(algo::Util::cast<osg::Vec3>(cf_iter->cN()));
			this->m_faceNormalArray->push_back(algo::Util::cast<osg::Vec3>(cf_iter->cN()));
			this->m_faceNormalArray->push_back(algo::Util::cast<osg::Vec3>(cf_iter->cN()));
			this->m_colorArray->push_back(osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f));
			this->m_colorArray->push_back(osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f));
			this->m_colorArray->push_back(osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f));
		}
		osg::DrawArrays *facePrimitives = new osg::DrawArrays(osg::PrimitiveSet::Mode::TRIANGLES, 0, this->m_mesh->fn * 3);
		this->m_faceGeometry->removePrimitiveSet(0, this->m_faceGeometry->getNumPrimitiveSets());
		this->m_faceGeometry->addPrimitiveSet(facePrimitives);
	}
	this->m_vertexGeometry->dirtyDisplayList();
	this->m_edgeGeometry->dirtyDisplayList();
	this->m_faceGeometry->dirtyDisplayList();
}

void Model::arrangeVertexColors(const osg::Vec4Array &colors) {
	if (!this->m_mesh || !this->m_colorArray
			|| !this->m_vertexGeometry || !this->m_edgeGeometry || !this->m_faceGeometry) return;
	if (colors.size() != this->m_mesh->vn) return;
	if (this->m_usingSharedVertex) {
		this->m_colorArray->assign(colors.begin(), colors.end());
	} else {
		this->m_colorArray->clear();
		this->m_colorArray->reserve(this->m_mesh->fn * 3);
		for (pcvcg::TriMesh::ConstFaceIterator cf_iter = this->m_mesh->face.begin();
				cf_iter != this->m_mesh->face.end(); ++cf_iter) {
			this->m_colorArray->push_back(colors[vcg::tri::Index(*this->m_mesh, cf_iter->cV(0))]);
			this->m_colorArray->push_back(colors[vcg::tri::Index(*this->m_mesh, cf_iter->cV(1))]);
			this->m_colorArray->push_back(colors[vcg::tri::Index(*this->m_mesh, cf_iter->cV(2))]);
		}
	}
	this->m_vertexGeometry->dirtyDisplayList();
	this->m_edgeGeometry->dirtyDisplayList();
	this->m_faceGeometry->dirtyDisplayList();
}

void Model::arrangeFaceColors(const osg::Vec4Array &colors) {
	if (!this->m_mesh || !this->m_colorArray
			|| !this->m_vertexGeometry || !this->m_edgeGeometry || !this->m_faceGeometry) return;
	if (this->m_usingSharedVertex) return;
	if (colors.size() != this->m_mesh->fn * 3) return;
	this->m_colorArray->assign(colors.begin(), colors.end());
	this->m_vertexGeometry->dirtyDisplayList();
	this->m_edgeGeometry->dirtyDisplayList();
	this->m_faceGeometry->dirtyDisplayList();
}



bool ModelCanonicalizer::repairVertexConnectivities(Model *model) {

	if (!model || !model->mesh()) return (false);
	pcvcg::TriMesh *mesh = model->mesh();

	// Finds all boundary vertices and their adjacent vertices.
	std::map<size_t, std::vector<size_t> > vert_adj;
	for (size_t i = 0; i < mesh->edge.size(); ++i) {
		const pcvcg::TriMesh::EdgeType &edge(mesh->edge[i]);
		if (edge.IsB()) {
			size_t vert0 = vcg::tri::Index(*mesh, edge.cV(0));
			size_t vert1 = vcg::tri::Index(*mesh, edge.cV(1));
			if (!vert_adj.count(vert0)) vert_adj[vert0] = std::vector<size_t>();
			if (!vert_adj.count(vert1)) vert_adj[vert1] = std::vector<size_t>();
			vert_adj[vert0].push_back(vert1);
			vert_adj[vert1].push_back(vert0);
		}
	}

	// Extracts vertices as shared adjacency. 
	std::set<size_t> shared_vert_adj;
	std::set<size_t> split_vert_adj;
	for (std::map<size_t, std::vector<size_t> >::const_iterator iter = vert_adj.begin();
			iter != vert_adj.end(); ++iter) {
		if (iter->second.size() != 2) shared_vert_adj.insert(iter->first);
		else split_vert_adj.insert(iter->first);
	}

	if (!shared_vert_adj.size()) return (false);

	while (true) {

		// Fixes T-vertices. Skip if the number of adjacent vertices is greater
		// or less than 2, which mean the current vertex is a shared adjacency
		// or invalid boundary vertex.
		std::set<size_t> neib_faces;
		std::vector<size_t> face_deletion;
		std::set<size_t> shared_faces;
		std::set<size_t> next_round_split;
		for (std::set<size_t>::const_iterator iter = split_vert_adj.begin();
			iter != split_vert_adj.end(); ++iter) {

				size_t vert = *iter;
				size_t adj0 = vert_adj[vert].front();
				size_t adj1 = vert_adj[vert].back();
				if (!shared_vert_adj.count(adj0) || !shared_vert_adj.count(adj1)) continue;

				// Finds common face of two adjacent vertices. 
				neib_faces.clear();
				for (pcvcg::VFIterator vf(&(mesh->vert[adj0])); !vf.End(); ++vf) {
					neib_faces.insert(vcg::tri::Index(*mesh, vf.F()));
				}
				for (pcvcg::VFIterator vf(&(mesh->vert[adj1])); !vf.End(); ++vf) {
					size_t face = vcg::tri::Index(*mesh, vf.F());
					if (neib_faces.count(face)) {
						size_t vert0 = vcg::tri::Index(*mesh, vf.F()->V1(vf.I()));
						size_t vert1 = vcg::tri::Index(*mesh, vf.F()->V2(vf.I()));
						bool direct = !(vert0 == adj0);
						bool deletion = !shared_faces.count(face);
						size_t common = direct ? vert0 : vert1;
						if (shared_vert_adj.count(common)) shared_faces.insert(face);
						if (deletion) {
							vcg::tri::Allocator<pcvcg::TriMesh>::AddFaces(*mesh, 1);
							mesh->face.back().V(0) = &(mesh->vert[common]);
							mesh->face.back().V(!direct + 1) = &(mesh->vert[adj0]);
							mesh->face.back().V(direct + 1) = &(mesh->vert[vert]);
							mesh->face.back().ClearAllF();
							vcg::tri::Allocator<pcvcg::TriMesh>::AddFaces(*mesh, 1);
							mesh->face.back().V(0) = &(mesh->vert[common]);
							mesh->face.back().V(!direct + 1) = &(mesh->vert[vert]);
							mesh->face.back().V(direct + 1) = &(mesh->vert[adj1]);
							mesh->face.back().ClearAllF();
							face_deletion.push_back(face);
						} else {
							next_round_split.insert(vert);
						}
						break;
					}
				}
		}

		// Removes splitted faces and update topologies.
		foreach (size_t face, face_deletion) {
			if (!mesh->face[face].IsD()) vcg::tri::Allocator<pcvcg::TriMesh>::DeleteFace(*mesh, mesh->face[face]);
		}
		pcvcg::UpdateTopology<pcvcg::TriMesh>::FaceFace(*mesh);
		pcvcg::UpdateTopology<pcvcg::TriMesh>::VertexFace(*mesh);
		if (next_round_split.size() == 0) break;
		split_vert_adj = next_round_split;
	}

	return (true);
}

bool ModelCanonicalizer::repairFaceConnectivities(Model *model) {

	if (!model || !model->mesh()) return (false);
	pcvcg::TriMesh *mesh = model->mesh();

	std::vector<bool> flags(mesh->en, false);

	// Finds non-manifold edges.
	for (size_t i = 0; i < mesh->fn; ++i) {
		const pcvcg::TriMesh::FaceType &face(mesh->face[i]);
		const pcvcg::TriMesh::VertexPointer vps[] = { face.cV(0), face.cV(1), face.cV(2) };
		for (int j = 0; j < 3; ++j) {
			const pcvcg::TriMesh::EdgePointer ep = face.cFEp(j);
			int ei = vcg::tri::Index(*mesh, face.cFEp(j));
			for (int k = 0; k < 3; ++k) {
				if (ep->cV(0) == vps[k]) {
					flags[ei] = flags[ei] ^ (ep->cV(1) == vps[(k + 1) % 3]);
					break;
				}
			}
		}
	}

	// Adjusts flags for boundary edges.
	int count = 0;
	for (size_t i = 0; i < mesh->en; ++i) {
		if (mesh->edge[i].IsB()) flags[i] = true;
		if (!flags[i]) ++count;
	}

	if (!count) return (false);

	// Computes accumulative normal.
	pcvcg::Point3 normal(0.0, 0.0, 0.0);
	for (size_t i = 0; i < mesh->fn; ++i) {
		normal += mesh->face[i].cN() * ((mesh->face[i].cP(1) - mesh->face[i].cP(0)) ^ (mesh->face[i].cP(2) - mesh->face[i].cP(0))).Norm();
	}
	normal.Normalize();

	// Traverses mesh and fix normals.
	std::vector<bool> visited(mesh->fn, false);

	while (true) {

		std::queue<pcvcg::TriMesh::FacePointer> queue;

		for (size_t i = 0; i < mesh->fn; ++i) {
			pcvcg::TriMesh::FacePointer fp = &(mesh->face[i]);
			for (int j = 0; j < 3; ++j) {
				pcvcg::TriMesh::EdgePointer ep = fp->FEp(j);
				int ei = vcg::tri::Index(*mesh, ep);
				if (!flags[ei]) {
					real_t fn = fp->cN() * normal;
					real_t ffn = fp->FFp(j)->cN() * normal;
					if (fn > ffn) queue.push(fp->FFp(j));
					else queue.push(fp);
					break;
				}
			}
			if (!queue.empty()) break;
		}

		if (queue.empty()) break;

		while (!queue.empty()) {

			pcvcg::TriMesh::FacePointer fp = queue.front();
			int fi = vcg::tri::Index(*mesh, fp);
			queue.pop();

			if (visited[fi]) continue;
			std::swap(fp->V(1), fp->V(2));
			visited[fi] = true;

			for (int i = 0; i < 3; ++i) {
				int ei = vcg::tri::Index(*mesh, fp->FEp(i));
				if (mesh->edge[ei].IsB()) continue;
				if (!(flags[ei] = !flags[ei])) queue.push(fp->FFp(i));
			}
		}
	}

	return (true);
}

void ModelCanonicalizer::reverseWindings(Model *model) {

	if (!model || !model->mesh()) return;
	pcvcg::TriMesh *mesh = model->mesh();

	for (size_t i = 0; i < mesh->fn; ++i) {
		pcvcg::TriMesh::FacePointer fp = &(mesh->face[i]);
		std::swap(fp->V(1), fp->V(2));
	}
}

void ModelCanonicalizer::copyTopologies(Model *source, Model *target) {

	if (!source || !source->mesh() || !target || !target->mesh()) return;

	if (source->mesh()->vn != target->mesh()->vn) {
		_VGR_SYSTEM_ERR("The vertices numbers of source and target meshes are not identical.");
		return;
	}

	Model *temp = new Model();

	vcg::tri::Append<pcvcg::TriMesh, pcvcg::TriMesh>::MeshCopy(*temp->mesh(), *source->mesh());
	for (size_t i = 0; i < target->mesh()->vn; ++i) temp->mesh()->vert[i].P() = target->mesh()->vert[i].P();
	vcg::tri::Append<pcvcg::TriMesh, pcvcg::TriMesh>::MeshCopy(*target->mesh(), *temp->mesh());
}



#pragma warning (pop)
