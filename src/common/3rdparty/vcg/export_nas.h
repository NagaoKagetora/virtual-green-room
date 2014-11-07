#pragma once
#ifndef __VCGLIB_EXPORT_NAS
#define __VCGLIB_EXPORT_NAS

#include <cstdio>



namespace vcg {
namespace tri {
namespace io {



template <class OpenMeshType>
class ExporterNAS {

public:
	typedef typename OpenMeshType::ScalarType			ScalarType;
	typedef typename OpenMeshType::VertexIterator		VertexIterator;
	typedef typename OpenMeshType::FaceIterator			FaceIterator;

public:
	static int Save(OpenMeshType &mesh, const char *filename, CallBackPos *cb = 0) {
		return (Save(mesh, filename, 0, cb));
	}
	static int Save(OpenMeshType &mesh, const char *filename, const int mask, CallBackPos *cb = 0) {
		
		FILE *fp = fopen(filename, "w");
		if (!fp) return (-1);

		std::fprintf(fp, "$ VN: %d\n", mesh.vn);
		std::fprintf(fp, "$ FN: %d\n", mesh.fn);

		std::fprintf(fp, "BEGIN BULK\n");

		for (size_t i = 0; i < mesh.vn; ++i) {
			std::fprintf(fp, "GRID*   %16d                %16e%16eG%07d\nG%07d%16e\n",
					i,
					mesh.vert[i].P()[0],
					mesh.vert[i].P()[1],
					i,
					i,
					mesh.vert[i].P()[2]);
		}

		for (size_t i = 0; i < mesh.fn; ++i) {
			std::fprintf(fp, "CTRIA3  %8d        %8d%8d%8d\n",
					i,
					vcg::tri::Index(mesh, mesh.face[i].cV(0)),
					vcg::tri::Index(mesh, mesh.face[i].cV(1)),
					vcg::tri::Index(mesh, mesh.face[i].cV(2)));
		}

		std::fprintf(fp, "ENDDATA\n");

		std::fclose(fp);

		return (0);
	}
};



}
}
}



#endif /* __VCGLIB_EXPORT_NAS */
