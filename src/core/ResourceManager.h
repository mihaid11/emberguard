#pragma once
#include <SFML/Graphics.hpp>
#include <map>
#include <string>
#include <iostream>

class ResourceManager {
public:
    static ResourceManager& getInstance() {
        static ResourceManager instance;
        return instance;
    }

    sf::Texture& getTexture(const std::string& filepath);

private:
    std::map<std::string, sf::Texture> mTextures;
    ResourceManager() {};
};

