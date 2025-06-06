#pragma once
#include "Zone.h"
#include <SFML/Graphics.hpp>
#include <map>
#include <memory>

class ZoneManager
{
public:
    ZoneManager(int viewRange = 3);

    void loadTileset(const std::string& path);
    void update(const sf::Vector2f& playerWorldPos);
    void loadNearbyZones();
    void draw(sf::RenderTarget& target);

private:
    std::map<std::pair<int, int>, std::unique_ptr<Zone>> mZones;
    sf::Texture mTilesetTexture;
    sf::Vector2i mCurrentZoneCoords;
    int mViewRange;
};
