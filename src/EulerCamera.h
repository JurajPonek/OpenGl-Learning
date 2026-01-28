#pragma once
#include "glm/glm.hpp"

enum class CameraDirection
{
	FORWARD,
	BACKWARD,
	RIGHT,
	LEFT
};

class EulerCamera
{
public:
	static constexpr float defaultCameraSpeed = 2.5f;
	static constexpr float maxPitchConstraint = 89.0f;
	static constexpr float minPitchConstraint = -89.0f;
	static constexpr float maxFov = 45.0f;
	static constexpr float minFov = 1.0f;
public:
	EulerCamera(glm::vec3 position, float fov, float acpectRatio, float nearPlane, float farPlane);

	void processKeyBoard(CameraDirection direction, float deltaTime);
	void processMouseMovement(double offsetX, double offsetY);
	void processMouseScroll(double offsetY);

	inline glm::mat4 getViewMatrix() const
	{
		return m_view;
	}
	inline float getFov() const { return m_fov; }
	inline float getAspectRatio() const { return m_acpectRatio; }
	inline float getNearPlane() const { return m_nearPlane; }
	inline float getFarPlane() const { return m_farPlane; }
private:
	void updateViewMatrix();

private:
	glm::vec3 m_position;
	float m_fov;
	float m_acpectRatio;
	float m_nearPlane;
	float m_farPlane;
	glm::mat4 m_view;
	glm::vec3 m_Up;
	glm::vec3 m_front;
	float m_pitch;
	float m_yaw;
};
