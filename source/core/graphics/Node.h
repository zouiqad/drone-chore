#ifndef NODE_H
#define NODE_H

#include <memory>
#include <glm/gtc/matrix_transform.hpp>
#include "Model.h"

namespace n2m::graphics {
class Node {
public:
    struct Transform {
        Transform(const glm::vec3 &translation = glm::vec3(0.0f),
                  const glm::vec3 &rotation = glm::vec3(0.0f),
                  const glm::vec3 &scale = glm::vec3(3.0f)) : translation(
                                                                  translation),
                                                              rotation(rotation), scale(scale) {
        }

        glm::vec3 translation;
        glm::vec3 rotation;
        glm::vec3 scale;
    };

    explicit Node(std::shared_ptr<Model> model = nullptr);

    glm::mat4 getTransformationMatrix() const;

    std::shared_ptr<Model> getModel() const { return m_model; }

    void setModel(const std::shared_ptr<Model> &model) {
        this->m_model = model;
    }

    void draw(Shader &shader) const;

    // Transformations
    glm::vec3 getTranslation() const {
        return this->transform.translation;
    }

    glm::vec3 getRotation() const {
        return this->transform.rotation;
    }

    glm::vec3 getScale() const {
        return this->transform.scale;
    }

    void setTranslation(const glm::vec3 &translation) {
        transform.translation = translation;
    }

    void setRotation(const glm::vec3 &rotation) {
        transform.rotation = rotation;
    }

    void setScale(const glm::vec3 &scale) { transform.scale = scale; }

    void translate(const glm::vec3 &delta) { transform.translation += delta; }
    void rotate(const glm::vec3 &delta) { transform.rotation += delta; }
    void scale(const glm::vec3 &factor) { transform.scale *= factor; }

private:
    std::shared_ptr<Model> m_model;
    Transform transform;
};
}


#endif //NODE_H
