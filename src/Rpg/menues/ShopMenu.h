#pragma once
#include "SFML/Graphics.hpp"
#include "../inventory/Inventory.h"
#include <vector>
#include "../../TowerDefense/menues/Button.h"
#include "../inventory/items/TowerBlueprint.h"
#include "../inventory/items/Wood.h"

class ShopMenu
{
public:
	ShopMenu(sf::RenderWindow& window, Inventory& inventory, int& crystals);
	
	void render(sf::RenderWindow& window);
	void handleMouseClick(const sf::Vector2f& mousePos);
	void updateHover(const sf::Vector2f& mousePos);
	void regenerateIds();

private:
	Inventory& mInventory;
	sf::RectangleShape mMenuShape;
	sf::RectangleShape mHoveredZoneShape;

	std::vector<int> mItemsId;
	std::vector<Button> mButtons;
	Button mItem1Button;
	Button mItem2Button;
	Button mItem3Button;

	//Tooltip elements
	sf::Font mFont;
	sf::Text mShopText;
	sf::Text mTooltipText;
	sf::RectangleShape mTooltipBackground;

	void updateTooltip(int slot, int id);
	int& mCrystals;
	sf::Text mCrystalsText;
};

