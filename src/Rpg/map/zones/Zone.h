#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>

class Zone {
public:
    Zone(sf::Vector2i coords, const sf::Texture& tileset, const std::string& csvPath);

    void loadFromCSV(const std::string& csvPath);
    void buildLayer();
    void draw(sf::RenderTarget& target);

private:
    sf::Vector2i mZoneCoords;
    std::vector<std::vector<int>> mTileIDs;
    std::vector<sf::VertexArray> mLayers;
    const sf::Texture* mTilesetTexture;
    int mTileSize;
    int mZoneWidth;
    int mZoneHeight;
};

