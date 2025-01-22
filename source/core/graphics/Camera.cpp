#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>

namespace n2m::graphics {
Camera::Camera ()
    : position (0.0f, 0.0f, 0.0f)
      , forward (0.0f, 0.0f, 1.0f)
      , right (1.0f, 0.0f, 0.0f)
      , up (0.0f, 1.0f, 0.0f)
      , baseFov (45.0f)
      , fov (45.0f)
      , aspectRatio (16.0f / 9.0f)
      , nearZ (0.1f)
      , farZ (100.0f) {
}

void Camera::setPerspective (float fovDegrees,
    float aspect,
    float nearPlane,
    float farPlane) {
    fov         = fovDegrees;
    aspectRatio = aspect;
    nearZ       = nearPlane;
    farZ        = farPlane;
}

void Camera::setPosition (const glm::vec3& pos) {
    position = pos;
}

void Camera::moveForward (float dist) {
    position += forward * dist;
}

void Camera::moveBackward (float dist) {
    position -= forward * dist;
}

void Camera::moveRight (float dist) {
    position += right * dist;
}

void Camera::moveLeft (float dist) {
    position -= right * dist;
}

void Camera::lookAt (const glm::vec3& target) {
    setDirection (normalize (target - position));
}

void Camera::setUp (const glm::vec3& upVec) {
    up = upVec;
}

void Camera::setDirection (const glm::vec3& dir) {
    forward = glm::normalize (dir);
    right = glm::normalize (glm::cross (forward, glm::vec3 (0.0f, 1.0f, 0.0f)));
    up = glm::normalize (glm::cross (right, forward));
}


void Camera::zoom (const float& zoomFactor) {
    // Zoom in or out
    fov += zoomFactor * zoomSensitivity;
    fov = glm::clamp (fov, fovMin, fovMax);
}

glm::mat4 Camera::getViewMatrix () const {
    // If using forward/up, we can compute a right vector
    // or just use glm::lookAt with position and position+forward
    return glm::lookAt (position, position + forward, up);
}

glm::mat4 Camera::getProjectionMatrix () const {
    // Convert degrees to radians for glm::perspective
    float fovRadians = glm::radians (fov);
    return glm::perspective (fovRadians, aspectRatio, nearZ, farZ);
}
}
