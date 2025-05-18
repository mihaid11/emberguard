#include "ShopMenu.h"
#include <stdlib.h>
#include <time.h>
#include <iostream>

ShopMenu::ShopMenu(sf::RenderWindow& window, Inventory& inventory, int& crystals)
	: mInventory(inventory), mItem1Button(sf::Vector2f(0.0f, 0.0f), sf::Vector2f(110.f, 150.f), "1"),
	mItem2Button(sf::Vector2f(0.0f, 0.0f), sf::Vector2f(110.f, 150.f), "2"),
	mItem3Button(sf::Vector2f(0.0f, 0.0f), sf::Vector2f(110.f, 150.f), "3"), mCrystals(crystals)
{
	mMenuShape.setSize(sf::Vector2f(window.getSize().x / 2.0f, window.getSize().y / 2.0f));
	mMenuShape.setFillColor(sf::Color(50, 50, 50, 255));
	mMenuShape.setPosition(
		(window.getSize().x - mMenuShape.getSize().x) / 2,
		(window.getSize().y - mMenuShape.getSize().y) / 2
	);

	mHoveredZoneShape.setSize(sf::Vector2f(window.getSize().x * 3.0f / 4.0f, 40));
	mHoveredZoneShape.setFillColor(sf::Color(10, 10, 10, 100));
	mHoveredZoneShape.setPosition(sf::Vector2f((window.getSize().x - mMenuShape.getSize().x) / 2.0f,
		(window.getSize().y - mMenuShape.getSize().y) / 2.0f));

	if (!mFont.loadFromFile("assets/fonts/gameFont.ttf")) {
		std::cout << "Failed to load font in the shop menu!";
	}
	mTooltipText.setFont(mFont);
	mTooltipText.setCharacterSize(14);
	mTooltipText.setFillColor(sf::Color::White);

	mTooltipBackground.setFillColor(sf::Color(50, 50, 50, 200));
	mTooltipBackground.setOutlineColor(sf::Color::White);
	mTooltipBackground.setOutlineThickness(1.0f);

	mShopText.setFont(mFont);
	mShopText.setCharacterSize(20);
	mShopText.setFillColor(sf::Color::White);
	mShopText.setString("Shop");
	mShopText.setPosition(sf::Vector2f(mMenuShape.getPosition().x + mMenuShape.getSize().x / 2.f - 50.f, mMenuShape.getPosition().y + 8.f));

	mCrystalsText.setFont(mFont);
	mCrystalsText.setCharacterSize(18);
	mCrystalsText.setFillColor(sf::Color::White);
	mCrystalsText.setString("Crystals: ");
	mCrystalsText.setPosition(sf::Vector2f(mMenuShape.getPosition().x + mMenuShape.getSize().x / 2.f - 75.f, 
		mMenuShape.getPosition().y + mMenuShape.getSize().y - 55.f));

	mItem1Button.setPosition(sf::Vector2f(mMenuShape.getPosition().x + mMenuShape.getSize().x / 2 - 55.f - 110.f - 50.f,
		mMenuShape.getPosition().y + mHoveredZoneShape.getSize().y + (mMenuShape.getSize().y - mHoveredZoneShape.getSize().y) / 2
		- 75.f));
	mItem2Button.setPosition(sf::Vector2f(mMenuShape.getPosition().x + mMenuShape.getSize().x / 2 - 55.f,
		mMenuShape.getPosition().y + mHoveredZoneShape.getSize().y + (mMenuShape.getSize().y - mHoveredZoneShape.getSize().y) / 2
		- 75.f));
	mItem3Button.setPosition(sf::Vector2f(mMenuShape.getPosition().x + mMenuShape.getSize().x / 2 + 55.f + 50.f,
		mMenuShape.getPosition().y + mHoveredZoneShape.getSize().y + (mMenuShape.getSize().y - mHoveredZoneShape.getSize().y) / 2
		- 75.f));

	int item1Id = rand() % 3;
	mItemsId.push_back(item1Id);
	int item2Id = rand() % 3;
	mItemsId.push_back(item2Id);
	int item3Id = rand() % 3;
	mItemsId.push_back(item3Id);

	mItem1Button.setCallback([&]() {
		if (mItemsId[0] == 1)
		{
			std::unique_ptr<Wood> woodItem = std::make_unique<Wood>();
			if (woodItem->getPrice() <= crystals)
			{
				crystals -= woodItem->getPrice();
				mInventory.addItem(std::move(woodItem), 1);
			}
		}
		else if (mItemsId[0] == 2)
		{
			std::unique_ptr<TowerBlueprint> towerBlueprintItem = std::make_unique<TowerBlueprint>();
			if (towerBlueprintItem->getPrice() <= crystals)
			{
				crystals -= towerBlueprintItem->getPrice();
				mInventory.addItem(std::move(towerBlueprintItem), 1);
			}
		}
		});
	mItem2Button.setCallback([&]() {
		if (mItemsId[1] == 1)
		{
			std::unique_ptr<Wood> woodItem = std::make_unique<Wood>();
			if (woodItem->getPrice() <= crystals)
			{
				crystals -= woodItem->getPrice();
				mInventory.addItem(std::move(woodItem), 1);
			}
		}
		else if (mItemsId[1] == 2)
		{
			std::unique_ptr<TowerBlueprint> towerBlueprintItem = std::make_unique<TowerBlueprint>();
			if (towerBlueprintItem->getPrice() <= crystals)
			{
				crystals -= towerBlueprintItem->getPrice();
				mInventory.addItem(std::move(towerBlueprintItem), 1);
			}
		}
		});
	mItem3Button.setCallback([&]() {
		if (mItemsId[2] == 1)
		{
			std::unique_ptr<Wood> woodItem = std::make_unique<Wood>();
			if (woodItem->getPrice() <= crystals)
			{
				crystals -= woodItem->getPrice();
				mInventory.addItem(std::move(woodItem), 1);
			}
		}
		else if (mItemsId[2] == 2)
		{
			std::unique_ptr<TowerBlueprint> towerBlueprintItem = std::make_unique<TowerBlueprint>();
			if (towerBlueprintItem->getPrice() <= crystals)
			{
				crystals -= towerBlueprintItem->getPrice();
				mInventory.addItem(std::move(towerBlueprintItem), 1);
			}
		}
		});

	mButtons.push_back(mItem1Button);
	mButtons.push_back(mItem2Button);
	mButtons.push_back(mItem3Button);
}

void ShopMenu::render(sf::RenderWindow& window)
{
	window.draw(mMenuShape);
	window.draw(mHoveredZoneShape);
	window.draw(mShopText);
	window.draw(mCrystalsText);

	for (auto& button : mButtons) {
		button.render(window);
	}

	if (!mTooltipText.getString().isEmpty()) {
		window.draw(mTooltipBackground);
		window.draw(mTooltipText);
	}
}

void ShopMenu::handleMouseClick(const sf::Vector2f& mousePos)
{
	for (auto& button : mButtons) {
		if (button.isMouseOver(mousePos)) {
			button.onClick();
		}
	}
}

void ShopMenu::updateHover(const sf::Vector2f& mousePos)
{
	bool hovered = false;
	int ind = 0;
	for (auto& button : mButtons) {
		button.updateHover(mousePos);
		if (button.getIfHovered()) {
			updateTooltip(ind, mItemsId[ind]);
			hovered = true;
		}
		ind++;
	}

	if (!hovered) {
		mTooltipText.setString("");
	}
	mCrystalsText.setString("Crystals: " + std::to_string(mCrystals));
}

void ShopMenu::regenerateIds()
{
	mItemsId.erase(mItemsId.begin(), mItemsId.end());
	srand(static_cast<unsigned int>(time(NULL)));
  int item1Id = rand() % 3;
	mItemsId.push_back(item1Id);
	int item2Id = rand() % 3;
	mItemsId.push_back(item2Id);
	int item3Id = rand() % 3;
	mItemsId.push_back(item3Id);
}

void ShopMenu::updateTooltip(int slot, int id)
{
	sf::Vector2f basePosition = mMenuShape.getPosition();

	sf::Vector2f slotPosition;
	if (slot == 0) slotPosition = basePosition + sf::Vector2f(60, 120);
	else if (slot == 1) slotPosition = basePosition + sf::Vector2f(210, 120);
	else if (slot == 2) slotPosition = basePosition + sf::Vector2f(360, 120);

	std::string tooltipText;
	if (id == 1) tooltipText = "Cost: 5\nWood\nA piece of wood, useful for crafting.";
	else if (id == 2) tooltipText = "Cost: 50\nTower Blueprint\nCan be used to unlock towers!";
	else tooltipText = "Nothing here!";

	mTooltipText.setString(tooltipText);
	sf::FloatRect textBounds = mTooltipText.getGlobalBounds();

	mTooltipBackground.setSize(sf::Vector2f(textBounds.width + 10, textBounds.height + 10));
	mTooltipBackground.setPosition(slotPosition.x + 10, slotPosition.y - textBounds.height - 10);
	mTooltipText.setPosition(mTooltipBackground.getPosition().x + 5, mTooltipBackground.getPosition().y + 5);
}


