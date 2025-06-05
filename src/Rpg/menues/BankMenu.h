#pragma once
#include <SFML/Graphics.hpp>
#include "../../TowerDefense/menues/Button.h"
#include "../../TimeSystem.h"
#include "WithdrawMenu.h"
#include "DepositMenu.h"
#include "BorrowMenu.h"
#include <vector>
#include <string>
#include <memory>

class BankMenu
{
public:
	BankMenu(sf::RenderWindow& window, int& crystals, int& storageCapacity,
        TimeSystem& timeSystem);
	void render(sf::RenderWindow& window);
    void update();
    void handleMouseClick(const sf::Vector2f& mousePos);
    void updateHover(const sf::Vector2f& mousePos);

    void switchToMenu(const std::string& menuName);
    int getBankBalance();
    void setBankBalance(int bankBalance);

    int getLoanAmount();
    void resetBorrowStats();
    void setBorrowStats(bool hasBorrowActive, int penalty, int interest, int amountToRepay, int daysToRepayment,
                        int startYear, int startDay, int startHour, int startMinute);

    int getPenalty();
    int getDaysToRepayment();
    int getInterest();
    int getAmounToRepay();
    int getStartYear();
    int getStartDay();
    int getStartHour();
    int getStartMinute();

    bool hasActiveBorrow();

    void restart();

private:
    sf::Text mTitle;
    sf::Font mFont;
    sf::RectangleShape mMenuShape;
    sf::RectangleShape mHoveredZoneShape;
    Button mDepositButton;
    Button mWithdrawButton;
    Button mBorrowButton;
    std::vector<Button> mButtons;

    std::unique_ptr<WithdrawMenu> mWithdrawMenu;
    std::unique_ptr<DepositMenu> mDepositMenu;
    std::unique_ptr<BorrowMenu> mBorrowMenu;

    int& mCrystals;
    std::string mCurrentMenu;
    int mBankBalance;

    bool mHasBorrowActive;
    TimeSystem& mTimeSystem;
};

