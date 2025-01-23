#ifndef RENDERER_H
#define RENDERER_H

#include "Camera.h"
#include "Shader.h"
#include "Scene.h"

#include "../io/FileIO.h"

#include <vector>

namespace n2m::graphics {
class Renderer {
public:
    Renderer();

    ~Renderer();

    bool init();

    // Loads shaders, initializes camera, maybe sets up default meshes
    void drawFrame();

    // Called every frame to render the scene
    void cleanup(); // Frees GPU resources if needed

    // Getters & setters
    void setScene(const Scene &scene) { this->mScene = scene; }
    Scene &getScene() { return mScene; }

    void setShader(const Shader &shader) { this->mShader = shader; }
    Shader &getShader() { return mShader; }

private:
    Shader mShader;
    Scene mScene;
};
}
#endif // RENDERER_H
