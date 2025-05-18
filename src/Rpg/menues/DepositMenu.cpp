#include "DepositMenu.h"
#include <iostream>

DepositMenu::DepositMenu(sf::RenderWindow& window, const sf::Vector2f position,
	const sf::Vector2f size, int& crystals, int& bankBalance, int& storageCapacity)
	:m100Button(sf::Vector2f(0, 0), sf::Vector2f(80.0f, 35.0f), "100"),
	m250Button(sf::Vector2f(0, 0), sf::Vector2f(80.0f, 35.0f), "250"),
	m500Button(sf::Vector2f(0, 0), sf::Vector2f(80.0f, 35.0f), "500"),
	mConfirmButton(sf::Vector2f(0, 0), sf::Vector2f(200.0f, 40.0f), "Confirm Deposit"),
	mBackButton(sf::Vector2f(0, 0), sf::Vector2f(100.0f, 40.0f), "Back"),
	mConfirmShowing(false), mAmountToDeposit(0), mCrystals(crystals), 
	mBankBalance(bankBalance), mStorageWillBeFull(false), mStorageCapacity(storageCapacity),
	mStorageAlreadyFull(false)
{
	if (!mFont.loadFromFile("assets/fonts/gameFont.ttf")) {
		std::cerr << "Failed to load font for BankMenu!" << std::endl;
	}

	mMenuShape.setSize(size);
	mMenuShape.setPosition(position);
	mMenuShape.setFillColor(sf::Color(70, 70, 70, 200));

	mCrystalBalance.setFont(mFont);
	mCrystalBalance.setCharacterSize(16);
	mCrystalBalance.setFillColor(sf::Color::White);
	mCrystalBalance.setString("Crystals: " + std::to_string(crystals));
	mCrystalBalance.setPosition(sf::Vector2f(mMenuShape.getPosition().x + 60.f, mMenuShape.getPosition().y + 175.f));

	mBalance.setFont(mFont);
	mBalance.setCharacterSize(16);
	mBalance.setFillColor(sf::Color::White);
	mBalance.setString("Bank Balance: " + std::to_string(bankBalance));
	mBalance.setPosition(sf::Vector2f(mMenuShape.getPosition().x + 60.f, mMenuShape.getPosition().y + 100.f));

	mAmount.setFont(mFont);
	mAmount.setCharacterSize(16);
	mAmount.setFillColor(sf::Color::White);
	mAmount.setString("Deposit Amount: ");
	mAmount.setPosition(sf::Vector2f(mMenuShape.getPosition().x + 60.f, mMenuShape.getPosition().y + 25.f));

	mStorageFullText.setFont(mFont);
	mStorageFullText.setCharacterSize(16);
	mStorageFullText.setFillColor(sf::Color::White);
	mStorageFullText.setString("You can't deposit anymore. Storage Capacity is full!");
	mStorageFullText.setPosition(sf::Vector2f(mMenuShape.getPosition().x + 60.f, mMenuShape.getPosition().y + 310.f));

	mCanDepositOnlyText.setFont(mFont);
	mCanDepositOnlyText.setCharacterSize(16);
	mCanDepositOnlyText.setFillColor(sf::Color::White);
	mCanDepositOnlyText.setString("Storage capacity is limited!");
	mCanDepositOnlyText.setPosition(sf::Vector2f(mMenuShape.getPosition().x + 60.f, mMenuShape.getPosition().y + 250.f));


	m100Button.setPosition(sf::Vector2f(mMenuShape.getPosition().x + 300.f,
		mMenuShape.getPosition().y + 25.f - 8.75f));
	m250Button.setPosition(sf::Vector2f(mMenuShape.getPosition().x + 300.f + 110.f,
		mMenuShape.getPosition().y + 25.f - 8.75f));
	m500Button.setPosition(sf::Vector2f(mMenuShape.getPosition().x + 300.f + 220.f,
		mMenuShape.getPosition().y + 25.f - 8.75f));
	mConfirmButton.setPosition(sf::Vector2f(mMenuShape.getPosition().x + 100.f, mMenuShape.getPosition().y + 310.f));
	mBackButton.setPosition(sf::Vector2f(mMenuShape.getPosition().x + 400.f, mMenuShape.getPosition().y + 310.f));

	m100Button.setCallback([&]() {
		if (crystals >= 100) {
			if (bankBalance == storageCapacity)
				mStorageAlreadyFull = true;
			else if (bankBalance + 100 > storageCapacity)
				mStorageWillBeFull = true;
			else
			{
				mAmountToDeposit = 100;
				mConfirmShowing = true;
				mStorageAlreadyFull = false;
				mStorageWillBeFull = false;
			}
		}
		else {
			mAmountToDeposit = 0;
			mConfirmShowing = false;
		}
		});

	m250Button.setCallback([&]() {
		if (crystals >= 250) {
			if (bankBalance == storageCapacity)
				mStorageAlreadyFull = true;
			else if (bankBalance + 250 > storageCapacity)
				mStorageWillBeFull = true;
			else
			{
				mAmountToDeposit = 250;
				mConfirmShowing = true;
				mStorageAlreadyFull = false;
				mStorageWillBeFull = false;
			}
		}
		else {
			mAmountToDeposit = 0;
			mConfirmShowing = false;
		}
		});

	m500Button.setCallback([&]() {
		if (crystals >= 500) {
			if (bankBalance == storageCapacity)
				mStorageAlreadyFull = true;
			else if (bankBalance + 500 > storageCapacity)
				mStorageWillBeFull = true;
			else
			{
				mAmountToDeposit = 500;
				mConfirmShowing = true;
				mStorageAlreadyFull = false;
				mStorageWillBeFull = false;
			}
		}
		else {
			mAmountToDeposit = 0;
			mConfirmShowing = false;
		}
		});

	mConfirmButton.setCallback([&]() {
		if (mStorageWillBeFull == false)
		{
			bankBalance += mAmountToDeposit;
			crystals -= mAmountToDeposit;
		}
		else
		{
			crystals -= storageCapacity - bankBalance;
			bankBalance = storageCapacity;
		}
		mAmountToDeposit = 0;
		mConfirmShowing = false;
		});

	mBackButton.setCallback([&]() {
		mAmountToDeposit = 0;
		mConfirmShowing = false;
		});
}

void DepositMenu::render(sf::RenderWindow& window)
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
		if (mStorageWillBeFull)
			window.draw(mCanDepositOnlyText);
	}
	
	if (mStorageAlreadyFull)
		window.draw(mStorageFullText);
}

void DepositMenu::handleClicks(const sf::Vector2f& mousePos)
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

void DepositMenu::updateHover(const sf::Vector2f& mousePos)
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

void DepositMenu::update()
{
	if (mConfirmShowing) {
		if (mStorageWillBeFull == true) {
			int amount = mStorageCapacity - mBankBalance;
			mBalance.setString("Bank Balance: " + std::to_string(mBankBalance) + " + " +
				std::to_string(amount) + " / " + std::to_string(mStorageCapacity));
			mCanDepositOnlyText.setString("Storage capacity is limited! You can deposit only " + 
			std::to_string(amount) + " crystals");
			mCrystalBalance.setString("Crystals: " + std::to_string(mCrystals) + " - " +
				std::to_string(amount));
		}
		else
		{
			mBalance.setString("Bank Balance: " + std::to_string(mBankBalance) + " + " +
				std::to_string(mAmountToDeposit) + " / " + std::to_string(mStorageCapacity));
			mCrystalBalance.setString("Crystals: " + std::to_string(mCrystals) + " - " +
				std::to_string(mAmountToDeposit));
		}
	}
	else {
		mCrystalBalance.setString("Crystals: " + std::to_string(mCrystals));
		mBalance.setString("Bank Balance: " + std::to_string(mBankBalance) + " / " +
		std::to_string(mStorageCapacity));
	}
}

void DepositMenu::restart()
{
	mConfirmShowing = false;
}
