#include "util.hpp"



_VGR_BEGIN
_VGR_ALGO_BEGIN



real_t Util::Epsilon = VGR_REAL_EPS * 1.0e6;

///	<description>
///	Stores coordinates of vertices of the specific mesh to a matrix.
///	</description>
///	<parameters>
///		<param name='mesh'> The specific mesh. </param>
///		<param name='mat' direction='out'> The matrix stores the coordinates of vertices. </param>
///	</parameters>
void
Util::toMatrix(
		const pcvcg::TriMesh &mesh,
		eigen::Matrix3xN *matrix) {

	matrix->resize(3, mesh.vn);
	for (size_t i = 0; i < mesh.vert.size(); ++i) {
		matrix->col(i) = Util::toVector(mesh.vert[i].P());
	}
}

///	<description>
///	Stores coordinates of vertices whose indices are specified by the given
///	indices vector to a matrix.
///	</description>
///	<parameters>
///		<param name='mesh'> The specific mesh. </param>
///		<param name='vert_idx'> The vertices indices vector. </param>
///		<param name='mat' direction='out'> The matrix stores the coordinates of vertices. </param>
///	</parameters>
void
Util::toMatrix(
		const pcvcg::TriMesh &mesh,
		const std::vector<size_t> &indices,
		eigen::Matrix3xN *matrix) {

	matrix->resize(3, indices.size());
	for (size_t i = 0; i < indices.size(); ++i) {
		matrix->col(i) = Util::toVector(mesh.vert[indices[i]].P());
	}
}

///	<description>
///	Assigns columns of a matrix to the coordinates of vertices of the specific
///	mesh.
///	</description>
///	<parameters>
///		<param name='mat'> The matrix stores the coordinates of vertices. </param>
///		<param name='mesh' direction='out'> The specific mesh. </param>
///	</parameters>
void
Util::toMesh(
		const eigen::Matrix3xN &matrix,
		pcvcg::TriMesh *mesh) {

	for (int i = 0; i < matrix.cols(); ++i) {
		mesh->vert[i].P() = Util::toPoint(matrix.col(i));
	}
}

///	<description>
///	Gets one-ring neighboring vertices of the given vertex.
///	</description>
///	<parameters>
///		<param name='mesh'> The input mesh. </param>
///		<param name='index'> Index of central vertex. </param>
///		<param name='neighbors' direction='out'> Neighboring vertices. </param>
///	</parameters>
void
Util::oneRing(
		const pcvcg::TriMesh &mesh,
		size_t index,
		std::vector<size_t> *neighbors) {

	const pcvcg::TriMesh::VertexType &v(mesh.vert[index]);
	pcvcg::TriMesh::VertexPointer vp = &const_cast<pcvcg::TriMesh::VertexType &>(v);
	pcvcg::Pos pos(vp->VFp(), vp);
	if (v.IsB()) {
		while (!pos.IsBorder()) pos.NextE();
		do {
			neighbors->push_back(vcg::tri::Index(mesh, pos.VFlip()));
			pos.NextE();
		} while (!pos.IsBorder());
		neighbors->push_back(vcg::tri::Index(mesh, pos.VFlip()));
	} else {
		pcvcg::TriMesh::FacePointer start = pos.F();
		do {
			neighbors->push_back(vcg::tri::Index(mesh, pos.VFlip()));
			pos.NextE();
		} while (pos.F() != start);
	}
}



_VGR_ALGO_END
_VGR_END
