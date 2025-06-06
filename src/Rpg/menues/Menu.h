#pragma once
#include <SFML/Graphics.hpp>
#include "../../TowerDefense/menues/Button.h"
#include <memory>
#include <vector>
#include "../skillTree/SkillTreeMenu.h"
#include "../skillTree/SkillTree.h"
#include "../inventory/InventoryMenu.h"
#include "../inventory/items/DroppedItem.h"

class RPGEngine;

class Menu
{
public:
    Menu(sf::RenderWindow& window, SkillTree& skillTree, Inventory& inventory,
        const sf::Vector2f& playerPos, std::vector<DroppedItem>& droppedItems, RPGEngine& rpgEngine);

    void render(sf::RenderWindow& window);
    void handleMouseClick(const sf::Vector2f& mousePos);
    void updateHover(const sf::Vector2f& mousePos);
    void switchToMenu(const std::string& menuName);
    void restart();

    void update(int crystals, const Inventory& inventory, const SkillTree& skillTree);
    const std::string& getMenuType() const;
    InventoryMenu& getInventoryMenu();

private:
    sf::RectangleShape mMenuShape;
    sf::RectangleShape mHoveredZoneShape;

    std::vector<Button> mButtons;
    Button skillTreeButton;
    Button inventoryButton;
    Button exitButton;

    std::string mCurrentMenu;
    std::unique_ptr<SkillTreeMenu> mSkillTreeMenu;
    std::unique_ptr<InventoryMenu> mInventoryMenu;
    sf::Font mFont;
    sf::Text mCrystalText;

    bool mShowText;
    sf::Text mErrorText;
    sf::Clock mClockText;
};
