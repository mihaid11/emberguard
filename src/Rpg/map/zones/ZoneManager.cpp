#include "ZoneManager.h"

ZoneManager::ZoneManager(int viewRange)
    : mViewRange(viewRange), mCurrentZoneCoords(0, 0)
{
}

void ZoneManager::loadTileset(const std::string& path)
{
    if (!mTilesetTexture.loadFromFile(path)) {
        std::cerr << "Failed to load tileset texture: " << path << std::endl;
    }
}

void ZoneManager::update(const sf::Vector2f& playerWorldPos)
{
    int zoneX = static_cast<int>(playerWorldPos.x / (mViewRange * 16 * 32));
    int zoneY = static_cast<int>(playerWorldPos.y / (mViewRange * 16 * 32));
    mCurrentZoneCoords = sf::Vector2i(zoneX, zoneY);

    loadNearbyZones();
}

void ZoneManager::loadNearbyZones()
{
    for (int y = -mViewRange; y <= mViewRange; ++y) {
        for (int x = -mViewRange; x <= mViewRange; ++x) {
            sf::Vector2i zoneCoords = mCurrentZoneCoords + sf::Vector2i(x, y);
            if (mZones.find({ zoneCoords.x, zoneCoords.y }) == mZones.end()) {
                std::string zoneFile = "map_" + std::to_string(zoneCoords.x) + "_" + std::to_string(zoneCoords.y) + ".csv";
                mZones[{zoneCoords.x, zoneCoords.y}] = std::make_unique<Zone>(zoneCoords, mTilesetTexture, zoneFile);
            }
        }
    }
}

void ZoneManager::draw(sf::RenderTarget& target)
{
    for (auto& zonePair : mZones) {
        zonePair.second->draw(target);
    }
}
