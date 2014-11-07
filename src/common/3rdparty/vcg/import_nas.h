#pragma once
#ifndef __VCGLIB_IMPORT_NAS
#define __VCGLIB_IMPORT_NAS

#include <cctype>
#include <cstdio>
#include <vcg/complex/allocate.h>

#pragma warning (push)
#pragma warning (disable:4482)	/* nonstandard extension used: enum 'enum' used in qualified name */

#define _SUPPORT_NASTRAN_FLOAT_FORMAT 1



namespace vcg {
namespace tri {
namespace io {



template <class OpenMeshType>
class ImporterNAS {

public:
	typedef typename OpenMeshType::ScalarType			ScalarType;
	typedef typename OpenMeshType::VertexIterator		VertexIterator;
	typedef typename OpenMeshType::FaceIterator			FaceIterator;

private:
	struct VertexData {

	public:
		double							x;
		double							y;
		double							z;
		int								index;

	public:
		VertexData(double x, double y, double z, int index = 0) : x(x), y(y), z(z), index(index) { }

	public:
		bool operator <(const VertexData &vertex) const { return (this->index < vertex.index); }
		bool operator >(const VertexData &vertex) const { return (this->index > vertex.index); }
		bool operator ==(const VertexData &vertex) const { return (this->index == vertex.index); }
	};

	struct TriangleData {

	public:
		int								a;
		int								b;
		int								c;
		int								index;

	public:
		TriangleData(int a, int b, int c, int index) : a(a), b(b), c(c), index(index) { }

	public:
		bool operator <(const TriangleData &triangle) const { return (this->index < triangle.index); }
		bool operator >(const TriangleData &triangle) const { return (this->index > triangle.index); }
		bool operator ==(const TriangleData &triangle) const { return (this->index == triangle.index); }
	};

	enum Format {
		Anomalous = -1,
		Undetermined = 0,
		Large,
		Small,
		Free
	};

private:
	static const int					KEYWORD_SIZE = 8;
	static const int					BUFFER_SIZE = 1024;
	static const int					FIELDS_SIZE = 4096;
	static const int					FIELD_WIDTH = 32;
	static const char					SPLITTER = ',';

public:
	static int Open(OpenMeshType &mesh, const char *filename, CallBackPos *cb = 0) {
		int dummymask = 0;
		return (Open(mesh, filename, dummymask, cb));
	}
	static int Open(OpenMeshType &mesh, const char *filename, int &loadmask, CallBackPos *cb = 0) {

		std::vector<VertexData> vertices;
		std::vector<TriangleData> triangles;

		if (int ret = read(filename, vertices, triangles)) return (ret);

		mesh.Clear();

		if (vertices.empty() || triangles.empty()) return (0);

		// Constructing indexing.
		std::sort(vertices.begin(), vertices.end());
		int base = vertices.front().index;
		std::vector<int> indices(vertices.back().index - base + 1, -1);
		for (size_t i = 0, c = 0; i < vertices.size(); ++i) {
			indices[vertices[i].index - base] = i;
		}

		VertexIterator vi = Allocator<OpenMeshType>::AddVertices(mesh, vertices.size());
		for (std::vector<VertexData>::const_iterator iter = vertices.begin(); iter != vertices.end(); ++iter) {
			vi->P()[0] = (ScalarType)iter->x;
			vi->P()[1] = (ScalarType)iter->y;
			vi->P()[2] = (ScalarType)iter->z;
			++vi;
		}

		FaceIterator fi = Allocator<OpenMeshType>::AddFaces(mesh, triangles.size());
		for (std::vector<TriangleData>::const_iterator iter = triangles.begin(); iter != triangles.end(); ++iter) {
			fi->V(0) = &(mesh.vert[indices[iter->a - base]]);
			fi->V(1) = &(mesh.vert[indices[iter->b - base]]);
			fi->V(2) = &(mesh.vert[indices[iter->c - base]]);
			++fi;
		}

		return (0);
	}

private:
	static int read(const char *name, std::vector<VertexData> &vertices, std::vector<TriangleData> &triangles) {

		FILE *fp = std::fopen(name, "r");
		if (!fp) return (-1);

		vertices.clear();
		triangles.clear();

		char buffer[BUFFER_SIZE];
		char fields[FIELDS_SIZE];

		Format format = Format::Undetermined;

		while (true) {

			// Clears buffer and fetches new line.
			std::memset(buffer, 0, BUFFER_SIZE);
			std::memset(fields, 0, FIELDS_SIZE);
			std::fgets(buffer, BUFFER_SIZE, fp);

			if (std::feof(fp)) break;

			// Skips comments.
			if (buffer[0] == '$') continue;

			// Parses data.
			if (!std::strncmp(buffer, "GRID", 4)) {

				switch (getFormat(buffer)) {
				case Format::Large:
					extractLargeFields(fields, getFixedBulk(buffer, fp));
					break;
				case Format::Small:
					extractSmallFields(fields, getFixedBulk(buffer, fp));
					break;
				case Format::Free:
					extractFreeFields(fields, getFreeBulk(buffer, fp));
					break;
				default:
					return (-1);
				}

				vertices.push_back(VertexData(toDouble(fields, 2), toDouble(fields, 3), toDouble(fields, 4), toInt(fields, 0)));

			} else if (!std::strncmp(buffer, "CQUAD4", 6)) {

				switch (getFormat(buffer)) {
				case Format::Large:
					extractLargeFields(fields, getFixedBulk(buffer, fp));
					break;
				case Format::Small:
					extractSmallFields(fields, getFixedBulk(buffer, fp));
					break;
				case Format::Free:
					extractFreeFields(fields, getFreeBulk(buffer, fp));
					break;
				default:
					return (-1);
				}

				triangles.push_back(TriangleData(toInt(fields, 2), toInt(fields, 3), toInt(fields, 4), toInt(fields, 0)));
				triangles.push_back(TriangleData(toInt(fields, 4), toInt(fields, 5), toInt(fields, 2), toInt(fields, 0)));

			} else if (!std::strncmp(buffer, "CTRIA3", 6)) {

				switch (getFormat(buffer)) {
				case Format::Large:
					extractLargeFields(fields, getFixedBulk(buffer, fp));
					break;
				case Format::Small:
					extractSmallFields(fields, getFixedBulk(buffer, fp));
					break;
				case Format::Free:
					extractFreeFields(fields, getFreeBulk(buffer, fp));
					break;
				default:
					return (-1);
				}

				triangles.push_back(TriangleData(toInt(fields, 2), toInt(fields, 3), toInt(fields, 4), toInt(fields, 0)));

			}
		}

		std::fclose(fp);

		return (0);
	}
	static inline int toInt(const char *fields, int index) {
		return (std::atoi(fields + FIELD_WIDTH * index));
	}
	static inline double toDouble(const char *fields, int index) {
#if _SUPPORT_NASTRAN_FLOAT_FORMAT
		return (toDouble(fields + FIELD_WIDTH * index));
#else
		return (std::atof(fields + FIELD_WIDTH * index));
#endif
	}
	static inline double toDouble(const char *str) {
		while (*str == ' ') ++str;
		char buf[FIELD_WIDTH + 1];
		*buf = *str;
		int i = 1;
		for (const char *ptr = str + 1; *ptr; ++ptr) {
			if (std::toupper(*ptr) == 'E') return (std::atof(str));
			if (std::toupper(*ptr) == 'D') {
				buf[i++] = 'E';
				while (*(ptr++)) buf[i++] = (*ptr);
				break;
			} else {
				if ((*ptr == '+') || (*ptr == '-')) buf[i++] = 'E';
				buf[i++] = *ptr;
			}
		}
		buf[i] = 0;
		return (std::atof(buf));
	}
	static Format getFormat(const char *keyword) {
		for (int i = 0; i < KEYWORD_SIZE; ++i) {
			if (keyword[i] == '*') return (Format::Large);
			if (keyword[i] == ',') return (Format::Free);
		}
		return (Format::Small);
	}
	static char *getFixedBulk(char *buffer, FILE *fp) {
		char *head = buffer;
		bool initial = false;
		while (true) {
			if (initial && !std::fgets(buffer, BUFFER_SIZE, fp)) return (head);
			if (!buffer[80]) break;
			buffer += 80;
			initial = true;
		}
		return (head);
	}
	static char *getFreeBulk(char *buffer, FILE *fp) {
		char *head = buffer;
		bool initial = false;
		if (initial && !std::fgets(buffer, BUFFER_SIZE, fp)) return (head);
		return (head);
	}
	static char *extractLargeFields(char *fields, const char *buffer) {
		char *head = fields;
		while (true) {
			buffer += 8;
			for (int i = 0; i < 4; ++i) {
				std::strncpy(fields, buffer, 16);
				buffer += 16;
				if (!*buffer || (*buffer == '\n')) return (head);
				fields += 32;
			}
			buffer += 8;
		}
		return (head);
	}
	static char *extractSmallFields(char *fields, const char *buffer) {
		char *head = fields;
		while (true) {
			buffer += 8;
			for (int i = 0; i < 8; ++i) {
				std::strncpy(fields, buffer, 8);
				buffer += 8;
				if (!*buffer || (*buffer == '\n')) return (head);
				fields += 32;
			}
			buffer += 8;
		}
		return (head);
	}
	static char *extractFreeFields(char *fields, const char *buffer) {
		char *head = fields;
		int count = 0;
		while (*buffer != ',') ++buffer;
		while (true) {
			++buffer;
			while (*buffer != ',') {
				if (!*buffer || (*buffer == '\n')) return (head);
				*(fields++) = *(buffer++);
			}
			fields = head + (++count) * 32;
		}
		return (head);
	}
};



}
}
}



#pragma warning (pop)

#endif /* __VCGLIB_IMPORT_NAS */
