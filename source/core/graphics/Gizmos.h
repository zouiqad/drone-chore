#ifndef GIZMOS_H
#define GIZMOS_H


#include "Shader.h"

#include <vector>
#include<glm/glm.hpp>

namespace n2m::graphics {
class Gizmos {
public:
    static Gizmos& Get () {
        static Gizmos instance;
        return instance;
    }

    bool init ();

    void render (Shader& gizmoShader,
        const glm::mat4& view,
        const glm::mat4& proj);

    void drawLine (const glm::vec3& origin,
        const glm::vec3& direction,
        float length           = 10.0f,
        const glm::vec3& color = glm::vec3 (0.0f, 1.0f, 0.0f));

    void drawLine (const glm::vec3& start,
        const glm::vec3& end,
        const glm::vec3& color = glm::vec3 (0.0f, 1.0f, 0.0f));

private:
    GLuint vaoStatic, vaoDynamic, vboStatic, vboDynamic;

    struct Vertex {
        glm::vec3 position;
        glm::vec3 color;
    };

    std::vector<Vertex> mDynamicVertices;
    std::vector<Vertex> mStaticVertices;

    void drawLineStatic (const glm::vec3& origin,
        const glm::vec3& direction,
        float length           = 10.0f,
        const glm::vec3& color = glm::vec3 (0.0f, 1.0f, 0.0f));

    void drawLineStatic (const glm::vec3& start,
        const glm::vec3& end,
        const glm::vec3& color = glm::vec3 (0.0f, 1.0f, 0.0f));

    void drawAxis ();

    void drawGrid (const int& gridSize = 100,
        const float& gridSpacing       = 10.0f,
        const glm::vec3& color         = glm::vec3 (1.0f));
};
}

#endif //GIZMO_H
