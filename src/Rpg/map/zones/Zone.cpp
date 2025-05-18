#include "Zone.h"
#include <exception>

Zone::Zone(sf::Vector2i coords, const sf::Texture& tileset, const std::string& csvPath)
    : mZoneCoords(coords), mTilesetTexture(&tileset), 
    mTileSize(32), mZoneWidth(16), mZoneHeight(16)
{
    loadFromCSV(csvPath);
    buildLayer();
}

void Zone::loadFromCSV(const std::string& csvPath)
{
    std::ifstream file(csvPath);
    if (!file.is_open()) {
        std::cerr << "Failed to open CSV file: " << csvPath << std::endl;
        return;
    }

    mTileIDs.resize(mZoneHeight, std::vector<int>(mZoneWidth, 0));
    std::string line;
    int row = 0;

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        int col = 0;
        std::string tileIDStr;
        while (std::getline(ss, tileIDStr, ',') && col < mZoneWidth) {
            try {
                mTileIDs[row][col] = std::stoi(tileIDStr);
            }
            catch (const std::exception e) {
                std::cerr << "Invalid tile ID in CSV file at [" << row << "][" << col << "]: " << tileIDStr << std::endl;
            }
            col++;
        }
        row++;
        if (row >= mZoneHeight) break;
    }

    file.close();
}

void Zone::buildLayer()
{
    mLayers.clear();

    sf::VertexArray layer(sf::Quads, mZoneWidth * mZoneHeight * 4);

    for (int y = 0; y < mZoneHeight; y++) {
        for (int x = 0; x < mZoneWidth; x++) {
            if (y >= mTileIDs.size() || x >= mTileIDs[y].size()) {
                std::cerr << "Out of bounds access at [" << y << "][" << x << "]" << std::endl;
                continue;
            }

            int tileID = mTileIDs[y][x];

            float tileX = (float)(x * mTileSize);
            float tileY = (float)(y * mTileSize);

            int tilesPerRow = mTilesetTexture->getSize().x / mTileSize;
            float tileXInTexture = (float)((tileID % tilesPerRow) * mTileSize);
            float tileYInTexture = (float)((tileID / tilesPerRow) * mTileSize);

            sf::Vertex* quad = &layer[(x + y * mZoneWidth) * 4];
            quad[0].position = sf::Vector2f(tileX, tileY);
            quad[1].position = sf::Vector2f(tileX + mTileSize, tileY);
            quad[2].position = sf::Vector2f(tileX + mTileSize, tileY + mTileSize);
            quad[3].position = sf::Vector2f(tileX, tileY + mTileSize);

            quad[0].texCoords = sf::Vector2f(tileXInTexture, tileYInTexture);
            quad[1].texCoords = sf::Vector2f(tileXInTexture + mTileSize, tileYInTexture);
            quad[2].texCoords = sf::Vector2f(tileXInTexture + mTileSize, tileYInTexture + mTileSize);
            quad[3].texCoords = sf::Vector2f(tileXInTexture, tileYInTexture + mTileSize);
        }
    }

    mLayers.push_back(layer);
}

void Zone::draw(sf::RenderTarget& target)
{
    for (auto& layer : mLayers) {
        target.draw(layer, mTilesetTexture);
    }
}
