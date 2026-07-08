#pragma once
#include <SFML/Graphics.hpp>
#include "../../ui/Menu.h"
#include "../inventory/Inventory.h"

class ChestMenu : public Menu {
public:
    ChestMenu(const sf::Vector2f& windowSize, Inventory& inventory, Inventory& chestInventory, const sf::Vector2f& slotSize);

    void render(sf::RenderWindow& window) override;
    void handleMouseClick(const sf::Vector2f& mousePos) override;
    void updateHover(const sf::Vector2f& mousePos) override;
    void update(float dt) override;

    int getSlotIndexAtPosition(const sf::Vector2f& pos) const;

private:
    Inventory& mInventory;
    Inventory& mChestInventory;

    std::vector<sf::RectangleShape> mInventorySlots;
    std::vector<sf::RectangleShape> mChestSlots;

    sf::RectangleShape mTooltipBackground;
    sf::Text mTooltipText;
    bool mShowTooltip;
    void updateTooltip(int slotIndex);
    void drawSlots(sf::RenderWindow& window, const std::vector<sf::RectangleShape>& slots, Inventory& inventory, int indexOffset);

    sf::Text mInventoryText;
    sf::Text mChestText;
    int mHoveredSlot;
};
