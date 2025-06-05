#ifndef TOWER_INFERNO_RESOURCEMANAGER_H
#define TOWER_INFERNO_RESOURCEMANAGER_H

#include <raylib.h>
#include <string>
#include <unordered_map>
#include <memory>

class ResourceManager {
private:
    static ResourceManager* instance;
    
    // Resource storage
    std::unordered_map<std::string, Texture2D> textures;
    std::unordered_map<std::string, Model> models;
    std::unordered_map<std::string, Sound> sounds;
    std::unordered_map<std::string, Music> music;
    std::unordered_map<std::string, Font> fonts;
    
    // Private constructor for singleton
    ResourceManager() = default;
    
public:
    // Delete copy constructor and assignment operator
    ResourceManager(const ResourceManager&) = delete;
    void operator=(const ResourceManager&) = delete;
    
    // Get singleton instance
    static ResourceManager& getInstance();
    
    // Load and unload resources
    bool loadResources();
    void unloadResources();
    
    // Resource accessors
    Texture2D& getTexture(const std::string& name);
    Model& getModel(const std::string& name);
    Sound& getSound(const std::string& name);
    Music& getMusic(const std::string& name);
    Font& getFont(const std::string& name);
    
    // Helper functions
    static std::string getAssetPath(const std::string& type, const std::string& filename);
};

#endif //TOWER_INFERNO_RESOURCEMANAGER_H
