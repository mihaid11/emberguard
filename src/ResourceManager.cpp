#include "ResourceManager.h"

sf::Texture& ResourceManager::getTexture(const std::string& filepath) {
    auto& textures = getInstance().mTextures;
    if (textures.find(filepath) != textures.end())
        return textures.at(filepath);

    sf::Texture& texture = textures[filepath];
    if (!texture.loadFromFile(filepath))
        std::cerr << "Failed to load texture: " << filepath << std::endl;
    return texture;
}
