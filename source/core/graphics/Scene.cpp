#include "Scene.h"

#include "patterns/singleton/EventDispatcher.h"

#include <iostream>
#include <ostream>

namespace n2m::graphics {
// ---------------- Scene Implementation -----------------
Scene::Scene (Shader& shader) {
    // Initialize camera
    mCamera = Camera ();

    mCamera.setPerspective (45.0f, 16.0f / 9.0f, 0.1f, 100.0f);
    mCamera.setPosition (glm::vec3 (0.0f, 0.0f, 2.0f));
    mCamera.lookAt (glm::vec3 (0.0f, 0.0f, 0.0f));
}

void Scene::clear () {
    nodes.clear ();
}

void Scene::addNode (const std::shared_ptr<Node>& node) {
    nodes.push_back (node);
}

void Scene::removeNode (const std::shared_ptr<Node>& node) {
    std::erase (nodes, node);
}

void Scene::draw (Shader& shader) const {
    // Loop through nodes and render associated geometry
    for (const auto& node : nodes) {
        node->draw (shader);
    }
}
}