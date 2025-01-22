#ifndef CAMERA_H
#define CAMERA_H
#include <iostream>
#include <ostream>
#include <glm/glm.hpp>


namespace n2m::graphics {
class Camera {
public:
    Camera();

    ~Camera() = default;

    float yaw = -90.0f;
    float pitch = 0.0f;

    // Set perspective parameters
    void setPerspective(float fovDegrees,
                        float aspect,
                        float nearPlane,
                        float farPlane);

    // void setOrthographic(float left, float right, float bottom, float top, float nearPlane, float farPlane);

    // Basic camera transforms
    glm::vec3 getPosition() const { return position; }

    void setPosition(const glm::vec3 &pos);

    void lookAt(const glm::vec3 &target);

    void setUp(const glm::vec3 &upVec);

    void setDirection(const glm::vec3 &dir);

    void zoom(const float &zoomFactor);

    glm::mat4 getViewMatrix() const;

    glm::mat4 getProjectionMatrix() const;


    void move(const glm::vec3 &dir) { setPosition(getPosition() + dir); }

    void moveForward(float distance);

    void moveBackward(float distance);

    void moveRight(float distance);

    void moveLeft(float distance);

private:
    glm::vec3 position;
    glm::vec3 forward;
    glm::vec3 right;
    glm::vec3 up;

    // For perspective
    float fov;
    float aspectRatio;
    float nearZ;
    float farZ;

    // Zoom
    float baseFov;
    float zoomSensitivity = 1.0f;
    float fovMin = 30.0f;
    float fovMax = 90.0f;
};
} // namespace MyApp::Graphics

#endif // CAMERA_H
