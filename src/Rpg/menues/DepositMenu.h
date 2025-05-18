#pragma once
#include <SFML/Graphics.hpp>
#include "../../TowerDefense/menues/Button.h"

class DepositMenu
{
public:
	DepositMenu(sf::RenderWindow& window, const sf::Vector2f position,
		const sf::Vector2f size, int& crystals, int& bankBalances, int& storageCapacity);
	void render(sf::RenderWindow& window);
	void handleClicks(const sf::Vector2f& mousePos);
	void updateHover(const sf::Vector2f& mousePos);
	void update();
	void restart();

private:
	sf::RectangleShape mMenuShape;
	Button m100Button;
	Button m250Button;
	Button m500Button;
	Button mConfirmButton;
	Button mBackButton;

	sf::Text mBalance;
	sf::Text mCrystalBalance;
	sf::Text mAmount;
	sf::Text mCanDepositOnlyText;
	sf::Font mFont;

	bool mConfirmShowing;
	bool mStorageWillBeFull;
	int mAmountToDeposit;
	int& mCrystals;
	int& mBankBalance;
	int& mStorageCapacity;

	bool mStorageAlreadyFull;
	sf::Text mStorageFullText;
};

