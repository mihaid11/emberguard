#pragma once
#include "Zone.h"
#include <SFML/Graphics.hpp>
#include <map>
#include <memory>
#include "../entities/EntityFactory.h"

class ZoneManager {
public:
    ZoneManager(GameContext& gameContext, int viewRange = 1);

    void update(const sf::Vector2f& playerWorldPos);
    void render(sf::RenderWindow& window);

    bool checkCollision(const sf::FloatRect& bounds);
    Entity* checkInteraction(const sf::FloatRect& bounds);

    std::vector<Zone*> getActiveZones();
    std::vector<Entity*> getEntities();

    void loadInterior(const std::string& path);
    void returnToOpenWorld();

private:
    void loadNearbyZones();
    void unloadFarZones();

    GameContext& mGameContext;
    std::map<std::pair<int, int>, std::unique_ptr<Zone>> mZones;
    sf::Vector2i mCurrentZoneCoords;
    int mViewRange;

    bool mIsInsideAStructure;
};

