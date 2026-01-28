#include "Camera.h"

Camera::Camera(glm::vec3 position, float fov, float acpectRatio, float nearPlane, float farPlane)
	:m_position(position), m_fov(fov), m_acpectRatio(acpectRatio), m_nearPlane(nearPlane), m_farPlane(farPlane)
{
}