
#pragma once

#include <filesystem>
#include <fstream>
#include <sstream>

namespace files{
    namespace fs = std::filesystem;

    class FileManager {
    public:
        const char* getAssetFileFrom(const fs::path &relativeFilePath);
        void writeFileToTemp(const char* stringToSave, const fs::path &fileName);
        void saveFileAt(const char* stringToSave, const fs::path &relativeFilePath);

    private:

       [[nodiscard]] fs::path static resolveForSubdirectory(const fs::path &relativeAssetPath, const fs::path &subdirectory) {
           return fs::canonical((subdirectory / relativeAssetPath).make_preferred());
       }
       std::string readText(const std::filesystem::path &fileName);
       void saveTextAt(const char* stringToSave, const std::filesystem::path &fileName);
    };
}
