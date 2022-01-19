
#include <iostream>
#include "FileManager.h"

namespace files{

    std::string files::FileManager::getAssetFileFrom(const std::filesystem::path &relativeFilePath) {
        fs::path fullAssetPath = resolveForSubdirectory(relativeFilePath, "../../assets");
        return readText(fullAssetPath);
    }

    void files::FileManager::writeFileToTemp(const char *stringToSave, const std::filesystem::path &fileName) {
        std::cout << "Before resolve" << std::endl;
        fs::path tempFilePath = resolveForSubdirectory(fileName, std::filesystem::temp_directory_path());
        std::cout << "After resolve" << std::endl;
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
}

