#pragma once
#include <SFML/Graphics.hpp>
#include "../inventory/Inventory.h"

class ChestMenu {
public:
    ChestMenu(sf::RenderWindow& window, Inventory& inventory, Inventory& chestInventory, const sf::Vector2f& slotSize);

    void render(sf::RenderWindow& window);
    void handleMouseClick(const sf::Vector2f& mousePos);
    void updateHover(const sf::Vector2f& mousePos);

    int getSlotIndexAtPosition(const sf::Vector2f& pos) const;

private:
    sf::RectangleShape mMenuShape;
    sf::RectangleShape mHoveredZoneShape;
    sf::Font mFont;

    Inventory& mInventory;
    Inventory& mChestInventory;
    std::vector<sf::RectangleShape> mInventorySlots;
    std::vector<sf::RectangleShape> mChestSlots;
    sf::RectangleShape mTooltipBackground;
    sf::Text mTooltipText;
    sf::Text mInventoryText;
    sf::Text mChestText;
    int mHoveredSlot;
};

