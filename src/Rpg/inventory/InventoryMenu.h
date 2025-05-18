#pragma once
#include <SFML/Graphics.hpp>
#include "Inventory.h"
#include "items/DroppedItem.h"

class InventoryMenu {
public:
    InventoryMenu(Inventory& inventory, const sf::Vector2f& position, const sf::Vector2f& slotSize,
        const sf::Vector2f& playerPos, std::vector<DroppedItem>& droppedItems);

    void render(sf::RenderWindow& window);
    void handleMouseClick(const sf::Vector2f& mousePos);
    void updateHover(const sf::Vector2f& mousePos);
    void handleDragAndDrop(const sf::Vector2f& mousePos);

    void update(const Inventory& inventory);
    int getHoveredSlot() const;

    void restart();

private:
    Inventory& mInventory;
    std::vector<sf::RectangleShape> mSlots;
    sf::Vector2f mSlotSize;
    sf::Vector2f mPosition;
    const sf::Vector2f& mPlayerPos;
    int mHoveredSlot;
    int mDraggedSlot;

    // Tooltip elements
    sf::RectangleShape mTooltipBackground;
    sf::Text mTooltipText;
    sf::Font mFont;

    void updateSlotColors();
    int getSlotIndexAtPosition(const sf::Vector2f& pos) const;
    void updateTooltip();

    std::vector<DroppedItem>& mDroppedItems;
};
