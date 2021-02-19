#pragma once
#include <glm/glm.hpp>
using namespace glm;
#include <vector>
class Texture {
public:
	Texture();
	~Texture();
	void Load(const char* filename);

	vec4 GetColor(float u, float v);
	vec4 GetColor(const vec2& uv);
	std::vector<float> data;
	int width;
	int height;
};