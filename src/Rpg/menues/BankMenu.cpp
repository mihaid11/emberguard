#include "BankMenu.h"
#include <iostream>

BankMenu::BankMenu(const sf::Vector2f& windowSize, int& crystals, int& storageCapacity, TimeSystem& timeSystem)
    : Menu(windowSize, sf::Vector2f(0.75f, 0.75f)), mCrystals(crystals), mBankBalance(0), mHasBorrowActive(false),
    mWithdrawButton(sf::Vector2f(1000.0f, 575.0f), sf::Vector2f(155.0f, 72.0f), "Withdraw"),
    mDepositButton(sf::Vector2f(1000.0f, 575.0f), sf::Vector2f(155.f, 72.0f), "Deposit"),
    mBorrowButton(sf::Vector2f(1000.0f, 575.0f), sf::Vector2f(155.f, 72.0f), "Borrow"),
    mTimeSystem(timeSystem) {

    if (!mFont.loadFromFile("assets/fonts/gameFont.ttf"))
        std::cerr << "Failed to load font for BankMenu!" << std::endl;

    mTitle.setFillColor(sf::Color::White);
    mTitle.setFont(mFont);
    mTitle.setCharacterSize(20);
    mTitle.setString("Crystal Bank");

     mTitle.setOrigin(mTitle.getLocalBounds().left + mTitle.getLocalBounds().width / 2.f,
                     mTitle.getLocalBounds().top + mTitle.getLocalBounds().height / 2.f);
     mTitle.setPosition(sf::Vector2f(mHoveredZoneShape.getPosition().x + mHoveredZoneShape.getSize().x / 2.f,
                                    mHoveredZoneShape.getPosition().y + mHoveredZoneShape.getSize().y / 2.f));

    sf::Vector2f buttonSize(155.0f, 72.0f);
    float gap = 50.0f;
    float startX = mMenuShape.getPosition().x + 50.f;
    float startY = mMenuShape.getPosition().y + (mMenuShape.getSize().y - 2 * buttonSize.x - gap) / 1.5f;

    mWithdrawButton.setPosition(sf::Vector2f(startX, startY));
    mDepositButton.setPosition(sf::Vector2f(startX, startY + buttonSize.y + gap));
    mBorrowButton.setPosition(sf::Vector2f(startX, startY + 2 * buttonSize.y + 2 * gap));

    mWithdrawMenu = std::make_unique<WithdrawMenu>(windowSize, sf::Vector2f(mMenuShape.getPosition().x +
           buttonSize.x + 125.f, mMenuShape.getPosition().y + 87.5f), sf::Vector2f(mMenuShape.getSize().x / 1.5f,
           mMenuShape.getSize().y - 150.f), crystals, mBankBalance, storageCapacity);

    mDepositMenu = std::make_unique<DepositMenu>(windowSize, sf::Vector2f(mMenuShape.getPosition().x +
            buttonSize.x + 125.f, mMenuShape.getPosition().y + 87.5f), sf::Vector2f(mMenuShape.getSize().x / 1.5f,
            mMenuShape.getSize().y - 150.f), crystals, mBankBalance, storageCapacity);

    mBorrowMenu = std::make_unique<BorrowMenu>(windowSize, sf::Vector2f(mMenuShape.getPosition().x +
            buttonSize.x + 125.f, mMenuShape.getPosition().y + 87.5f), sf::Vector2f(mMenuShape.getSize().x / 1.5f,
            mMenuShape.getSize().y - 150.f), crystals, mHasBorrowActive, timeSystem);

    mWithdrawButton.setCallback([&]() {
        mWithdrawMenu->setActive(true);
        mDepositMenu->setActive(false);
        mBorrowMenu->setActive(false);

        mWithdrawMenu->updateTexts();
    });

    mDepositButton.setCallback([&]() {
        mWithdrawMenu->setActive(false);
        mDepositMenu->setActive(true);
        mBorrowMenu->setActive(false);

        mDepositMenu->updateTexts();
    });

    mBorrowButton.setCallback([&]() {
        mWithdrawMenu->setActive(false);
        mDepositMenu->setActive(false);
        mBorrowMenu->setActive(true);

        mBorrowMenu->updateTexts();
    });

    mButtons.push_back(&mWithdrawButton);
    mButtons.push_back(&mDepositButton);
    mButtons.push_back(&mBorrowButton);
}

void BankMenu::render(sf::RenderWindow& window) {
    if (!mIsActive)
        return;

    Menu::render(window);
    window.draw(mTitle);

    for (auto& button : mButtons)
        button->render(window);

    mWithdrawMenu->render(window);
    mDepositMenu->render(window);
    mBorrowMenu->render(window);
}

void BankMenu::update(float dt) {
    if (!mIsActive)
        return;

    mWithdrawMenu->update(dt);
    mDepositMenu->update(dt);
    mBorrowMenu->update(dt);
}

void BankMenu::handleMouseClick(const sf::Vector2f& mousePos) {
    if (!mIsActive)
        return;

    for (auto& button : mButtons) {
        if (button->isMouseOver(mousePos))
            button->onClick();
    }

    mWithdrawMenu->handleMouseClick(mousePos);
    mDepositMenu->handleMouseClick(mousePos);
    mBorrowMenu->handleMouseClick(mousePos);
}

void BankMenu::updateHover(const sf::Vector2f& mousePos) {
    if (!mIsActive)
        return;

    for (auto& button : mButtons)
        button->updateHover(mousePos);

    mWithdrawMenu->updateHover(mousePos);
    mDepositMenu->updateHover(mousePos);
    mBorrowMenu->updateHover(mousePos);
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
