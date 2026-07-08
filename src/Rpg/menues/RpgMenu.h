#pragma once
#include <SFML/Graphics.hpp>
#include "../../ui/Menu.h"
#include "../../ui/Button.h"
#include <memory>
#include <vector>
#include "../menues/SkillTreeMenu.h"
#include "../skillTree/SkillTree.h"
#include "../menues/InventoryMenu.h"
#include "../inventory/items/DroppedItem.h"

class RPGEngine;
class GameManager;

class RpgMenu : public Menu {
public:
    RpgMenu(const sf::Vector2f& windowSize, SkillTree& skillTree, Inventory& inventory,
            RPGEngine& rpgEngine, GameManager* gameManager, int& crystals);

    void render(sf::RenderWindow& window) override;
    void handleMouseClick(const sf::Vector2f& mousePos) override;
    void updateHover(const sf::Vector2f& mousePos) override;
    void update(float dt) override;

    void switchToMenu(const std::string& menuName);
    void restart();

    const std::string& getMenuType() const;
    InventoryMenu& getInventoryMenu();

private:
    GameManager* mGameManager;
    int& mCrystals;

    std::vector<std::unique_ptr<Button>> mButtons;
    sf::Vector2f mButtonSize;
    float mGap;

    std::string mCurrentMenu;
    std::unique_ptr<SkillTreeMenu> mSkillTreeMenu;
    std::unique_ptr<InventoryMenu> mInventoryMenu;

    sf::Text mCrystalText;

    bool mShowText;
    sf::Text mErrorText;
    sf::Clock mClock;
};
