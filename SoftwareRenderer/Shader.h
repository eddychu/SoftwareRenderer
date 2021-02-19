#pragma once

#include <glm/glm.hpp>
using namespace glm;
#include <algorithm>
#include <iostream>
#include "Texture.h"


struct Shader 
{
	mat4 MVP;
	mat4 MV;
	mat3 N;
	Texture* diffuseTexture;
	Texture* specTexture;
	float varIntensity = 0;
	vec3 lightPos;
	vec2 varyingUVs[3];
	vec3 varyingPos[3];
	vec3 varyingNormal[3];

	vec4 Vertex(const vec3& vertex, const vec3& normal, const vec2& uv, int index)
	{
		varyingUVs[index] = uv;
		varyingPos[index] = vec3(MV * vec4(vertex, 1.0));
		varyingNormal[index] = normalize(N * normal);
		return MVP * vec4(vertex, 1.0);
	}

	vec4 Fragment(float a, float b, float c)
	{
		vec2 interUV = a * varyingUVs[0] + b * varyingUVs[1] + c * varyingUVs[2];
		vec3 interPos = a * varyingPos[0] + b * varyingPos[1] + c * varyingPos[2];
		vec3 interNorm = a * varyingNormal[0] + b * varyingNormal[1] + c * varyingNormal[2];		
		vec3 s = normalize(lightPos - interPos);
		float sDotN = max(dot(s, interNorm), 0.0f);
		vec4 diffuse = diffuseTexture->GetColor(interUV) * sDotN;
		vec4 spec = specTexture->GetColor(interUV);
		if (sDotN > 0.0f) {
			vec3 v = normalize(-interPos);
			vec3 r = glm::reflect(-s, interNorm);
			float temp = std::pow(max(dot(r, v), 0.0f), 64);
			spec = spec * temp;
		}
		return diffuse + spec;
	}
};