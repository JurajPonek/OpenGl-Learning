#pragma once
#include "glm/glm.hpp"

enum class CameraDirection
{
	FORWARD,
	BACKWARD,
	RIGHT,
	LEFT
};

class Camera
{
public:
	static constexpr float defaultCameraSpeed = 2.5f;
	static constexpr float maxFov = 45.0f;
	static constexpr float minFov = 1.0f;
	static constexpr float defaultSensitivity = 0.05f;
public:
	Camera(glm::vec3 position, float fov, float acpectRatio, float nearPlane, float farPlane);
	virtual ~Camera() = default;

	virtual void processKeyBoard(CameraDirection direction, float deltaTime) = 0;
	virtual void processMouseMovement(double offsetX, double offsetY) = 0;
	virtual void processMouseScroll(double offsetY) = 0;

	inline glm::mat4 getViewMatrix() const
	{
		return m_view;
	}
	inline float getFov() const { return m_fov; }
	inline float getAspectRatio() const { return m_acpectRatio; }
	inline float getNearPlane() const { return m_nearPlane; }
	inline float getFarPlane() const { return m_farPlane; }
	inline glm::vec3 getPosition() const { return m_position; }
protected:
	virtual void updateViewMatrix() = 0;

protected:
	glm::vec3 m_position;
	float m_fov;
	float m_acpectRatio;
	float m_nearPlane;
	float m_farPlane;
	glm::mat4 m_view;
};
