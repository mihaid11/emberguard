#pragma once
#include <SFML/Graphics.hpp>
#include "../../ui/Menu.h"
#include "../../ui/Button.h"
#include "../../core/TimeSystem.h"

class BorrowMenu : public Menu {
public:
    BorrowMenu(const sf::Vector2f& windowSize, const sf::Vector2f& position,
               const sf::Vector2f& size, int& crystals, bool& hasBorrowActive, TimeSystem& timeSystem);

    void render(sf::RenderWindow& window) override;
    void handleMouseClick(const sf::Vector2f& mousePos) override;
    void updateHover(const sf::Vector2f& mousePos) override;
    void update(float dt) override;

    int getPenalty();
    int getDaysToRepayment();
    int getInterest();
    int getAmounToRepay();
    int getStartYear();
    int getStartDay();
    int getStartHour();
    int getStartMinute();

    bool getActiveBorrow();

    void updateTexts();
    void restart();
    void reset();
    void setStats(bool hasBorrowActive, int penalty, int interest, int amountToRepay, int daysToRepayment,
                  int startYear, int startDay, int startHour, int startMinute);

private:
    Button m500Button;
    Button m1000Button;
    Button m2000Button;
    Button mConfirmButton;
    Button mBackButton;

    sf::Text mLoanAmount;
    sf::Text mRepayment;
    sf::Text mInterestText;
    sf::Text mTotalRepay;
    sf::Text mPenaltyText;
    sf::Text mTimerText;

    bool mConfirmShowing;
    int mAmountToBorrow;
    int mAmountToRepay;
    int mInterest;
    int mDaysToRepayment;
    int mRepaymentDay;
    int mPenalty;
    int& mCrystals;

    bool& mHasBorrowActive;
    sf::Text mBorrowActiveText;
    int mStartYear;
    int mStartDay;
    int mStartHour;
    int mStartMinute;
    TimeSystem& mTimeSystem;
};
