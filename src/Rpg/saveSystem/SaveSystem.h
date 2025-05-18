#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "../inventory/items/DroppedItem.h"
#include "../inventory/Inventory.h"

class SaveSystem
{
public:
    SaveSystem(const std::string& saveFilePath);

    void save(const sf::Vector2f& playerPosition, const std::vector<sf::Vector2f>& npcPositions, const std::vector<int>& npcWaypoints,
        const int& crystals, const int& year, const int& day, const int& hour,
        const int& minute, const int& bankBalance, const int& hasBorrowActive, 
        const int& penalty, const int& interest, const int& amountToRepay,
        const int& startYear, const int& startDay, const int& startHour, const int& startMinute,
        const std::vector<int>& inventoryItemId, const std::vector<int>& inventoryItemQuantity,
        const std::vector<int>& droppedItemId, const std::vector<float>& droppedItemXPos,
        const std::vector<float>& droppedItemYPos, const std::vector<int>& droppedItemQuantity,
        const int& extracting, const int& inSlot, const int& completed, const int& timerActive,
        const int& startYear1, const int& startDay1, const int& startHour1, const int& startMinute1,
        const int& slotItemid, const int& insideStructure, const sf::Vector2f& fixedCameraPos);
    bool load(sf::Vector2f& playerPosition, std::vector<sf::Vector2f>& npcPositions, std::vector<int>& npcWaypoints,
        int& crystals, int& year, int& day, int& hour, int& minute, int& bankBalance,
        int& hasBorrowActive, int& penalty, int& interest, int& amountToRepay, int& startYear,
        int& startDay, int& startHour, int& startMinute,
        std::vector<int>& inventoryItemId, std::vector<int>& inventoryItemQuantity, 
        std::vector<int>& droppedItemId, std::vector<float>& droppedItemXPos, 
        std::vector<float>& droppedItemYPos, std::vector<int>& droppedItemQuantity,
        int& extracting, int& inSlot, int& completed, int& timerActive, int& startYear1,
        int& startDay1, int& startHour1, int& startMinute1, int& slotItemId,
        int& insideStructure, sf::Vector2f& fixedCameraPos);

private:
    std::string mSaveFilePath;
};
