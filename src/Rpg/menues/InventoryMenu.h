#pragma once
#include <SFML/Graphics.hpp>
#include "../../ui/Menu.h"
#include "../mainCharacter/MainCharacter.h"
#include "../inventory/Inventory.h"
#include "../inventory/items/DroppedItem.h"

class InventoryMenu : public Menu {
public:
    InventoryMenu(const sf::Vector2f& position, const sf::Vector2f& size, Inventory& inventory,
                  MainCharacter& character, const sf::Vector2f& slotSize);

    void render(sf::RenderWindow& window) override;
    void handleMouseClick(const sf::Vector2f& mousePos) override;
    void updateHover(const sf::Vector2f& mousePos) override;
    void update(float dt) override;

    int getHoveredSlot() const;
    void restart();

private:
    MainCharacter& mCharacter;
    sf::Sprite mCharacterSprite;

    Inventory& mInventory;
    std::vector<sf::RectangleShape> mSlots;
    sf::Vector2f mSlotSize;

    int mHoveredSlot;
    int mDraggedSlot;
    float mGap;

    sf::Text mLevelText;
    sf::Text mXpText;
    sf::RectangleShape mXpBar;
    sf::RectangleShape mXpBarFill;

    // Tooltip elements
    sf::RectangleShape mTooltipBackground;
    sf::Text mTooltipText;
    bool mShowTooltip;
    sf::Font mFont;

    void initializeLayout();
    void updateSlotColors();
    int getSlotIndexAtPosition(const sf::Vector2f& pos) const;
    void updateTooltip();
};
