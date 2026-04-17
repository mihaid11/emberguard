#include "DepositMenu.h"
#include <iostream>

DepositMenu::DepositMenu(const sf::Vector2f& windowSize, const sf::Vector2f& position, const sf::Vector2f& size,
                         int& crystals, int& bankBalance, int& storageCapacity)
    : Menu(size, position, true),
    m100Button(sf::Vector2f(0, 0), sf::Vector2f(80.0f, 35.0f), "100"),
    m250Button(sf::Vector2f(0, 0), sf::Vector2f(80.0f, 35.0f), "250"),
    m500Button(sf::Vector2f(0, 0), sf::Vector2f(80.0f, 35.0f), "500"),
    mConfirmButton(sf::Vector2f(0, 0), sf::Vector2f(200.0f, 40.0f), "Confirm Deposit"),
    mBackButton(sf::Vector2f(0, 0), sf::Vector2f(100.0f, 40.0f), "Back"),
    mConfirmShowing(false), mAmountToDeposit(0), mCrystals(crystals),
    mBankBalance(bankBalance), mStorageWillBeFull(false), mStorageCapacity(storageCapacity),
    mStorageAlreadyFull(false) {

    if (!mFont.loadFromFile("assets/fonts/gameFont.ttf"))
        std::cerr << "Failed to load font for BankMenu!" << std::endl;

    mCrystalBalance = createMessageText(mFont, ("Crystals: " + std::to_string(crystals)),
                                        sf::Vector2f(mMenuShape.getPosition().x + 60.f, mMenuShape.getPosition().y + 175.f));
    mBalance = createMessageText(mFont, ("Bank Balance: " + std::to_string(bankBalance)),
                                 sf::Vector2f(mMenuShape.getPosition().x + 60.f, mMenuShape.getPosition().y + 100.f));
    mAmount = createMessageText(mFont, ("Deposit Amount: "),
                                sf::Vector2f(mMenuShape.getPosition().x + 60.f, mMenuShape.getPosition().y + 25.f));
    mStorageFullText = createMessageText(mFont, "You can't deposit anymore. Storage Capacity is full!",
                                         sf::Vector2f(mMenuShape.getPosition().x + 60.f, mMenuShape.getPosition().y + 310.f));
    mCanDepositOnlyText = createMessageText(mFont, "Storage capacity is limited!",
                                            sf::Vector2f(mMenuShape.getPosition().x + 60.f, mMenuShape.getPosition().y + 250.f));
    mInsufficientFundsText = createMessageText(mFont, "Insufficient funds to fill bank as requested",
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
        depositAmount(100);
        updateTexts();
    });

    m250Button.setCallback([this]() {
        depositAmount(250);
        updateTexts();
    });

    m500Button.setCallback([this]() {
        depositAmount(500);
        updateTexts();
    });

    mConfirmButton.setCallback([this]() {
        if (mStorageWillBeFull == false) {
            mBankBalance += mAmountToDeposit;
            mCrystals -= mAmountToDeposit;
        } else {
            mCrystals -= mStorageCapacity - mBankBalance;
            mBankBalance = mStorageCapacity;
        }
        mAmountToDeposit = 0;
        mConfirmShowing = false;

        updateTexts();
    });

    mBackButton.setCallback([this]() {
        mAmountToDeposit = 0;
        mConfirmShowing = false;
        mInsufficientFunds = false;

        updateTexts();
    });

    updateTexts();
}

void DepositMenu::render(sf::RenderWindow& window) {
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
        if (mStorageWillBeFull)
            window.draw(mCanDepositOnlyText);
    }

    if (mStorageAlreadyFull)
        window.draw(mStorageFullText);

    if (mStorageWillBeFull)
        window.draw(mCanDepositOnlyText);

    if (mInsufficientFunds)
        window.draw(mInsufficientFundsText);
}

void DepositMenu::handleMouseClick(const sf::Vector2f& mousePos) {
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

void DepositMenu::updateHover(const sf::Vector2f& mousePos) {
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

void DepositMenu::update(float dt) {
    if (!mIsActive)
        return;
}

void DepositMenu::restart() {
    mConfirmShowing = false;
    mInsufficientFunds = false;
}

void DepositMenu::depositAmount(int amount) {
    if (mCrystals >= amount) {
        if (mBankBalance == mStorageCapacity) {
            mStorageAlreadyFull = true;
            mInsufficientFunds = false;
        } else if (mBankBalance + amount > mStorageCapacity) {
            mStorageWillBeFull = true;
            mInsufficientFunds = false;
        } else {
            mAmountToDeposit = amount;
            mConfirmShowing = true;
            mInsufficientFunds = false;
            mStorageAlreadyFull = false;
            mStorageWillBeFull = false;
        }
    } else {
        mAmountToDeposit = 0;
        mConfirmShowing = false;
        mInsufficientFunds = true;
    }
}

void DepositMenu::updateTexts() {
    if (mConfirmShowing) {
        if (mStorageWillBeFull == true) {
            int amount = mStorageCapacity - mBankBalance;
            mBalance.setString("Bank Balance: " + std::to_string(mBankBalance) + " + " +
                std::to_string(amount) + " / " + std::to_string(mStorageCapacity));
            mCanDepositOnlyText.setString("Storage capacity is limited! You can deposit only " +
            std::to_string(amount) + " crystals");
            mCrystalBalance.setString("Crystals: " + std::to_string(mCrystals) + " - " +
                std::to_string(amount));
        } else {
            mBalance.setString("Bank Balance: " + std::to_string(mBankBalance) + " + " +
                std::to_string(mAmountToDeposit) + " / " + std::to_string(mStorageCapacity));
            mCrystalBalance.setString("Crystals: " + std::to_string(mCrystals) + " - " +
                std::to_string(mAmountToDeposit));
        }
    } else {
        mCrystalBalance.setString("Crystals: " + std::to_string(mCrystals));
        mBalance.setString("Bank Balance: " + std::to_string(mBankBalance) + " / " +
        std::to_string(mStorageCapacity));
    }
}
