#pragma once
#ifndef _VGR_COMMON_3RDPARTY_VCGLIB_HPP
#define _VGR_COMMON_3RDPARTY_VCGLIB_HPP

#pragma warning (push)
#pragma warning (disable:4002)	/* too many actual parameters for macro 'identifier' */
#pragma warning (disable:4244)	/* conversion from 'type1' to 'type2', possible loss of data */
#pragma warning (disable:4819)	/* contains characters that cannot be represented in the current code page */
#pragma warning (disable:4996)	/* function was declared deprecated */

#include <common/global.hpp>
#include <common/3rdparty/3rdparty.hpp>
#include <vcglib/wrap/callback.h>
#include <vcglib/vcg/simplex/face/topology.h>
#include <vcglib/vcg/simplex/face/pos.h>
#include <vcglib/vcg/simplex/face/jumping_pos.h>
#include <vcglib/vcg/complex/complex.h>
#include <vcglib/vcg/complex/append.h>
#include <vcglib/vcg/complex/algorithms/clean.h>
#include <vcglib/vcg/complex/algorithms/update/bounding.h>
#include <vcglib/vcg/complex/algorithms/update/normal.h>
#include <vcglib/vcg/complex/algorithms/update/topology.h>
#include <vcglib/vcg/complex/algorithms/update/curvature.h>
#include <vcglib/vcg/space/index/aabb_binary_tree/aabb_binary_tree.h>

#pragma warning (pop)

#ifdef static_assert
#undef static_assert
#endif

#define _VGR_VCG						vgr::pcvcg
#define _VGR_VCG_BEGIN					namespace pcvcg {
#define _VGR_VCG_END					}



_VGR_BEGIN
_VGR_VCG_BEGIN



struct UsedTypes;
class Vertex;
class Face;
class Edge;
class TriMesh;



struct UsedTypes :
		public vcg::UsedTypes<
				vcg::Use<Vertex>::AsVertexType,
				vcg::Use<Edge>::AsEdgeType,
				vcg::Use<Face>::AsFaceType> { };



class Vertex :
		public vcg::Vertex<
				UsedTypes,
				vcg::vertex::Coord3d,
				vcg::vertex::Normal3d,
				vcg::vertex::Color4b,
				vcg::vertex::Qualityd,
				vcg::vertex::Curvatured,
				vcg::vertex::CurvatureDird,
				vcg::vertex::BitFlags,
				vcg::vertex::VFAdj> { };



class Face :
		public vcg::Face<
				UsedTypes,
				vcg::face::Normal3d,
				vcg::face::Color4b,
				vcg::face::WedgeTexCoord2d,
				vcg::face::BitFlags,
				vcg::face::VertexRef,
				vcg::face::VFAdj,
				vcg::face::FFAdj,
				vcg::face::FEAdj> { };



class Edge :
		public vcg::Edge<
				UsedTypes,
				vcg::edge::BitFlags,
				vcg::edge::EVAdj> { };



class TriMesh : public vcg::tri::TriMesh<std::vector<Vertex>, std::vector<Face>, std::vector<Edge> > {

public:
	typedef vcg::tri::TriMesh<std::vector<Vertex>, std::vector<Face>, std::vector<Edge> >
												Base;
	typedef Base::MeshType						MeshType;
	typedef Base::ScalarType					ScalarType;
	typedef Base::VertexType					VertexType;
	typedef Base::VertexPointer					VertexPointer;
	typedef Base::VertexIterator				VertexIterator;
	typedef Base::ConstVertexPointer			ConstVertexPointer;
	typedef Base::ConstVertexIterator			ConstVertexIterator;
	typedef Base::FaceType						FaceType;
	typedef Base::FacePointer					FacePointer;
	typedef Base::FaceIterator					FaceIterator;
	typedef Base::ConstFacePointer				ConstFacePointer;
	typedef Base::ConstFaceIterator				ConstFaceIterator;
	typedef Base::EdgeType						EdgeType;
	typedef Base::EdgePointer					EdgePointer;
	typedef Base::EdgeIterator					EdgeIterator;
};



template <typename UpdateMeshType>
class UpdateTopology : public vcg::tri::UpdateTopology<UpdateMeshType> {

public:
	typedef vcg::tri::UpdateTopology<UpdateMeshType>
												Base;
	typedef typename Base::MeshType				MeshType;
	typedef typename Base::VertexType			VertexType;
	typedef typename Base::VertexPointer		VertexPointer;
	typedef typename Base::VertexIterator		VertexIterator;
	typedef typename Base::FaceType				FaceType;
	typedef typename Base::FacePointer			FacePointer;
	typedef typename Base::FaceIterator			FaceIterator;
	typedef typename MeshType::EdgeType			EdgeType;
	typedef typename MeshType::EdgePointer		EdgePointer;
	typedef typename MeshType::EdgeIterator		EdgeIterator;

public:
	using Base::FillEdgeVector;
	static void FillEdgeVector(MeshType &m) {
		assert(vcg::tri::HasFFAdjacency(m));
		vcg::tri::UpdateFlags<MeshType>::FaceClearV(m);
		size_t count = 0;
		for (FaceIterator fi = m.face.begin(); fi != m.face.end(); ++fi) {
			if (fi->IsD()) continue;
			for (int i = 0; i < 3; ++i) {
				if (!fi->FFp(i)->IsV()) ++count;
			}
			fi->SetV();
		}
		m.en = count;
		m.edge.resize(count);
		vcg::tri::UpdateFlags<MeshType>::FaceClearV(m);
	}
	static void EdgeVertex(MeshType &m) {
		if (!vcg::tri::HasEVAdjacency(m)) return;
		assert(vcg::tri::HasFFAdjacency(m));
		vcg::tri::UpdateFlags<MeshType>::FaceClearV(m);
		EdgeIterator ei(m.edge.begin());
		for (FaceIterator fi = m.face.begin(); fi != m.face.end(); ++fi) {
			if (fi->IsD()) continue;
			for (int i = 0; i < 3; ++i) {
				if (fi->FFp(i)->IsV()) continue;
				VertexPointer vp0 = fi->V(i), vp1 = fi->V((i + 1) % 3);
				if (vp0 > vp1) std::swap(vp0, vp1);
				while (ei->IsD()) ++ei;
				ei->V(0) = vp0;
				ei->V(1) = vp1;
				++ei;
			}
			fi->SetV();
		}
		vcg::tri::UpdateFlags<MeshType>::FaceClearV(m);
	}
	static void FaceEdge(MeshType &m) {
		if (!vcg::tri::HasFEAdjacency(m)) return;
		assert(vcg::tri::HasFFAdjacency(m));
		vcg::tri::UpdateFlags<MeshType>::FaceClearV(m);
		EdgeIterator ei(m.edge.begin());
		for (FaceIterator fi = m.face.begin(); fi != m.face.end(); ++fi) {
			if (fi->IsD()) continue;
			for (int i = 0; i < 3; ++i) {
				if (!fi->FFp(i)->IsV()) {
					while (ei->IsD()) ++ei;
					fi->FEp(i) = fi->FFp(i)->FEp(fi->FFi(i)) = &(*ei);
					++ei;
				}
			}
			fi->SetV();
		}
		vcg::tri::UpdateFlags<MeshType>::FaceClearV(m);
	}
};



template <typename UpdateMeshType>
class UpdateFlags : public vcg::tri::UpdateFlags<UpdateMeshType> {

public:
	typedef vcg::tri::UpdateFlags<UpdateMeshType>
												Base;
	typedef typename Base::MeshType				MeshType;
	typedef typename Base::VertexType			VertexType;
	typedef typename Base::VertexPointer		VertexPointer;
	typedef typename Base::VertexIterator		VertexIterator;
	typedef typename Base::FaceType				FaceType;
	typedef typename Base::FacePointer			FacePointer;
	typedef typename Base::FaceIterator			FaceIterator;
	typedef typename MeshType::EdgeType			EdgeType;
	typedef typename MeshType::EdgePointer		EdgePointer;
	typedef typename MeshType::EdgeIterator		EdgeIterator;

public:
	static void EdgeBorderFromFace(MeshType &m) {
		assert(vcg::tri::HasPerFaceFlags(m));
		for (FaceIterator fi = m.face.begin(); fi != m.face.end(); ++fi) {
			if (fi->IsD()) continue;
			for (int i = 0; i < fi->VN(); ++i) {
				if (fi->IsB(i)) fi->FEp(i)->SetB();
				else fi->FEp(i)->ClearB();
			}
		}
	}
};



typedef vcg::EmptyClass					EmptyClass;
typedef vcg::GetPointerFunctor			GetPointerFunctor;



class GetBox3Functor;
class GetBarycenter3Functor;



class GetBox3Functor {

public:
	template <typename ObjectType, typename ScalarType>
	void operator ()(const ObjectType &obj, vcg::Box3<ScalarType> &box) const {
		vcg::Box3<typename ObjectType::ScalarType> tb;
		obj.GetBBox(tb);
		box.Import(tb);
	}
	template <typename ObjectType>
	void operator ()(const ObjectType &obj, vcg::Box3<typename ObjectType::ScalarType> &box) const {
		obj.GetBBox(box);
	}
};



class GetBarycenter3Functor {

public:
	template <typename ObjectType, typename ScalarType>
	void operator ()(const ObjectType &obj, vcg::Point3<ScalarType> &bar) const {
		bar.Import(vcg::Barycenter<ObjectType>(obj));
	}
	template <typename ObjectType>
	void operator ()(const ObjectType &obj, vcg::Point3<typename ObjectType::ScalarType> &bar) const {
		bar = vcg::Barycenter<ObjectType>(obj);
	}
};



typedef vcg::Point3d															Point3;
typedef vcg::Box3d																Box3;
typedef vcg::Ray3d																Ray3;
typedef vcg::Line3d																Line3;
typedef vcg::Color4b															Color4;
typedef vcg::face::Pos<TriMesh::FaceType>										Pos;
typedef vcg::face::VFIterator<TriMesh::FaceType>								VFIterator;
typedef vcg::AABBBinaryTreeIndex<TriMesh::FaceType, real_t>						TreeIndex;
typedef vcg::face::PointDistanceBaseFunctor<pcvcg::TreeIndex::ScalarType>		PointDistanceBaseFunctor;
typedef vcg::face::PointNormalDistanceFunctor<pcvcg::TreeIndex::ScalarType>		PointNormalDistanceFunctor;
typedef vcg::RayTriangleIntersectionFunctor<true>								RayTriangleIntersectionFunctor;



_VGR_VCG_END
_VGR_END



#endif /* _VGR_COMMON_3RDPARTY_VCGLIB_HPP */
