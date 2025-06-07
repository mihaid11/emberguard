#include "Map.h"
#include "PathsConfig.h"
#include <iostream>

Map::Map(int level) {
    initializeLevel(level);
}

void Map::initializeLevel(int level) {
    mRoad.clear();

    const auto& paths = getPaths();

    if (level < 0 || level >= paths.size())
        return;

    const std::vector<Path>& pathsForLevel = paths[level];
    if (!pathsForLevel.empty()) {
        for (const Path& path : pathsForLevel)
            createRoad(path);
    }
}

void Map::createRoad(const std::vector<sf::Vector2f>& path) {
    for (size_t i = 0; i < path.size() - 1; ++i) {
        sf::Vector2f start = path[i];
        sf::Vector2f end = path[i + 1];

        sf::RectangleShape roadSegment;
        roadSegment.setFillColor(sf::Color(128, 128, 128));

        if (start.x == end.x) {
            roadSegment.setSize({ 50.f, std::abs(end.y - start.y) });
            roadSegment.setPosition(start.x - 25.f, std::min(start.y, end.y));
        } else if (start.y == end.y) {
            roadSegment.setSize({ std::abs(end.x - start.x), 50.f });
            roadSegment.setPosition(std::min(start.x, end.x), start.y - 25.f);
        }

        mRoad.push_back(roadSegment);
    }
}

void Map::render(sf::RenderWindow& window) {
    for (const auto& roadSegment : mRoad)
        window.draw(roadSegment);
}

bool Map::canPlaceTower(const sf::Vector2f& position) const {
    for (const auto& roadSegment : mRoad) {
        if (roadSegment.getGlobalBounds().contains(position))
            return false;
    }
    return true;
}

