#pragma once
#include "Inventory.h"
#include "items/DroppedItem.h"
#include <SFML/Graphics.hpp>

class Hotbar {
public:
    Hotbar(Inventory& inventory, const sf::Vector2f& position, int slotCount, const sf::Vector2f& slotSize);

    void update();
    void render(sf::RenderWindow& window);
    int getHoveredSlot() const;
    void setHoveredSlot(int slot);
    int contains(const sf::Vector2f& mousePos);

private:
    Inventory& mInventory;
    sf::Vector2f mPosition;
    sf::Vector2f mSlotSize;
    int mSlotCount;
    std::vector<sf::RectangleShape> mSlots;
    int mHoveredSlot;
    sf::Font mFont;
    sf::Text mQuantityText;

    void updateSlotColors();
};
