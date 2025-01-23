#include "Renderer.h"
#include <ctime>
#include <chrono>
#include <thread>


namespace n2m::graphics {
Renderer::Renderer () {
}

Renderer::~Renderer () {
    cleanup ();
}

bool Renderer::init () {
    // Load a basic vertex/fragment shader program
    if (!mShader.loadShaders ("resources/shaders/simple.vert",
        "resources/shaders/simple.frag")) {
        return false;
    }

    mScene = Scene (mShader);

    auto rootNode  = std::make_shared<Node> ();
    auto model_ptr = std::make_shared<Model> (
        "resources/models/backpack/backpack.obj");

    rootNode->setModel (model_ptr);

    //mScene.addNode (rootNode);

    std::vector<Drone> drones = io::FileIO::readJson("resources/waypoints.json");

    std::cout << "x pos first drone = " << drones[0].getTranslation().x << std::endl;
    std::cout << "y pos first drone = " << drones[0].getTranslation().y << std::endl;
    std::cout << "z pos first drone = " << drones[0].getTranslation().z << std::endl;

    for (size_t i = 0; i < drones.size(); i++)
    {
        drones[i].setModel(model_ptr);
        mScene.addNode(std::make_shared<Drone>(drones[i]));
    }
    

    return true;
}

void Renderer::drawFrame () {
    

    glClearColor (0.1f, 0.1f, 0.2f, 1.0f);
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    mShader.use ();

    // Make the camera look at the origin
    glm::mat4 view       = mScene.getCamera ().getViewMatrix ();
    glm::mat4 projection = mScene.getCamera ().getProjectionMatrix ();

    // Set uniform
    mShader.setUniform ("u_view", view);
    mShader.setUniform ("u_proj", projection);

    mShader.setUniform ("uViewPos", mScene.getCamera ().getPosition ());

    // Draw the scene
    mScene.draw (mShader);
}

void Renderer::cleanup () {
    mScene.clear ();
}
}
