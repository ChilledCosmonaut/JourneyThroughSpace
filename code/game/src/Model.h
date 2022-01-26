#pragma once


#include "shader.h"
#include "Mesh.h"
#include "../FileManager/FileManager.h"

class Model {
public:
    vector<Texture> textures_loaded;
    explicit Model(char *path){
        loadModel(path);
    }
    void Draw(gl3::shader &shader);
private:
    // model data
    vector<Mesh> meshes;
    string directory;

    void loadModel(string path);
    void processNode(aiNode *node, const aiScene *scene);
    Mesh processMesh(aiMesh *mesh, const aiScene *scene);
    vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type,
                                         string typeName);
};
