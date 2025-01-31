#include "Renderer.h"
#include <ctime>
#include <chrono>
#include <thread>

#include "core/io/FileIO.h"


namespace n2m::graphics {
Renderer::Renderer () {
}

Renderer::~Renderer () {
    cleanup ();
}

bool Renderer::initGizmos () {
    if (!mGizmosShader.loadShaders ("resources/shaders/gizmos.vert",
        "resources/shaders/gizmos.frag")) {
        return false;
    }

    Gizmos::Get ().init ();
    return true;
}

bool Renderer::initSkybox () {
    if (!mSkyboxShader.loadShaders ("resources/shaders/skybox.vert",
        "resources/shaders/skybox.frag")) {
        return false;
    }

    std::string rootPath = "resources/textures/skybox/";
    std::vector texPaths = {
        rootPath + "right.jpg",
        rootPath + "left.jpg",
        rootPath + "top.jpg",
        rootPath + "bottom.jpg",
        rootPath + "front.jpg",
        rootPath + "back.jpg"
    };

    mSkybox = std::make_unique<Skybox> (texPaths);

    return true;
}

bool Renderer::init () {
    // Load a basic vertex/fragment shader program
    if (!mShader.loadShaders ("resources/shaders/simple.vert",
        "resources/shaders/simple.frag")) {
        return false;
    }

    initSkybox ();

    initGizmos ();

    mScene = Scene (mShader);

    auto rootNode  = std::make_shared<Node> ();
    auto model_ptr = std::make_shared<Model> (
        "resources/models/drone/scene.gltf");

    //rootNode->setModel (model_ptr);


    std::vector<Drone> drones = n2m::io::FileIO::readJson (
        "resources/waypoints.json");

    for (size_t i = 0; i < drones.size (); i++) {
        drones[i].setModel (model_ptr);
        auto drone = std::make_shared<Drone> (drones[i]);
        drone->setScale (glm::vec3 (0.1f));
        mScene.addNode (drone);
    }


    // add ground
    auto ground_model = std::make_shared<Model> (
        "resources/models/ground/ground.obj");

    auto ground = std::make_shared<Node> (ground_model);
    ground->setScale (glm::vec3 (10.0f));
    mScene.addNode (ground);

    return true;
}

void Renderer::drawFrame () {
    glClearColor (0.1f, 0.1f, 0.2f, 1.0f);
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (mSkybox) {
        mSkybox->draw (mSkyboxShader, mScene.getCamera ().getViewMatrix (),
            mScene.getCamera ().getProjectionMatrix ());
    }

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

    Gizmos::Get ().render (mGizmosShader, mScene.getCamera ().getViewMatrix (),
        mScene.getCamera ().getProjectionMatrix ());
}

void Renderer::cleanup () {
    mScene.clear ();
}
}
