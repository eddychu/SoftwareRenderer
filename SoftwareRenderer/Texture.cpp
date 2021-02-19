#include "Texture.h"
#include <cmath>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
Texture::Texture()
	:width(0)
	,height(0)
{
}

Texture::~Texture() {
	
}

void Texture::Load(const char* file)
{
	stbi_set_flip_vertically_on_load(true);
	int channels;
	unsigned char* rawData = stbi_load(file, &width, &height, &channels, 4);

	data.resize(width * height * 4);

	for (int i = 0; i < width * height * 4; i++) {
		data[i] = std::pow((float)rawData[i] * (1 / 255.0f), 2.0f);
	}
	stbi_image_free(rawData);
}

vec4 Texture::GetColor(float u, float v)
{
	int x = u * width;
	int y = v * height;
	int index = (y * width + x) * 4;
	return vec4(data[index], data[index + 1], data[index + 2], data[index + 3]);
}

vec4 Texture::GetColor(const vec2& uv)
{
	return GetColor(uv.x, uv.y);
}
