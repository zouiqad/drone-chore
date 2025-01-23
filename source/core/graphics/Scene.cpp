#include "Scene.h"

#include "patterns/singleton/EventDispatcher.h"

#include <iostream>
#include <ostream>


#define FRAMETIME 33



namespace n2m::graphics {
    int Scene::t = 0;
    bool Scene::pause = false;
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

void Scene::draw (Shader& shader) const {
    static clock_t temps = clock();

    if(t < 480 && !pause && (clock() - temps) * 50000 / CLOCKS_PER_SEC >= FRAMETIME)
    {
        //std::cout << "t = " << t << std::endl;
        t++;
        temps = clock();
    }
    // Loop through nodes and render associated geometry
    for (const auto& node : nodes) {
        if(node->whoAmI() == "Drone")
        {
            ((Drone*)node.get())->updatePosition(t);
        }
        node->draw (shader);
    }
    
}
}