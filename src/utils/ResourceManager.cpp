#include "ResourceManager.h"
#include <raylib.h>
#include <iostream>

// Initialize static member
ResourceManager* ResourceManager::instance = nullptr;

ResourceManager& ResourceManager::getInstance() {
    if (!instance) {
        instance = new ResourceManager();
    }
    return *instance;
}

bool ResourceManager::loadResources() {
    // Load textures
    // Example: textures["background"] = LoadTexture(getAssetPath("textures", "background.png").c_str());
    
    // Load models
    // Example: models["tower"] = LoadModel(getAssetPath("models", "tower.obj").c_str());
    
    // Load sounds
    // Example: sounds["shoot"] = LoadSound(getAssetPath("sounds", "shoot.wav").c_str());
    
    // Load music
    // Example: music["main_theme"] = LoadMusicStream(getAssetPath("sounds", "main_theme.mp3").c_str());
    
    // Load fonts
    fonts["default"] = GetFontDefault();
    // Example: fonts["main"] = LoadFont(getAssetPath("fonts", "font.ttf").c_str());
    
    return true;
}

void ResourceManager::unloadResources() {
    // Unload textures
    for (auto& pair : textures) {
        UnloadTexture(pair.second);
    }
    textures.clear();
    
    // Unload models
    for (auto& pair : models) {
        UnloadModel(pair.second);
    }
    models.clear();
    
    // Unload sounds
    for (auto& pair : sounds) {
        UnloadSound(pair.second);
    }
    sounds.clear();
    
    // Unload music
    for (auto& pair : music) {
        UnloadMusicStream(pair.second);
    }
    music.clear();
    
    // Unload fonts (except default)
    for (auto& pair : fonts) {
        if (pair.first != "default") {
            UnloadFont(pair.second);
        }
    }
    fonts.clear();
}

Texture2D& ResourceManager::getTexture(const std::string& name) {
    auto it = textures.find(name);
    if (it == textures.end()) {
        // Return a default texture or handle error
        static Texture2D defaultTex = {0};
        return defaultTex;
    }
    return it->second;
}

Model& ResourceManager::getModel(const std::string& name) {
    auto it = models.find(name);
    if (it == models.end()) {
        // Return a default model or handle error
        static Model defaultModel = {0};
        return defaultModel;
    }
    return it->second;
}

Sound& ResourceManager::getSound(const std::string& name) {
    auto it = sounds.find(name);
    if (it == sounds.end()) {
        // Return a default sound or handle error
        static Sound defaultSound = {0};
        return defaultSound;
    }
    return it->second;
}

Music& ResourceManager::getMusic(const std::string& name) {
    auto it = music.find(name);
    if (it == music.end()) {
        // Return a default music or handle error
        static Music defaultMusic = {0};
        return defaultMusic;
    }
    return it->second;
}

Font& ResourceManager::getFont(const std::string& name) {
    auto it = fonts.find(name);
    if (it == fonts.end()) {
        // Return default font
        return fonts["default"];
    }
    return it->second;
}

std::string ResourceManager::getAssetPath(const std::string& type, const std::string& filename) {
    return "assets/" + type + "/" + filename;
}
