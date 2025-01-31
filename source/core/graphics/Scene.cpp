#include "Scene.h"

#include "Gizmos.h"
#include "patterns/singleton/EventDispatcher.h"

#include <iostream>
#include <ostream>
#include <GLFW/glfw3.h>


#define FRAMETIME 33


namespace n2m::graphics {
int Scene::t                = 0;
int Scene::collision_radius = 50;
int Scene::speed_limit      = 50;
bool Scene::pause           = false;
// ---------------- Scene Implementation -----------------
Scene::Scene (Shader& shader) {
    // Initialize camera
    mCamera = Camera ();

    mCamera.setPerspective (45.0f, 16.0f / 9.0f, 0.1f, 100.0f);
    mCamera.setPosition (glm::vec3 (0.0f, 10.0f, 10.0f));
    mCamera.lookAt (glm::vec3 (0.0f, 5.0f, 0.0f));
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

void Scene::draw (Shader& shader) {
    float currentTime = static_cast<float> (glfwGetTime ());
    float deltaTime   = currentTime - lastTime;

    if (t < 480 && !pause && deltaTime * 1000 >= FRAMETIME) {
        t++;
        lastTime = currentTime;
    }

    // Loop through nodes and render associated geometry
    int cptDOG = 0;
    for (const auto& node : nodes) {
        if (node->whoAmI () == "Drone") {
            auto oldPos = ((Drone*)node.get ())->getTranslation ();
            ((Drone*)node.get ())->updatePosition (t);
            auto newPos = ((Drone*)node.get ())->getTranslation ();
            if ((oldPos - newPos).length () > speed_limit / 10.0f)
                std::cout <<
                    "A la frame " << t << ", le drone " << cptDOG <<
                    " va trop vite ! " << (oldPos - newPos).length () <<
                    std::endl;
            cptDOG++;

            // debug draw
            Gizmos::Get ().drawLine (newPos, glm::vec3 (0.0f, -1.0f, 0.0f),
                10.0f);
        }
        node->draw (shader);
    }

    for (size_t i = 0; i < nodes.size () - 1; i++) {
        if (nodes[i]->whoAmI () != "Drone") {
            continue;
        }
        for (size_t j = i + 1; j < nodes.size (); j++) {
            if (nodes[j]->whoAmI () == "Drone") {
                if (Drone::distance (*((Drone*)nodes[j].get ()),
                    *((Drone*)nodes[i].get ())) < collision_radius * 100.0f) {
                    // std::cout << "Collision entre le drone " << i <<
                    //     " et le drone " << j << " à la frame " << t <<
                    //     std::endl;
                }
            }
        }
    }
}
}