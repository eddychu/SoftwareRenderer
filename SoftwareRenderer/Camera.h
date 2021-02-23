#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;

struct Camera {
	vec3 position;
	vec3 target;
	vec3 up;
	float fov = 90.f;
	float aspect = 4.0f / 3.0f;
	float near = 0.1f;
	float far = 100.f;
	Camera(vec3 p, vec3 t, vec3 u)
		: position(p)
		, target(t)
		, up(u)
	{
		
	}

	void OnMouseMove(int x, int y) {
		auto rotationMatrix = glm::rotate(glm::mat4(1.0), glm::radians(-x * 0.1f), up);
		position = vec3(rotationMatrix * vec4(position, 1.0));
		
	
		rotationMatrix = glm::rotate(glm::mat4(1.0), glm::radians(-y * 0.1f), GetRightVector());
		position = vec3(rotationMatrix * vec4(position, 1.0));
	}

	mat4 GetViewMatrix() {
		return lookAt(position, target, up);
	}

	mat4 GetProjectionMatrix() {
		return perspective(glm::radians(fov), aspect, near, far);
	}
	glm::vec3 GetRightVector()  
	{ return glm::transpose(GetViewMatrix())[0]; }

};