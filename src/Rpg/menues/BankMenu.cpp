#include "BankMenu.h"
#include <iostream>

BankMenu::BankMenu(sf::RenderWindow& window, int& crystals, int& storageCapacity, TimeSystem& timeSystem)
    : mCrystals(crystals), mBankBalance(0), mHasBorrowActive(false),
    mWithdrawButton(sf::Vector2f(1000.0f, 575.0f), sf::Vector2f(155.0f, 72.0f), "Withdraw"),
    mDepositButton(sf::Vector2f(1000.0f, 575.0f), sf::Vector2f(155.f, 72.0f), "Deposit"),
    mBorrowButton(sf::Vector2f(1000.0f, 575.0f), sf::Vector2f(155.f, 72.0f), "Borrow"),
    mTimeSystem(timeSystem) {

    if (!mFont.loadFromFile("assets/fonts/gameFont.ttf"))
        std::cerr << "Failed to load font for BankMenu!" << std::endl;

    mMenuShape.setSize(sf::Vector2f(window.getSize().x * 3.0f / 4.0f, window.getSize().y * 3.0f / 4.0f));
    mMenuShape.setFillColor(sf::Color(50, 50, 50, 255));
    mMenuShape.setPosition(sf::Vector2f((window.getSize().x - mMenuShape.getSize().x) / 2.0f,
                                        (window.getSize().y - mMenuShape.getSize().y) / 2.0f));

    mHoveredZoneShape.setSize(sf::Vector2f(window.getSize().x * 3.0f / 4.0f, 50));
    mHoveredZoneShape.setFillColor(sf::Color(10, 10, 10, 100));
    mHoveredZoneShape.setPosition(sf::Vector2f((window.getSize().x - mMenuShape.getSize().x) / 2.0f,
                                               (window.getSize().y - mMenuShape.getSize().y) / 2.0f));

    mTitle.setFillColor(sf::Color::White);
    mTitle.setFont(mFont);
    mTitle.setCharacterSize(20);
    mTitle.setString("Crystal Bank");
    mTitle.setPosition(sf::Vector2f(mMenuShape.getPosition().x + (mMenuShape.getSize().x - mTitle.getScale().x) / 2.2f,
                                    mMenuShape.getPosition().y + 15.f));

    sf::Vector2f buttonSize(155.0f, 72.0f);
    float gap = 50.0f;
    float startX = mMenuShape.getPosition().x + 50.f;
    float startY = mMenuShape.getPosition().y + (mMenuShape.getSize().y - 2 * buttonSize.x - gap) / 1.5f;

    mWithdrawButton.setPosition(sf::Vector2f(startX, startY));
    mDepositButton.setPosition(sf::Vector2f(startX, startY + buttonSize.y + gap));
    mBorrowButton.setPosition(sf::Vector2f(startX, startY + 2 * buttonSize.y + 2 * gap));

    mWithdrawButton.setCallback([&]() {
        switchToMenu("Withdraw");
    });

    mDepositButton.setCallback([&]() {
        switchToMenu("Deposit");
    });

    mBorrowButton.setCallback([&]() {
        switchToMenu("Borrow");
    });

    mButtons.push_back(mWithdrawButton);
    mButtons.push_back(mDepositButton);
    mButtons.push_back(mBorrowButton);

    mWithdrawMenu = std::make_unique<WithdrawMenu>(window, sf::Vector2f(mMenuShape.getPosition().x +
           buttonSize.x + 125, mMenuShape.getPosition().y + 87.5f), sf::Vector2f(mMenuShape.getSize().x / 1.5f,
           mMenuShape.getSize().y - 150), crystals, mBankBalance, storageCapacity);

    mDepositMenu = std::make_unique<DepositMenu>(window, sf::Vector2f(mMenuShape.getPosition().x +
            buttonSize.x + 125, mMenuShape.getPosition().y + 87.5f), sf::Vector2f(mMenuShape.getSize().x / 1.5f,
            mMenuShape.getSize().y - 150), crystals, mBankBalance, storageCapacity);

    mBorrowMenu = std::make_unique<BorrowMenu>(window, sf::Vector2f(mMenuShape.getPosition().x +
            buttonSize.x + 125, mMenuShape.getPosition().y + 87.5f), sf::Vector2f(mMenuShape.getSize().x / 1.5f,
            mMenuShape.getSize().y - 150), crystals, mHasBorrowActive, timeSystem);
}

void BankMenu::render(sf::RenderWindow& window) {
    window.draw(mMenuShape);
    window.draw(mHoveredZoneShape);
    for (auto& button : mButtons)
        button.render(window);
    window.draw(mTitle);
    if (mCurrentMenu == "Withdraw")
        mWithdrawMenu->render(window);
    else if (mCurrentMenu == "Deposit")
        mDepositMenu->render(window);
    else if (mCurrentMenu == "Borrow")
        mBorrowMenu->render(window);
}

void BankMenu::update() {
    if (mCurrentMenu == "Withdraw")
        mWithdrawMenu->update();
    else if (mCurrentMenu == "Deposit")
        mDepositMenu->update();
    else if (mCurrentMenu == "Borrow")
        mBorrowMenu->update();
}

void BankMenu::handleMouseClick(const sf::Vector2f& mousePos) {
    for (auto& button : mButtons) {
        if (button.isMouseOver(mousePos))
            button.onClick();
    }

    if (mCurrentMenu == "Withdraw")
        mWithdrawMenu->handleClicks(mousePos);
    else if (mCurrentMenu == "Deposit")
        mDepositMenu->handleClicks(mousePos);
    else if (mCurrentMenu == "Borrow")
        mBorrowMenu->handleClicks(mousePos);
}

void BankMenu::updateHover(const sf::Vector2f& mousePos) {
    for (auto& button : mButtons)
        button.updateHover(mousePos);

    if (mCurrentMenu == "Withdraw")
        mWithdrawMenu->updateHover(mousePos);
    else if (mCurrentMenu == "Deposit")
        mDepositMenu->updateHover(mousePos);
    else if (mCurrentMenu == "Borrow")
        mBorrowMenu->updateHover(mousePos);
}

void BankMenu::switchToMenu(const std::string& menuName) {
    mCurrentMenu = menuName;
}

int BankMenu::getBankBalance() {
    return mBankBalance;
}

void BankMenu::setBankBalance(int bankBalance) {
    mBankBalance = bankBalance;
}

bool BankMenu::hasActiveBorrow() {
    return mBorrowMenu->getActiveBorrow();
}

void BankMenu::restart() {
    mWithdrawMenu->restart();
    mDepositMenu->restart();
    mBorrowMenu->restart();
}

int BankMenu::getLoanAmount() {
    return mBorrowMenu->getAmounToRepay();
}

void BankMenu::resetBorrowStats() {
    mBorrowMenu->reset();
}

void BankMenu::setBorrowStats(bool hasBorrowActive, int penalty, int interest, int amountToRepay, int daysToRepayment,
                              int startYear, int startDay, int startHour, int startMinute) {
    mBorrowMenu->setStats(hasBorrowActive, penalty, interest, amountToRepay,
                          daysToRepayment, startYear, startDay, startHour, startMinute);
}

int BankMenu::getPenalty() {
    return mBorrowMenu->getPenalty();
}

int BankMenu::getDaysToRepayment() {
    return mBorrowMenu->getDaysToRepayment();
}

int BankMenu::getInterest() {
    return mBorrowMenu->getInterest();
}

int BankMenu::getAmounToRepay() {
    return mBorrowMenu->getAmounToRepay();
}

int BankMenu::getStartYear() {
    return mBorrowMenu->getStartYear();
}

int BankMenu::getStartDay() {
    return mBorrowMenu->getStartDay();
}

int BankMenu::getStartHour() {
    return mBorrowMenu->getStartHour();
}

int BankMenu::getStartMinute() {
    return mBorrowMenu->getStartMinute();
}

