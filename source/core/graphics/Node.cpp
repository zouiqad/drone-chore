#include "Node.h"
#include <glm/gtc/matrix_transform.hpp>

namespace n2m::graphics {
Node::Node (std::shared_ptr<Model> model)
    : m_model (std::move (model)) {
}

void Node::draw (Shader& shader) const {
    if (!m_model) return;

    // Apply node's transformation matrix
    glm::mat4 modelMatrix = getTransformationMatrix ();

    // Set uniform
    shader.setUniform ("u_model", modelMatrix);

    // Set default light position (world space)
    shader.setUniform ("uLightPos", glm::vec3 (10.0f, 10.0f, 10.0f));

    // Set default light color (white light)
    shader.setUniform ("uLightColor", glm::vec3 (1.0f, 1.0f, 1.0f));

    // Set default ambient color (dim gray ambient light)
    shader.setUniform ("uAmbientColor", glm::vec3 (1.0f, 1.0f, 1.0f));

    m_model->draw (shader);
}

glm::mat4 Node::getTransformationMatrix () const {
    glm::mat4 mat = glm::mat4 (1.0f);
    mat           = glm::translate (mat, transform.translation);

    mat = glm::rotate (mat, glm::radians (transform.rotation.x),
        {1.0f, 0.0f, 0.0f});
    mat = glm::rotate (mat, glm::radians (transform.rotation.y),
        {0.0f, 1.0f, 0.0f});
    mat = glm::rotate (mat, glm::radians (transform.rotation.z),
        {0.0f, 0.0f, 1.0f});

    mat = glm::scale (mat, transform.scale);
    return mat;
}
}