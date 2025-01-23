#ifndef FILEIO_H
#define FILEIO_H

#define TINYOBJLOADER_IMPLEMENTATION

#include <memory>
#include <string>
#include <cstdio>
#include "core/graphics/Drone.h"
#include "core/graphics/Geometry.h"
#include "core/graphics/Mesh.h"
#include "core/graphics/Scene.h"
#include "rapidjson/document.h" 
#include "rapidjson/filereadstream.h" 


namespace n2m::io {
class FileIO {
public:
    FileIO() = default;

    ~FileIO() = default;

    static std::string readFile(const std::string &filePath);

    static std::shared_ptr<graphics::Mesh> loadOBJ(
        const std::string &filepath);

    static bool exportOBJ(const std::string &filepath,
                          const std::vector<glm::vec3> &vertices,
                          const std::vector<unsigned int> &indices,
                          const std::vector<glm::vec3> &normals = {});

    static bool exportSTL(const std::string &filepath,
                          const std::vector<glm::vec3> &vertices,
                          const std::vector<unsigned int> &indices,
                          const std::vector<glm::vec3> &normals = {});
    static std::vector<graphics::Drone> readJson(std::string filename);


};
}

#endif
