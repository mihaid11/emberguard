#pragma once
#include <SFML/Graphics.hpp>
#include "../../ui/Menu.h"
#include "../../ui/Button.h"
#include "../../core/TimeSystem.h"
#include "WithdrawMenu.h"
#include "DepositMenu.h"
#include "BorrowMenu.h"
#include <vector>
#include <string>
#include <memory>

class BankMenu : public Menu {
public:
    BankMenu(const sf::Vector2f& windowSize, int& crystals, int& storageCapacity, TimeSystem& timeSystem);

    void render(sf::RenderWindow& window) override;
    void update(float dt) override;
    void handleMouseClick(const sf::Vector2f& mousePos) override;
    void updateHover(const sf::Vector2f& mousePos) override;

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
    std::vector<std::unique_ptr<Button>> mButtons;

    std::unique_ptr<WithdrawMenu> mWithdrawMenu;
    std::unique_ptr<DepositMenu> mDepositMenu;
    std::unique_ptr<BorrowMenu> mBorrowMenu;

    int& mCrystals;
    int mBankBalance;

    bool mHasBorrowActive;
    TimeSystem& mTimeSystem;
};
