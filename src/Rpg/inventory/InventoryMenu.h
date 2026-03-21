#pragma once
#include <SFML/Graphics.hpp>
#include "../mainCharacter/MainCharacter.h"
#include "Inventory.h"
#include "items/DroppedItem.h"

class InventoryMenu {
public:
    InventoryMenu(Inventory& inventory, const sf::Vector2f& position,
                  MainCharacter& character, const sf::Vector2f& slotSize);

    void render(sf::RenderWindow& window);
    void handleMouseClick(const sf::Vector2f& mousePos);
    void updateHover(const sf::Vector2f& mousePos);
    void handleDragAndDrop(const sf::Vector2f& mousePos);

    void update();
    int getHoveredSlot() const;

    void restart();

private:
    MainCharacter& mCharacter;
    sf::Sprite mCharacterSprite;

    Inventory& mInventory;
    std::vector<sf::RectangleShape> mSlots;
    sf::Vector2f mSlotSize;
    sf::Vector2f mPosition;
    int mHoveredSlot;
    int mDraggedSlot;
    float mGap;

    // Tooltip elements
    sf::RectangleShape mTooltipBackground;
    sf::Text mTooltipText;
    sf::Font mFont;

    void updateSlotColors();
    int getSlotIndexAtPosition(const sf::Vector2f& pos) const;
    void updateTooltip();
};

