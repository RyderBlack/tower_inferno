#pragma once
#include <string>

#ifdef _WIN32
#include <windows.h>
#endif

inline std::string getResourcePath() {
    std::string resourcePath;
    
    #ifdef _WIN32
    // On Windows, get the path to the executable
    char path[MAX_PATH] = {0};
    GetModuleFileNameA(NULL, path, MAX_PATH);
    std::string exePath = path;
    // Remove the executable name to get the directory
    size_t lastSlash = exePath.find_last_of("\\/");
    if (lastSlash != std::string::npos) {
        resourcePath = exePath.substr(0, lastSlash + 1);
    }
    #else
    // On other platforms, use the current directory
    resourcePath = "./";
    #endif
    
    return resourcePath;
}

inline std::string getAssetPath(const std::string& relativePath) {
    std::string basePath = getResourcePath();
    std::cout << "Base resource path: " << basePath << std::endl;
    
    // Check if we're running from the build directory
    bool isBuildDir = false;
    if (basePath.find("cmake-build-debug") != std::string::npos || 
        basePath.find("cmake-build-release") != std::string::npos) {
        isBuildDir = true;
        // Go up one level to the project root
        size_t buildPos = basePath.find("cmake-build");
        basePath = basePath.substr(0, buildPos);
    }
    
    std::string fullPath = basePath + "assets/" + relativePath;
    std::cout << "Resolved asset path for '" << relativePath << "': " << fullPath << 
                 " (from " << (isBuildDir ? "build dir" : "project root") << ")" << std::endl;
    
    return fullPath;
}
