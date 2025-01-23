#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <memory>
#include "Shader.h"
#include "Camera.h"
#include "Node.h"
#include "Drone.h"
#include "patterns/events/KeyboardEvent.h"

namespace n2m::graphics {
class Scene {
public:
    Scene() = default;

    Scene(Shader &shader);

    void draw(Shader &shader) const;

    // clear the scene from all the nodes
    void clear();

    // Add a new node to the scene
    void addNode(const std::shared_ptr<Node> &node);

    void removeNode(const std::shared_ptr<Node> &node);

    void setCamera(const Camera &camera) { this->mCamera = camera; }
    Camera &getCamera() { return mCamera; }

    // Get all nodes in the scene
    std::vector<std::shared_ptr<Node> > &getAllNodes() {
        return nodes;
    }

    static int t;
    static bool pause;

private:
    Camera mCamera;

    std::vector<std::shared_ptr<Node> > nodes;
};
} // namespace n2m::graphics

#endif // SCENE_H
