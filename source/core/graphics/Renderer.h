#ifndef RENDERER_H
#define RENDERER_H

#include "Camera.h"
#include "Skybox.h"
#include "Shader.h"
#include "Scene.h"
#include "Gizmos.h"


namespace n2m::graphics {
class Renderer {
public:
    Renderer();

    ~Renderer();

    bool init();

    bool initSkybox();

    bool initGizmos();

    void drawFrame();

    void cleanup();

    // Getters & setters
    void setScene(const Scene &scene) { this->mScene = scene; }
    Scene &getScene() { return mScene; }

    void setShader(const Shader &shader) { this->mShader = shader; }
    Shader &getShader() { return mShader; }

private:
    Scene mScene;

    Shader mShader;
    Shader mSkyboxShader;
    Shader mGizmosShader;

    std::unique_ptr<Skybox> mSkybox;
};
}
#endif // RENDERER_H
