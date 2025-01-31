#ifndef MODEL_H
#define MODEL_H

#include "Mesh.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace n2m::graphics {
unsigned int loadTexture(const char *path,
                         const std::string &directory,
                         bool gamma = false);

class Model {
public:
    Model(const char *path) {
        loadModel(path);
    }

    virtual void draw(Shader &shader);

private:
    // model data
    std::vector<Mesh> meshes;
    std::string directory;

    std::vector<Texture> textures_loaded;

    void loadModel(std::string path);

    void processNode(aiNode *node, const aiScene *scene);

    Mesh processMesh(aiMesh *mesh, const aiScene *scene);

    std::vector<Texture> loadMaterialTextures(aiMaterial *mat,
                                              aiTextureType type,
                                              std::string typeName);
};
}

#endif //MODEL_H
