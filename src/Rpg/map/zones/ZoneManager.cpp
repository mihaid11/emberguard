#include "ZoneManager.h"
#include "Zone.h"
#include <math.h>

const int CHUNK_SIZE = 1024;

ZoneManager::ZoneManager(GameContext& gameContext, int viewRange)
    : mGameContext(gameContext), mViewRange(viewRange), mCurrentZoneCoords(0, 0), mIsInsideAStructure(gameContext.isInsideStructure) {
}

void ZoneManager::update(const sf::Vector2f& playerWorldPos) {
    if (mIsInsideAStructure)
        return;

    int zoneX = static_cast<int>(playerWorldPos.x / (CHUNK_SIZE));
    int zoneY = static_cast<int>(playerWorldPos.y / (CHUNK_SIZE));
    mCurrentZoneCoords = sf::Vector2i(zoneX, zoneY);

    loadNearbyZones();
    unloadFarZones();
}

void ZoneManager::loadNearbyZones() {
    for (int y = -mViewRange; y <= mViewRange; ++y) {
        for (int x = -mViewRange; x <= mViewRange; ++x) {
            sf::Vector2i zoneCoords = mCurrentZoneCoords + sf::Vector2i(x, y);

            if (mZones.find({ zoneCoords.x, zoneCoords.y }) == mZones.end()) {
                std::string zoneFile = "assets/maps/map_" + std::to_string(zoneCoords.x) + "_" + std::to_string(zoneCoords.y) + ".json";

                auto zone = std::make_unique<Zone>(zoneCoords, zoneFile, mGameContext);
                mZones[{zoneCoords.x, zoneCoords.y}] = std::move(zone);
            }
        }
    }
}

void ZoneManager::unloadFarZones() {
    for (auto zone = mZones.begin(); zone != mZones.end();) {
        int x = std::abs(zone->first.first - mCurrentZoneCoords.x);
        int y = std::abs(zone->first.second - mCurrentZoneCoords.y);

        if (x > mViewRange || y > mViewRange)
            zone = mZones.erase(zone);
        else
            ++zone;
    }
}

void ZoneManager::loadInterior(const std::string& path) {
    mZones.clear();

    auto zone = std::make_unique<Zone>(sf::Vector2i(-1, -1), path, mGameContext);
    if (zone->hasCameraTarget())
        mGameContext.cameraFixedPosition = zone->getCameraTarget();
    else
        mGameContext.cameraFixedPosition = {0.f, 0.f};
    mZones[{-1, -1}] = std::move(zone);
}

void ZoneManager::returnToOpenWorld() {
    mZones.clear();
}

bool ZoneManager::checkCollision(const sf::FloatRect& bounds) {
    for (auto& pair: mZones) {
        for (const auto& entity: pair.second->getEntities())
            if (entity->getBounds().intersects(bounds))
                return true;
    }
    return false;
}

Entity* ZoneManager::checkInteraction(const sf::FloatRect& bounds) {
    for (auto& pair: mZones) {
        for (const auto& entity: pair.second->getEntities()) {
            if (entity->isInteractable() && entity->getInteractBounds().intersects(bounds))
                return entity.get();
        }
    }
    return nullptr;
}

void ZoneManager::render(sf::RenderWindow& window) {
    for (auto& pair: mZones)
        pair.second->render(window);
}

std::vector<Zone*> ZoneManager::getActiveZones() {
    std::vector<Zone*> activeZones;
    for (auto& pair: mZones)
        activeZones.push_back(pair.second.get());

    return activeZones;
}

std::vector<Entity*> ZoneManager::getEntities() {
    std::vector<Entity*> entities;

    for (auto& pair: mZones) {
        for (const auto& entity: pair.second->getEntities())
            entities.push_back(entity.get());
    }
    return entities;
}

