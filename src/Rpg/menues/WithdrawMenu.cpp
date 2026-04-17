#include "WithdrawMenu.h"
#include <iostream>

WithdrawMenu::WithdrawMenu(const sf::Vector2f& windowSize, const sf::Vector2f& position,
                           const sf::Vector2f& size, int& crystals, int& bankBalance, int& storageCapacity)
    : Menu(size, position, true),
    m100Button(sf::Vector2f(0, 0), sf::Vector2f(80.0f, 35.0f), "100"),
    m250Button(sf::Vector2f(0, 0), sf::Vector2f(80.0f, 35.0f), "250"),
    m500Button(sf::Vector2f(0, 0), sf::Vector2f(80.0f, 35.0f), "500"),
    mConfirmButton(sf::Vector2f(0, 0), sf::Vector2f(200.0f, 40.0f), "Confirm Withdrawal"),
    mBackButton(sf::Vector2f(0, 0), sf::Vector2f(100.0f, 40.0f), "Back"),
    mConfirmShowing(false), mAmountToWithdraw(0), mCrystals(crystals),
    mBankBalance(bankBalance), mStorageCapacity(storageCapacity) {

    if (!mFont.loadFromFile("assets/fonts/gameFont.ttf"))
        std::cerr << "Failed to load font for BankMenu!" << std::endl;

    mCrystalBalance = createMessageText(mFont, "Crystals: " + std::to_string(crystals),
                                        sf::Vector2f(mMenuShape.getPosition().x + 60.f, mMenuShape.getPosition().y + 175.f));
    mBalance = createMessageText(mFont, "Bank Balance: "  + std::to_string(bankBalance),
                                 sf::Vector2f(mMenuShape.getPosition().x + 60.f, mMenuShape.getPosition().y + 100.f));
    mAmount = createMessageText(mFont, "Withdraw Amount: ",
                                sf::Vector2f(mMenuShape.getPosition().x + 60.f, mMenuShape.getPosition().y + 25.f));
    mInsufficientFundsText = createMessageText(mFont, "Insufficient funds in bank",
                                               sf::Vector2f(mMenuShape.getPosition().x + 60.f, mMenuShape.getPosition().y + 310.f));

    m100Button.setPosition(sf::Vector2f(mMenuShape.getPosition().x + 300.f,
                                        mMenuShape.getPosition().y + 25.f - 8.75f));
    m250Button.setPosition(sf::Vector2f(mMenuShape.getPosition().x + 300.f + 110.f,
                                        mMenuShape.getPosition().y + 25.f - 8.75f));
    m500Button.setPosition(sf::Vector2f(mMenuShape.getPosition().x + 300.f + 220.f,
                                        mMenuShape.getPosition().y + 25.f - 8.75f));
    mConfirmButton.setPosition(sf::Vector2f(mMenuShape.getPosition().x + 75.f, mMenuShape.getPosition().y + 310.f));
    mBackButton.setPosition(sf::Vector2f(mMenuShape.getPosition().x + 450.f, mMenuShape.getPosition().y + 310.f));

    m100Button.setCallback([this]() {
        withdrawAmount(100);
        updateTexts();
    });

    m250Button.setCallback([this]() {
        withdrawAmount(250);
        updateTexts();
    });

    m500Button.setCallback([this]() {
        withdrawAmount(500);
        updateTexts();
    });

    mConfirmButton.setCallback([this]() {
        mBankBalance -= mAmountToWithdraw;
        mCrystals += mAmountToWithdraw;
        mAmountToWithdraw = 0;
        mConfirmShowing = false;

        updateTexts();
    });

    mBackButton.setCallback([this]() {
        mAmountToWithdraw = 0;
        mConfirmShowing = false;
        mInsufficientFunds = false;

        updateTexts();
    });

    updateTexts();
}

void WithdrawMenu::render(sf::RenderWindow& window) {
    if (!mIsActive)
        return;

    window.draw(mMenuShape);

    window.draw(mBalance);
    window.draw(mAmount);
    window.draw(mCrystalBalance);

    m100Button.render(window);
    m250Button.render(window);
    m500Button.render(window);

    if (mConfirmShowing) {
        mConfirmButton.render(window);
        mBackButton.render(window);
    }

    if (mInsufficientFunds)
        window.draw(mInsufficientFundsText);
}

void WithdrawMenu::handleMouseClick(const sf::Vector2f& mousePos) {
    if (!mIsActive)
        return;

    if (m100Button.isMouseOver(mousePos))
        m100Button.onClick();
    if (m250Button.isMouseOver(mousePos))
        m250Button.onClick();
    if (m500Button.isMouseOver(mousePos))
        m500Button.onClick();

    if (mConfirmShowing) {
        if (mConfirmButton.isMouseOver(mousePos))
            mConfirmButton.onClick();
        if (mBackButton.isMouseOver(mousePos))
            mBackButton.onClick();
    }
}

void WithdrawMenu::updateHover(const sf::Vector2f& mousePos) {
    if (!mIsActive)
        return;

    m100Button.updateHover(mousePos);
    m250Button.updateHover(mousePos);
    m500Button.updateHover(mousePos);

    if (mConfirmShowing) {
        mConfirmButton.updateHover(mousePos);
        mBackButton.updateHover(mousePos);
    }
}

void WithdrawMenu::update(float dt) {
    if (!mIsActive)
        return;
}

void WithdrawMenu::restart() {
    mConfirmShowing = false;
    mInsufficientFunds = false;
}

void WithdrawMenu::withdrawAmount(int amount) {
    if (mBankBalance >= amount) {
            mAmountToWithdraw = amount;
            mConfirmShowing = true; // show button to confirm amount to be withdrawn
            mInsufficientFunds = false;
        } else {
            mAmountToWithdraw = 0;
            mConfirmShowing = false;
            mInsufficientFunds = true; // show low funds error message
        }
}

void WithdrawMenu::updateTexts() {
    if (mConfirmShowing) {
        mCrystalBalance.setString("Crystals: " + std::to_string(mCrystals) + " + " +
            std::to_string(mAmountToWithdraw));
        mBalance.setString("Bank Balance: " + std::to_string(mBankBalance) + " - " +
            std::to_string(mAmountToWithdraw) + " / " + std::to_string(mStorageCapacity));
    } else {
        mCrystalBalance.setString("Crystals: " + std::to_string(mCrystals));
        mBalance.setString("Bank Balance: " + std::to_string(mBankBalance) + " / " +
        std::to_string(mStorageCapacity));
    }
}
