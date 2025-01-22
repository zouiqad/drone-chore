#include "Engine.h"


#include "patterns/singleton/EventDispatcher.h"
#include "io/FileIO.h"
#include "io/Window.h"
#include <iostream>
#include <GLFW/glfw3.h>

namespace n2m {
bool Engine::init () {
    // Init window
    if (!m_window.init (1280, 720, "Nuage2Mesh Viewer")) {
        std::cerr << "Failed to initialize window.\n";
        return false;
    }

    if (!m_renderer.init ()) {
        std::cerr << "Failed to initialize renderer.\n";
        return false;
    }

    // Subscribe to events

    EventDispatcher::Instance ().subscribe<MouseDragEvent> (
        [this](const MouseDragEvent& e) {
            handleMouseDragEvent (e);
        });

    EventDispatcher::Instance ().subscribe<MouseScrollEvent> (
        [this](const MouseScrollEvent& e) {
            handleMouseScrollEvent (e);
        });

    EventDispatcher::Instance ().subscribe<RenderModeEvent> (
        [this](const RenderModeEvent& e) {
            handleRenderModeEvent (e);
        });

    EventDispatcher::Instance ().subscribe<KeyboardEvent> (
        [this](const KeyboardEvent& e) {
            handleKeyboardEvent (e);
        });

    return true;
}

void Engine::run () {
    while (!m_window.shouldClose ()) {
        m_window.render ();

        if (!m_window.pollEvents ()) {
            continue;
        }

        m_renderer.drawFrame ();
    }

    shutdown ();
}

void Engine::shutdown () {
    m_window.shutdown ();
    m_renderer.cleanup ();
}

//@todo fix
void Engine::handleMouseDragEvent (const MouseDragEvent& e) {
    const float SENSITIVITY = 0.5f;

    m_renderer.getScene ().getCamera ().yaw += e.xoffset * SENSITIVITY;
    m_renderer.getScene ().getCamera ().pitch += e.yoffset * SENSITIVITY;

    if (m_renderer.getScene ().getCamera ().pitch > 89.0f)
        m_renderer.
            getScene ().getCamera ().pitch = 89.0f;
    if (m_renderer.getScene ().getCamera ().pitch < -89.0f)
        m_renderer.
            getScene ().getCamera ().pitch = -89.0f;

    glm::vec3 direction;
    direction.x = cos (glm::radians (m_renderer.getScene ().getCamera ().yaw)) *
        cos (glm::radians (m_renderer.getScene ().getCamera ().pitch));
    direction.y =
        sin (glm::radians (m_renderer.getScene ().getCamera ().pitch));
    direction.z = sin (glm::radians (m_renderer.getScene ().getCamera ().yaw)) *
        cos (glm::radians (m_renderer.getScene ().getCamera ().pitch));

    m_renderer.getScene ().getCamera ().setDirection (direction);
}

//@todo fix
void Engine::handleMouseScrollEvent (const MouseScrollEvent& e) {
    zoomFactor -= static_cast<float> (e.yoffset) * 0.5f;
    zoomFactor = glm::clamp (zoomFactor, 1.0f, 20.0f);
}


void Engine::handleRenderModeEvent (const RenderModeEvent& e) {
    switch (e.render_mode) {
    case RenderModeEvent::renderMode::Wireframe:
        glPolygonMode (GL_FRONT_AND_BACK, GL_LINE);
        break;
    case RenderModeEvent::renderMode::Solid:
        glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);
        break;
    default: break;
    }
}

void Engine::handleKeyboardEvent (const KeyboardEvent& e) {
    float speed = 0.04f;

    if (e.getAction () == KeyAction::PRESS) {
        switch (e.getKeyCode ()) {
        case GLFW_KEY_W: m_renderer.getScene ().getCamera ().moveForward (
                speed);
            break;
        case GLFW_KEY_S: m_renderer.getScene ().getCamera ().moveBackward (
                speed);
            break;
        case GLFW_KEY_A: m_renderer.getScene ().getCamera ().moveLeft (
                speed);
            break;
        case GLFW_KEY_D: m_renderer.getScene ().getCamera ().moveRight (
                speed);
            break;
        default: break;
        }
    }
}
}