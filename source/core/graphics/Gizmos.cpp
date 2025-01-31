
#include "Gizmos.h"

namespace n2m::graphics {
bool Gizmos::init () {
    glGenVertexArrays (1, &vaoStatic);
    glGenVertexArrays (1, &vaoDynamic);
    glGenBuffers (1, &vboDynamic);
    glGenBuffers (1, &vboStatic);


    glBindVertexArray (vaoDynamic);
    glBindBuffer (GL_ARRAY_BUFFER, vboDynamic);
    glBufferData (GL_ARRAY_BUFFER, mDynamicVertices.size () * sizeof (Vertex),
        mDynamicVertices.data (), GL_DYNAMIC_DRAW);

    // position
    glEnableVertexAttribArray (0);
    glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE,
        sizeof (Vertex), (void*)0);

    // color
    glEnableVertexAttribArray (1);
    glVertexAttribPointer (1, 3, GL_FLOAT, GL_FALSE,
        sizeof (Vertex),
        (void*)offsetof (Vertex, color));

    glBindVertexArray (0);

    // populate static vertices
    drawAxis ();
    drawGrid ();

    glBindVertexArray (vaoStatic);
    glBindBuffer (GL_ARRAY_BUFFER, vboStatic);
    glBufferData (GL_ARRAY_BUFFER, mStaticVertices.size () * sizeof (Vertex),
        mStaticVertices.data (), GL_STATIC_DRAW);

    // position
    glEnableVertexAttribArray (0);
    glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE,
        sizeof (Vertex), (void*)0);

    // color
    glEnableVertexAttribArray (1);
    glVertexAttribPointer (1, 3, GL_FLOAT, GL_FALSE,
        sizeof (Vertex),
        (void*)offsetof (Vertex, color));

    glBindVertexArray (0);

    return true;
}

void Gizmos::drawGrid (const int& gridSize,
    const float& spacing,
    const glm::vec3& color) {
    const float halfSize = gridSize / 2.0f;

    // Draw lines along X-axis
    for (int y = 0; y <= gridSize; y++) {
        for (int z = 0; z <= gridSize; z++) {
            float yPos = -halfSize + y * spacing;
            float zPos = -halfSize + z * spacing;

            drawLineStatic (glm::vec3 (-halfSize, yPos, zPos),
                glm::vec3 (1, 0, 0),
                gridSize,
                color);
        }
    }

    // Draw lines along Y-axis
    for (int x = 0; x <= gridSize; x++) {
        for (int z = 0; z <= gridSize; z++) {
            float xPos = -halfSize + x * spacing;
            float zPos = -halfSize + z * spacing;

            drawLineStatic (glm::vec3 (xPos, -halfSize, zPos),
                glm::vec3 (0, 1, 0),
                gridSize,
                color);
        }
    }

    // Draw lines along Z-axis
    for (int x = 0; x <= gridSize; x++) {
        for (int y = 0; y <= gridSize; y++) {
            float xPos = -halfSize + x * spacing;
            float yPos = -halfSize + y * spacing;

            drawLineStatic (glm::vec3 (xPos, yPos, -halfSize),
                glm::vec3 (0, 0, 1),
                gridSize,
                color);
        }
    }
}

void Gizmos::drawAxis () {
    auto xaxis = glm::vec3 (1.0f, 0.0f, 0.0f);
    auto yaxis = glm::vec3 (0.0f, 1.0f, 0.0f);
    auto zaxis = glm::vec3 (0.0f, 0.0f, 1.0f);

    drawLineStatic (xaxis * -100.0f, xaxis * 100.0f, xaxis);
    drawLineStatic (yaxis * -100.0f, yaxis * 100.0f, yaxis);
    drawLineStatic (zaxis * -100.0f, zaxis * 100.0f, zaxis);
}

void Gizmos::drawLineStatic (const glm::vec3& origin,
    const glm::vec3& direction,
    float length,
    const glm::vec3& color) {
    mStaticVertices.push_back ({origin, color});
    mStaticVertices.push_back ({origin + direction * length, color});
}

void Gizmos::drawLineStatic (const glm::vec3& start,
    const glm::vec3& end,
    const glm::vec3& color) {
    mStaticVertices.push_back ({start, color});
    mStaticVertices.push_back ({end, color});
}

void Gizmos::drawLine (const glm::vec3& origin,
    const glm::vec3& direction,
    float length,
    const glm::vec3& color) {
    mDynamicVertices.push_back ({origin, color});
    mDynamicVertices.push_back ({origin + direction * length, color});
}

void Gizmos::drawLine (const glm::vec3& start,
    const glm::vec3& end,
    const glm::vec3& color) {
    mDynamicVertices.push_back ({start, color});
    mDynamicVertices.push_back ({end, color});
}

void Gizmos::render (Shader& gizmoShader,
    const glm::mat4& view,
    const glm::mat4& proj) {
    gizmoShader.use ();
    gizmoShader.setUniform ("u_view", view);
    gizmoShader.setUniform ("u_proj", proj);

    glBindVertexArray (vaoDynamic);

    glBindBuffer (GL_ARRAY_BUFFER, vboDynamic);
    glBufferData (GL_ARRAY_BUFFER,
        mDynamicVertices.size () * sizeof (Vertex),
        mDynamicVertices.data (),
        GL_DYNAMIC_DRAW);
    glDrawArrays (GL_LINES, 0, mDynamicVertices.size ());

    glBindVertexArray (vaoStatic);

    glBindBuffer (GL_ARRAY_BUFFER, vboStatic);
    glBufferData (GL_ARRAY_BUFFER,
        mStaticVertices.size () * sizeof (Vertex),
        mStaticVertices.data (),
        GL_STATIC_DRAW);

    glDrawArrays (GL_LINES, 0, mStaticVertices.size ());

    glBindVertexArray (0);
    mDynamicVertices.clear ();
}
}