#pragma once
#include <SFML/Graphics.hpp>
#include "../../ui/Menu.h"
#include "../../ui/Button.h"

class DepositMenu : public Menu {
public:
    DepositMenu(const sf::Vector2f& windowSize, const sf::Vector2f& position,
                const sf::Vector2f& size, int& crystals, int& bankBalances, int& storageCapacity);

    void render(sf::RenderWindow& window) override;
    void handleMouseClick(const sf::Vector2f& mousePos) override;
    void updateHover(const sf::Vector2f& mousePos) override;
    void update(float dt) override;

    void updateTexts();
    void restart();

private:
    // called when deposit buttons are clicked
    void depositAmount(int amount);

    Button m100Button;
    Button m250Button;
    Button m500Button;
    Button mConfirmButton;
    Button mBackButton;

    sf::Text mBalance;
    sf::Text mCrystalBalance;
    sf::Text mAmount;
    sf::Text mCanDepositOnlyText;

    bool mConfirmShowing;
    bool mStorageWillBeFull;
    int mAmountToDeposit;
    int& mCrystals;
    int& mBankBalance;
    int& mStorageCapacity;

    bool mStorageAlreadyFull;
    sf::Text mStorageFullText;

    bool mInsufficientFunds;
    sf::Text mInsufficientFundsText;
};
