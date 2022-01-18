
#include "FileManager.h"

const char* files::FileManager::getAssetFileFrom(const std::filesystem::path &relativeFilePath) {
    fs::path fullAssetPath = resolveForSubdirectory(relativeFilePath, "../../assets");
    auto fileContent = readText(fullAssetPath);
    return fileContent.c_str();
}

void files::FileManager::writeFileToTemp(const char *stringToSave, const std::filesystem::path &fileName) {
    fs::path tempFilePath = resolveForSubdirectory(fileName, std::filesystem::temp_directory_path());
    tempFilePath = fs::canonical(tempFilePath);
    saveTextAt(stringToSave, tempFilePath);
}

void files::FileManager::saveFileAt(const char *stringToSave, const std::filesystem::path &relativeFilePath) {

}

std::string files::FileManager::readText(const std::filesystem::path &fileName) {
    std::ifstream sourceFile(fileName);
    std::stringstream buffer;
    buffer << sourceFile.rdbuf();
    return buffer.str();
}

void files::FileManager::saveTextAt(const char *stringToSave, const std::filesystem::path &fileName) {

}


