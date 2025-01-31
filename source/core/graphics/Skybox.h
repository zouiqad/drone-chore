#ifndef __SKYBOX_H__
#define __SKYBOX_H__


#include "Shader.h"
#include <vector>
#include <string>


namespace n2m::graphics {
class Skybox {
public:
    Skybox (const std::vector<std::string>& texPaths);

    ~Skybox ();

    void draw (Shader& skyboxShader,
        const glm::mat4& view,
        const glm::mat4& projection);

private:
    GLuint cubemapTexture;
    GLuint vao, vbo;

    void loadCubemap (const std::vector<std::string>& faces);

    void setup ();
};
}

#endif