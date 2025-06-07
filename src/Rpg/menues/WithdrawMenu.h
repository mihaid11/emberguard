#pragma once
#include <SFML/Graphics.hpp>
#include "../../TowerDefense/menues/Button.h"

class WithdrawMenu
{
public:
	WithdrawMenu(sf::RenderWindow& window, const sf::Vector2f position,
		const sf::Vector2f size, int& crystals, int& bankBalances, int& storageCapacity);
	void render(sf::RenderWindow& window);
	void handleClicks(const sf::Vector2f& mousePos);
	void updateHover(const sf::Vector2f& mousePos);
	void update();
	void restart();

private:
	void withdrawAmount(int amount, int& bankBalance); // called when withdraw buttons are clicked
	sf::Text createMessageText(std::string string, float height);
	sf::RectangleShape mMenuShape;
	Button m100Button;
	Button m250Button;
	Button m500Button;
	Button mConfirmButton;
	Button mBackButton;

	sf::Text mBalance;
	sf::Text mCrystalBalance;
	sf::Text mInsufficientFundsText;
	sf::Text mAmount;
	sf::Font mFont;

	bool mConfirmShowing;
	bool mInsufficientFunds;
	int mAmountToWithdraw;
	int& mCrystals;
	int& mBankBalance;
	int& mStorageCapacity;
};

