#include <map>
#include <string>
#include <vector>
#include<fstream>
#include<iostream>
#include <cassert>

namespace my_loader {
#define IS_SPACE(x) (((x) == ' ') || ((x) == '\t'))
#define IS_DIGIT(x) \
  (static_cast<unsigned int>((x) - '0') < static_cast<unsigned int>(10))
#define IS_NEW_LINE(x) (((x) == '\r') || ((x) == '\n') || ((x) == '\0'))

	typedef float real_t;

	// Vertex attributes
	typedef struct {
		std::vector<real_t> vertices;   // 'v'
		std::vector<real_t> normals;    // 'vn'
		std::vector<real_t> texcoords;  // 'vt'
	} attrib_t;

	typedef struct {
		int vertex_index;
		int normal_index;
		int texcoord_index;
	} index_t;

	typedef struct {
		std::vector<index_t> indices;
		std::vector<unsigned char> num_face_vertices;  // The number of vertices per face. 3 = polygon, 4 = quad, Up to 255.
	} mesh_t;

	typedef struct {
		std::string name;
		mesh_t mesh;
	} shape_t;

	struct vertex_index {
		int v_idx, vt_idx, vn_idx;
		vertex_index() : v_idx(-1), vt_idx(-1), vn_idx(-1) {}
		explicit vertex_index(int idx) : v_idx(idx), vt_idx(idx), vn_idx(idx) {}
		vertex_index(int vidx, int vtidx, int vnidx)
			: v_idx(vidx), vt_idx(vtidx), vn_idx(vnidx) {}
	};

	//transmit functions

	static bool tryParseDouble(const char* s, const char* s_end, double* result) {
		if (s >= s_end) {
			return false;
		}

		double mantissa = 0.0;
		// This exponent is base 2 rather than 10.
		// However the exponent we parse is supposed to be one of ten,
		// thus we must take care to convert the exponent/and or the
		// mantissa to a * 2^E, where a is the mantissa and E is the
		// exponent.
		// To get the final double we will use ldexp, it requires the
		// exponent to be in base 2.
		int exponent = 0;

		// NOTE: THESE MUST BE DECLARED HERE SINCE WE ARE NOT ALLOWED
		// TO JUMP OVER DEFINITIONS.
		char sign = '+';
		char exp_sign = '+';
		char const* curr = s;

		// How many characters were read in a loop.
		int read = 0;
		// Tells whether a loop terminated due to reaching s_end.
		bool end_not_reached = false;

		/*
				BEGIN PARSING.
		*/

		// Find out what sign we've got.
		if (*curr == '+' || *curr == '-') {
			sign = *curr;
			curr++;
		}
		else if (IS_DIGIT(*curr)) { /* Pass through. */
		}
		else {
			goto fail;
		}

		// Read the integer part.
		end_not_reached = (curr != s_end);
		while (end_not_reached && IS_DIGIT(*curr)) {
			mantissa *= 10;
			mantissa += static_cast<int>(*curr - 0x30);
			curr++;
			read++;
			end_not_reached = (curr != s_end);
		}

		// We must make sure we actually got something.
		if (read == 0) goto fail;
		// We allow numbers of form "#", "###" etc.
		if (!end_not_reached) goto assemble;

		// Read the decimal part.
		if (*curr == '.') {
			curr++;
			read = 1;
			end_not_reached = (curr != s_end);
			while (end_not_reached && IS_DIGIT(*curr)) {
				static const double pow_lut[] = {
					1.0, 0.1, 0.01, 0.001, 0.0001, 0.00001, 0.000001, 0.0000001,
				};
				const int lut_entries = sizeof pow_lut / sizeof pow_lut[0];

				// NOTE: Don't use powf here, it will absolutely murder precision.
				mantissa += static_cast<int>(*curr - 0x30) *
					(read < lut_entries ? pow_lut[read] : std::pow(10.0, -read));
				read++;
				curr++;
				end_not_reached = (curr != s_end);
			}
		}
		else if (*curr == 'e' || *curr == 'E') {
		}
		else {
			goto assemble;
		}

		if (!end_not_reached) goto assemble;

		// Read the exponent part.
		if (*curr == 'e' || *curr == 'E') {
			curr++;
			// Figure out if a sign is present and if it is.
			end_not_reached = (curr != s_end);
			if (end_not_reached && (*curr == '+' || *curr == '-')) {
				exp_sign = *curr;
				curr++;
			}
			else if (IS_DIGIT(*curr)) { /* Pass through. */
			}
			else {
				// Empty E is not allowed.
				goto fail;
			}

			read = 0;
			end_not_reached = (curr != s_end);
			while (end_not_reached && IS_DIGIT(*curr)) {
				exponent *= 10;
				exponent += static_cast<int>(*curr - 0x30);
				curr++;
				read++;
				end_not_reached = (curr != s_end);
			}
			exponent *= (exp_sign == '+' ? 1 : -1);
			if (read == 0) goto fail;
		}

	assemble:
		*result =
			(sign == '+' ? 1 : -1) *
			(exponent ? std::ldexp(mantissa * std::pow(5.0, exponent), exponent) : mantissa);
		return true;
	fail:
		return false;
	}

	static inline real_t parseReal(const char** token, double default_value = 0.0) {
		(*token) += strspn((*token), " \t");
		const char* end = (*token) + strcspn((*token), " \t\r");
		double val = default_value;
		tryParseDouble((*token), end, &val);
		real_t f = static_cast<real_t>(val);
		(*token) = end;
		return f;
	}

	static inline void parseReal2(real_t* x, real_t* y, const char** token,
		const double default_x = 0.0,
		const double default_y = 0.0) {
		(*x) = parseReal(token, default_x);
		(*y) = parseReal(token, default_y);
	}

	static inline void parseReal3(real_t* x, real_t* y, real_t* z, const char** token,
		const double default_x = 0.0,
		const double default_y = 0.0,
		const double default_z = 0.0) {
		(*x) = parseReal(token, default_x);
		(*y) = parseReal(token, default_y);
		(*z) = parseReal(token, default_z);
	}

	static inline int fixIndex(int idx, int n) {
		if (idx > 0) return idx - 1;
		if (idx == 0) return 0;
		return n + idx;  // negative value = relative
	}

	static vertex_index parseRawTriple(const char** token) {
		vertex_index vi(static_cast<int>(0));  // 0 is an invalid index in OBJ

		vi.v_idx = atoi((*token));
		(*token) += strcspn((*token), "/ \t\r");
		if ((*token)[0] != '/') {
			return vi;
		}
		(*token)++;

		// i//k
		if ((*token)[0] == '/') {
			(*token)++;
			vi.vn_idx = atoi((*token));
			(*token) += strcspn((*token), "/ \t\r");
			return vi;
		}

		// i/j/k or i/j
		vi.vt_idx = atoi((*token));
		(*token) += strcspn((*token), "/ \t\r");
		if ((*token)[0] != '/') {
			return vi;
		}

		// i/j/k
		(*token)++;  // skip '/'
		vi.vn_idx = atoi((*token));
		(*token) += strcspn((*token), "/ \t\r");
		return vi;
	}

	static vertex_index parseTriple(const char** token, int vsize, int vnsize,
		int vtsize) {
		vertex_index vi(-1);

		vi.v_idx = fixIndex(atoi((*token)), vsize);
		(*token) += strcspn((*token), "/ \t\r");
		if ((*token)[0] != '/') {
			return vi;
		}
		(*token)++;

		// i//k
		if ((*token)[0] == '/') {
			(*token)++;
			vi.vn_idx = fixIndex(atoi((*token)), vnsize);
			(*token) += strcspn((*token), "/ \t\r");
			return vi;
		}

		// i/j/k or i/j
		vi.vt_idx = fixIndex(atoi((*token)), vtsize);
		(*token) += strcspn((*token), "/ \t\r");
		if ((*token)[0] != '/') {
			return vi;
		}

		// i/j/k
		(*token)++;  // skip '/'
		vi.vn_idx = fixIndex(atoi((*token)), vnsize);
		(*token) += strcspn((*token), "/ \t\r");
		return vi;
	}

	//将曲面组中的曲面信息传递给shape
	static bool exportFaceGroupToShape(shape_t* shape, const std::vector<std::vector<vertex_index> >& faceGroup) {
		if (faceGroup.empty()) {
			return false;
		}

		// Flatten vertices and indices
		for (size_t i = 0; i < faceGroup.size(); i++) {
			const std::vector<vertex_index>& face = faceGroup[i];

			vertex_index i0 = face[0];
			vertex_index i1(-1);
			vertex_index i2 = face[1];

			size_t npolys = face.size();

			for (size_t k = 2; k < npolys; k++) {
				i1 = i2;
				i2 = face[k];

				index_t idx0, idx1, idx2;
				idx0.vertex_index = i0.v_idx;
				idx0.normal_index = i0.vn_idx;
				idx0.texcoord_index = i0.vt_idx;
				idx1.vertex_index = i1.v_idx;
				idx1.normal_index = i1.vn_idx;
				idx1.texcoord_index = i1.vt_idx;
				idx2.vertex_index = i2.v_idx;
				idx2.normal_index = i2.vn_idx;
				idx2.texcoord_index = i2.vt_idx;

				shape->mesh.indices.push_back(idx0);
				shape->mesh.indices.push_back(idx1);
				shape->mesh.indices.push_back(idx2);

				shape->mesh.num_face_vertices.push_back(3);
			}
		}
		return true;
	}

	bool LoadObj(attrib_t* attrib, std::vector<shape_t>* shapes, const char* filename)
	{
		attrib->vertices.clear();
		attrib->normals.clear();
		attrib->texcoords.clear();
		shapes->clear();

		std::vector<real_t> v;
		std::vector<real_t> vn;
		std::vector<real_t> vt;
		std::vector<std::vector<vertex_index> > faceGroup;
		shape_t shape;

		std::ifstream ifs(filename);
		if (!ifs) {
			std::cout << "Cannot open file [" << filename << "]" << std::endl;
			return false;
		}

		std::string linebuf;
		while (ifs.peek() != -1) {
			getline(ifs, linebuf);

			// Trim newline '\r\n' or '\n'
			if (linebuf.size() > 0) {
				if (linebuf[linebuf.size() - 1] == '\n')
					linebuf.erase(linebuf.size() - 1);
			}
			if (linebuf.size() > 0) {
				if (linebuf[linebuf.size() - 1] == '\r')
					linebuf.erase(linebuf.size() - 1);
			}

			// Skip if empty line.
			if (linebuf.empty()) {
				continue;
			}

			const char* token = linebuf.c_str();
			token += strspn(token, " \t");

			assert(token);
			if (token[0] == '\0') continue;  // empty line
			if (token[0] == '#') continue;   // comment line

				// vertex
			if (token[0] == 'v' && IS_SPACE((token[1]))) {
				token += 2;
				real_t x, y, z;
				parseReal3(&x, &y, &z, &token);
				v.push_back(x);
				v.push_back(y);
				v.push_back(z);
				continue;
			}

			// normal
			if (token[0] == 'v' && token[1] == 'n' && IS_SPACE((token[2]))) {
				token += 3;
				real_t x, y, z;
				parseReal3(&x, &y, &z, &token);
				vn.push_back(x);
				vn.push_back(y);
				vn.push_back(z);
				continue;
			}

			// texcoord
			if (token[0] == 'v' && token[1] == 't' && IS_SPACE((token[2]))) {
				token += 3;
				real_t x, y;
				parseReal2(&x, &y, &token);
				vt.push_back(x);
				vt.push_back(y);
				continue;
			}

			if (token[0] == 'f' && IS_SPACE((token[1])))
			{
				token += 2;
				token += strspn(token, " \t");

				std::vector<vertex_index> face;
				face.reserve(3);

				while (!IS_NEW_LINE(token[0])) {
					vertex_index vi = parseTriple(&token, static_cast<int>(v.size() / 3),
						static_cast<int>(vn.size() / 3),
						static_cast<int>(vt.size() / 2));
					face.push_back(vi);
					size_t n = strspn(token, " \t\r");
					token += n;
				}

				// replace with emplace_back + std::move on C++11
				faceGroup.push_back(std::vector<vertex_index>());
				faceGroup[faceGroup.size() - 1].swap(face);

				continue;
			}
		}

		bool ret = exportFaceGroupToShape(&shape, faceGroup);
		// exportFaceGroupToShape return false when `usemtl` is called in the last
		// line.
		// we also add `shape` to `shapes` when `shape.mesh` has already some
		// faces(indices)
		if (ret || shape.mesh.indices.size()) {
			shapes->push_back(shape);
		}
		faceGroup.clear();  // for safety


		attrib->vertices.swap(v);
		attrib->normals.swap(vn);
		attrib->texcoords.swap(vt);
	}
}

