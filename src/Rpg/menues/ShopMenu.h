#pragma once
#include "SFML/Graphics.hpp"
#include <vector>
#include "../../ui/Menu.h"
#include "../../ui/Button.h"
#include "../inventory/Inventory.h"
#include "../../core/TimeSystem.h"
#include "../inventory/items/TowerBlueprint.h"
#include "../inventory/items/Wood.h"
#include "../inventory/items/TowerBlueprintRare.h"
#include "../inventory/items/TowerBlueprintEpic.h"
#include "../inventory/items/TowerBlueprintMythic.h"

class ShopMenu : public Menu {
public:
    ShopMenu(const sf::Vector2f& windowSize, Inventory& inventory, TimeSystem& timeSystem,
             int numItems, int& crystals);

    void update(float dt) override;
    void handleMouseClick(const sf::Vector2f& mousePos) override;
    void updateHover(const sf::Vector2f& mousePos) override;
    void render(sf::RenderWindow& window) override;

    void regenerateIds();

private:
    Inventory& mInventory;
    TimeSystem& mTimeSystem;

    std::vector<int> mItemsId;
    std::vector<Button*> mButtons;
    Button mItem1Button;
    Button mItem2Button;
    Button mItem3Button;
    int mNumItems;

    // Rendering of the error message variables
    bool mShowText;
    sf::Text mErrorText;
    sf::Clock mClock;

    //Tooltip elements
    sf::Font mFont;
    sf::Text mShopText;
    sf::Text mTooltipText;
    sf::Text mTimerDisplay;
    sf::RectangleShape mTooltipBackground;

    void buyItem(int buttonNumber);
    void updateTooltip(int slot, int id);
    int& mCrystals;
    sf::Text mCrystalsText;
};
