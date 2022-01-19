
#pragma once

#include <filesystem>
#include <fstream>
#include <sstream>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace files{
    namespace fs = std::filesystem;

    class FileManager {
    public:
        static std::string getAssetFileFrom(const fs::path &relativeFilePath);
        static void writeFileToTemp(const char* stringToSave, const fs::path &fileName);
        static void saveFileAt(const char* stringToSave, const fs::path &relativeFilePath);
        static const aiScene* loadModelFromFile(const fs::path &relativeFilePath);

    private:

       [[nodiscard]] fs::path static resolveForSubdirectory(const fs::path &relativeAssetPath, const fs::path &subdirectory) {
           return fs::weakly_canonical((subdirectory / relativeAssetPath).make_preferred());
       }
       static std::string readText(const std::filesystem::path &fileName);
       static void saveTextAt(const char* stringToSave, const std::filesystem::path &fileName);
    };
}
