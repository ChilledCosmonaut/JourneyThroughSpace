#include "FileManager.h"

namespace files{

    std::string files::FileManager::getAssetFileFrom(const std::filesystem::path &relativeFilePath) {
        fs::path fullAssetPath = resolveForSubdirectory(relativeFilePath, "../../assets");
        return readText(fullAssetPath);
    }

    void files::FileManager::writeFileToTemp(const char *stringToSave, const std::filesystem::path &fileName) {
        fs::path tempFilePath = resolveForSubdirectory(fileName, std::filesystem::temp_directory_path());
        saveTextAt(stringToSave, tempFilePath);
    }

    void files::FileManager::saveFileAt(const char *stringToSave, const std::filesystem::path &relativeFilePath) {
        fs::path tempFilePath = resolveForSubdirectory(relativeFilePath, "../..");
        saveTextAt(stringToSave, tempFilePath);
    }

    std::string files::FileManager::readText(const std::filesystem::path &fileName) {
        std::ifstream sourceFile(fileName);
        std::stringstream buffer;
        buffer << sourceFile.rdbuf();
        sourceFile.close();
        return buffer.str();
    }

    void files::FileManager::saveTextAt(const char *stringToSave, const std::filesystem::path &fileName) {
        std::ofstream sourceFile(fileName);
        sourceFile << sourceFile.rdbuf();
        sourceFile.close();
    }

    const aiScene* FileManager::loadModelFromFile(const fs::path &relativeFilePath) {
        Assimp::Importer importer;
        const aiScene *modelScene = importer.ReadFile(relativeFilePath.string(), aiProcess_Triangulate | aiProcess_FlipUVs);

        if(!modelScene || modelScene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !modelScene->mRootNode) {
            throw std::runtime_error("ERROR::ASSIMP::" + std::string(importer.GetErrorString()));
        }
        return modelScene;
    }

    texture FileManager::loadTextureFromFile(const fs::path &relativeFilePath) {
        texture textureFile{};
        std::string stringFilePath = resolveForSubdirectory(relativeFilePath, "../../assets/textures").string();
        const char* filePath = stringFilePath.c_str();
        textureFile.content = stbi_load(filePath, &textureFile.width, &textureFile.height, &textureFile.colChannel, 0);
        return textureFile;
    }
}

