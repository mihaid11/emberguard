#include "WithdrawMenu.h"
#include <iostream>

WithdrawMenu::WithdrawMenu(sf::RenderWindow& window, const sf::Vector2f position,
	const sf::Vector2f size, int& crystals, int& bankBalance, int& storageCapacity)
	:m100Button(sf::Vector2f(0, 0), sf::Vector2f(80.0f, 35.0f), "100"),
	m250Button(sf::Vector2f(0, 0), sf::Vector2f(80.0f, 35.0f), "250"),
	m500Button(sf::Vector2f(0, 0), sf::Vector2f(80.0f, 35.0f), "500"),
	mConfirmButton(sf::Vector2f(0, 0), sf::Vector2f(200.0f, 40.0f), "Confirm Withdrawal"),
	mBackButton(sf::Vector2f(0, 0), sf::Vector2f(100.0f, 40.0f), "Back"),
	mConfirmShowing(false), mAmountToWithdraw(0), mCrystals(crystals), mBankBalance(bankBalance),
	mStorageCapacity(storageCapacity)
{
	if (!mFont.loadFromFile("assets/fonts/gameFont.ttf")) {
		std::cerr << "Failed to load font for BankMenu!" << std::endl;
	}

	mMenuShape.setSize(size);
	mMenuShape.setPosition(position);
	mMenuShape.setFillColor(sf::Color(70, 70, 70, 200));

	mCrystalBalance = createMessageText("Crystals: " + std::to_string(crystals), 175.f);
	mBalance = createMessageText("Bank Balance: "  + std::to_string(bankBalance), 100.f);
	mAmount = createMessageText("Withdraw Amount: ", 25.f);
	mInsufficientFundsText = createMessageText("Insufficient funds in bank", 310.f);

	m100Button.setPosition(sf::Vector2f(mMenuShape.getPosition().x + 300.f, 
		mMenuShape.getPosition().y + 25.f - 8.75f));
	m250Button.setPosition(sf::Vector2f(mMenuShape.getPosition().x + 300.f + 110.f, 
		mMenuShape.getPosition().y + 25.f - 8.75f));
	m500Button.setPosition(sf::Vector2f(mMenuShape.getPosition().x + 300.f + 220.f, 
		mMenuShape.getPosition().y + 25.f - 8.75f));
	mConfirmButton.setPosition(sf::Vector2f(mMenuShape.getPosition().x + 75.f, mMenuShape.getPosition().y + 310.f));
	mBackButton.setPosition(sf::Vector2f(mMenuShape.getPosition().x + 450.f, mMenuShape.getPosition().y + 310.f));

	m100Button.setCallback([&]() {
		withdrawAmount(100, bankBalance);
		});

	m250Button.setCallback([&]() {
		withdrawAmount(250, bankBalance);
		});

	m500Button.setCallback([&]() {
		withdrawAmount(500, bankBalance);
		});

	mConfirmButton.setCallback([&]() {
		bankBalance -= mAmountToWithdraw;
		crystals += mAmountToWithdraw;
		mAmountToWithdraw = 0;
		mConfirmShowing = false;
		});
	
	mBackButton.setCallback([&]() {
		mAmountToWithdraw = 0;
		mConfirmShowing = false;
		mInsufficientFunds = false;
		});	
}

void WithdrawMenu::render(sf::RenderWindow& window)
{
	window.draw(mMenuShape);
	window.draw(mBalance);
	window.draw(mAmount);
	window.draw(mCrystalBalance);
	m100Button.render(window);
	m250Button.render(window);
	m500Button.render(window);

	if (mConfirmShowing)
	{
		mConfirmButton.render(window);
		mBackButton.render(window);
	}
	if (mInsufficientFunds)
		window.draw(mInsufficientFundsText);
}

void WithdrawMenu::handleClicks(const sf::Vector2f& mousePos)
{
	if (m100Button.isMouseOver(mousePos))
		m100Button.onClick();
	if (m250Button.isMouseOver(mousePos))
		m250Button.onClick();
	if (m500Button.isMouseOver(mousePos))
		m500Button.onClick();

	if (mConfirmShowing)
	{
		if (mConfirmButton.isMouseOver(mousePos))
			mConfirmButton.onClick();
		if (mBackButton.isMouseOver(mousePos))
			mBackButton.onClick();
	}
}

void WithdrawMenu::updateHover(const sf::Vector2f& mousePos)
{
	m100Button.updateHover(mousePos);
	m250Button.updateHover(mousePos);
	m500Button.updateHover(mousePos);

	if (mConfirmShowing)
	{
		mConfirmButton.updateHover(mousePos);
		mBackButton.updateHover(mousePos);
	}
}

void WithdrawMenu::update()
{
	if (mConfirmShowing) {
		mCrystalBalance.setString("Crystals: " + std::to_string(mCrystals) + " + " + 
			std::to_string(mAmountToWithdraw));
		mBalance.setString("Bank Balance: " + std::to_string(mBankBalance) + " - " +
			std::to_string(mAmountToWithdraw) + " / " + std::to_string(mStorageCapacity));
	}
	else {
		mCrystalBalance.setString("Crystals: " + std::to_string(mCrystals));
		mBalance.setString("Bank Balance: " + std::to_string(mBankBalance) + " / " +
		std::to_string(mStorageCapacity));
	}
}

void WithdrawMenu::restart()
{
	mConfirmShowing = false;
	mInsufficientFunds = false;
}

void WithdrawMenu::withdrawAmount(int amount, int& bankBalance)
{
	if (bankBalance >= amount) {
			mAmountToWithdraw = amount;
			mConfirmShowing = true; // show button to confirm amount to be withdrawn
			mInsufficientFunds = false;
		}
		else {
			mAmountToWithdraw = 0;
			mConfirmShowing = false;
			mInsufficientFunds = true; // show low funds error message
		}
}

sf::Text WithdrawMenu::createMessageText(std::string string, float height) // DUPLICATE FUNCTION IN DEPOSITMENU.CPP
{
	sf::Text output;
	output.setFont(mFont);
	output.setCharacterSize(16);
	output.setFillColor(sf::Color::White);
	output.setString(string);
	output.setPosition(sf::Vector2f(mMenuShape.getPosition().x + 60.f, mMenuShape.getPosition().y + height));	
	return output;
}