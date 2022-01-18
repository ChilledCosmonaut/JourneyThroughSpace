
#pragma once

#include <filesystem>
#include <fstream>
#include <sstream>

namespace files{
    namespace fs = std::filesystem;

    class FileManager {
    public:
        static const char* getAssetFileFrom(const fs::path &relativeFilePath);
        static void writeFileToTemp(const char* stringToSave, const fs::path &fileName);
        static void saveFileAt(const char* stringToSave, const fs::path &relativeFilePath);

    private:

       [[nodiscard]] fs::path static resolveForSubdirectory(const fs::path &relativeAssetPath, const fs::path &subdirectory) {
           return fs::weakly_canonical((subdirectory / relativeAssetPath).make_preferred());
       }
       static std::string readText(const std::filesystem::path &fileName);
       static void saveTextAt(const char* stringToSave, const std::filesystem::path &fileName);
    };
}
