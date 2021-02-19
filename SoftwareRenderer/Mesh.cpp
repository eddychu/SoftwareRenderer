#include "Mesh.h"
#include <fstream>
#include <sstream>
#include <iostream>
Mesh::Mesh(const char* filepath, const char* name)
{
	std::ifstream in;
	std::string objFile = std::string(filepath) + "/" + std::string(name) + ".obj";
	in.open(objFile, std::ifstream::in);

	if (in.fail()) throw std::runtime_error("couldn't open file.\n");

	std::string line;

	while (!in.eof()) {
		std::getline(in, line);
		std::istringstream iss(line.c_str());
		char trash;
		if (!line.compare(0, 2, "v ")) {
			iss >> trash;
			float x, y, z;
			iss >> x >> y >> z;
			m_vertices.push_back(vec3(x, y, z));
		}
		else if (!line.compare(0, 3, "vn ")) {
			iss >> trash >> trash;
			float x, y, z;
			iss >> x >> y >> z;
			m_normals.push_back(vec3(x, y, z));
		}
		else if (!line.compare(0, 3, "vt ")) {
			iss >> trash >> trash;
			vec2 uv;
			float x, y;
			iss >> x >> y;
			m_texels.push_back(vec2(x, y));
		}
		else if (!line.compare(0, 2, "f ")) {
			int f, t, n;
			iss >> trash;
			int cnt = 0;
			while (iss >> f >> trash >> t >> trash >> n) {
				m_vertexIndices.push_back(--f);
				m_textureIndices.push_back(--t);
				m_normalsIndices.push_back(--n);
				cnt++;
			}

			if (3 != cnt) {
				std::cerr << "Error: the obj file is supposed to be triangulated" << std::endl;
				in.close();
				return;
			}

		}
	}
	std::cout << "indices " << m_vertexIndices.size() / 3 << std::endl;
	m_numFaces = m_vertexIndices.size() / 3;
	for (int i = 0; i < m_numFaces; i++) {
		vec3 v0 = vertice(i, 0);
		vec3 v1 = vertice(i, 1);
		vec3 v2 = vertice(i, 2);

		m_faceNormals.push_back(normalize(cross(v1 - v0, v2 - v0)));
	}

	std::cout << m_vertices.size() << "vertices " << m_texels.size() << "uvs " << m_normals.size() << "normals" << std::endl;

	std::string diffusefile = std::string(filepath) + "/" + std::string(name) + "_diffuse.tga";
	diffuseTexture.Load(diffusefile.c_str());

	std::string specularfile = std::string(filepath) + "/" + std::string(name) + "_spec.tga";
	specularTexture.Load(specularfile.c_str());

	std::string normtfile = std::string(filepath) + "/" + std::string(name) + "_nm_tangent.tga";
	nmTangentTexture.Load(normtfile.c_str());
}

Mesh::~Mesh()
{
}