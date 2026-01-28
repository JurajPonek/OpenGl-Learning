#pragma once
#include "Camera.h"
#include "glm/glm.hpp"
#include <glm/gtc/quaternion.hpp>

class QuaternionCamera : public Camera
{
public:
	QuaternionCamera(glm::vec3 position, glm::vec3 orientation, float fov, float acpectRatio, float nearPlane, float farPlane);

	void processKeyBoard(CameraDirection direction, float deltaTime) override;
	void processMouseMovement(double offsetX, double offsetY) override;
	void processMouseScroll(double offsetY) override;

private:
	void updateViewMatrix() override;

private:
	inline glm::vec3 getFront() const
	{
		return m_orientation * glm::vec3(.0f, .0f, -1.0f);
	}
	inline glm::vec3 getUp() const
	{
		return m_orientation * glm::vec3(.0f, 1.0f, .0f);
	}
	inline glm::vec3 getRight() const
	{
		return m_orientation * glm::vec3(1.0f, .0f, .0f);
	}

private:
	glm::quat m_orientation;
};
