#include "BorrowMenu.h"
#include <iostream>
#include <sstream>

BorrowMenu::BorrowMenu(sf::RenderWindow& window, const sf::Vector2f position,
                       const sf::Vector2f size, int& crystals, bool& hasBorrowActive, TimeSystem& timeSystem)
    :m500Button(sf::Vector2f(0, 0), sf::Vector2f(80.0f, 35.0f), "500"),
    m1000Button(sf::Vector2f(0, 0), sf::Vector2f(80.0f, 35.0f), "1000"),
    m2000Button(sf::Vector2f(0, 0), sf::Vector2f(80.0f, 35.0f), "2000"),
    mConfirmButton(sf::Vector2f(0, 0), sf::Vector2f(200.0f, 40.0f), "Confirm Loan"),
    mBackButton(sf::Vector2f(0, 0), sf::Vector2f(100.0f, 40.0f), "Back"),
    mConfirmShowing(false), mAmountToBorrow(0), mCrystals(crystals), mAmountToRepay(0),
    mInterest(0), mDaysToRepayment(0), mPenalty(0), mHasBorrowActive(hasBorrowActive),
    mRepaymentDay(0), mStartYear(0), mStartDay(0), mStartHour(0), mStartMinute(0), mTimeSystem(timeSystem) {

    if (!mFont.loadFromFile("assets/fonts/gameFont.ttf"))
        std::cerr << "Failed to load font for BankMenu!" << std::endl;

    mMenuShape.setSize(size);
    mMenuShape.setPosition(position);
    mMenuShape.setFillColor(sf::Color(70, 70, 70, 200));

    mLoanAmount.setFont(mFont);
    mLoanAmount.setCharacterSize(16);
    mLoanAmount.setFillColor(sf::Color::White);
    mLoanAmount.setString("Loan Amount: ");
    mLoanAmount.setPosition(sf::Vector2f(mMenuShape.getPosition().x + 60.f, mMenuShape.getPosition().y + 25.f));

    mRepayment.setFont(mFont);
    mRepayment.setCharacterSize(16);
    mRepayment.setFillColor(sf::Color::White);
    mRepayment.setPosition(sf::Vector2f(mMenuShape.getPosition().x + 60.f, mMenuShape.getPosition().y + 75.f));

    mInterestText.setFont(mFont);
    mInterestText.setCharacterSize(16);
    mInterestText.setFillColor(sf::Color::White);
    mInterestText.setPosition(sf::Vector2f(mMenuShape.getPosition().x + 60.f, mMenuShape.getPosition().y + 125.f));

    mTotalRepay.setFont(mFont);
    mTotalRepay.setCharacterSize(16);
    mTotalRepay.setFillColor(sf::Color::White);
    mTotalRepay.setPosition(sf::Vector2f(mMenuShape.getPosition().x + 60.f, mMenuShape.getPosition().y + 175.f));

    mPenaltyText.setFont(mFont);
    mPenaltyText.setCharacterSize(16);
    mPenaltyText.setFillColor(sf::Color::White);
    mPenaltyText.setPosition(sf::Vector2f(mMenuShape.getPosition().x + 60.f, mMenuShape.getPosition().y + 250.f));

    mBorrowActiveText.setFont(mFont);
    mBorrowActiveText.setCharacterSize(16);
    mBorrowActiveText.setFillColor(sf::Color::White);
    mBorrowActiveText.setString("You can't have more than one borrow active!");
    mBorrowActiveText.setPosition(sf::Vector2f(mMenuShape.getPosition().x + 60.f, mMenuShape.getPosition().y + 25.f));

    m500Button.setPosition(sf::Vector2f(mMenuShape.getPosition().x + 300.f,
                                        mMenuShape.getPosition().y + 25.f - 8.75f));
    m1000Button.setPosition(sf::Vector2f(mMenuShape.getPosition().x + 300.f + 110.f,
                                         mMenuShape.getPosition().y + 25.f - 8.75f));
    m2000Button.setPosition(sf::Vector2f(mMenuShape.getPosition().x + 300.f + 220.f,
                                         mMenuShape.getPosition().y + 25.f - 8.75f));
    mConfirmButton.setPosition(sf::Vector2f(mMenuShape.getPosition().x + 100.f, mMenuShape.getPosition().y + 310.f));
    mBackButton.setPosition(sf::Vector2f(mMenuShape.getPosition().x + 400.f, mMenuShape.getPosition().y + 310.f));

    m500Button.setCallback([&]() {
        if (hasBorrowActive == false) {
            mAmountToBorrow = 500;
            mConfirmShowing = true;
            mInterest = 30;
            mDaysToRepayment = 3;
            mPenalty = 20;
            mAmountToRepay = mAmountToBorrow * (100 + mInterest) / 100;
            mRepaymentDay = mDaysToRepayment + timeSystem.getDay();
        }
    });

    m1000Button.setCallback([&]() {
        if (hasBorrowActive == false) {
            mAmountToBorrow = 1000;
            mConfirmShowing = true;
            mInterest = 25;
            mDaysToRepayment = 5;
            mPenalty = 15;
            mAmountToRepay = mAmountToBorrow * (100 + mInterest) / 100;
            mRepaymentDay = mDaysToRepayment + timeSystem.getDay();
        }
    });

    m2000Button.setCallback([&]() {
        if (hasBorrowActive == false) {
            mAmountToBorrow = 2000;
            mConfirmShowing = true;
            mInterest = 20;
            mDaysToRepayment = 7;
            mPenalty = 10;
            mAmountToRepay = mAmountToBorrow * (100 + mInterest) / 100;
            mRepaymentDay = mDaysToRepayment + timeSystem.getDay();
        }
    });

    mConfirmButton.setCallback([&]() {
        crystals += mAmountToBorrow;
        mAmountToBorrow = 0;
        mConfirmShowing = false;
        mHasBorrowActive = true;
        mStartYear = mTimeSystem.getYear();
        mStartDay = mTimeSystem.getDay();
        mStartHour = mTimeSystem.getHour();
        mStartMinute = mTimeSystem.getMinute();
    });

    mBackButton.setCallback([&]() {
        mAmountToBorrow = 0;
        mInterest = 0;
        mDaysToRepayment = 0;
        mPenalty = 0;
        mAmountToRepay = 0;
        mRepaymentDay = 0;
        mConfirmShowing = false;
    });
}

void BorrowMenu::render(sf::RenderWindow& window) {
    window.draw(mMenuShape);
    window.draw(mRepayment);
    window.draw(mInterestText);
    window.draw(mTotalRepay);
    window.draw(mPenaltyText);

    if (mHasBorrowActive == false) {
        window.draw(mLoanAmount);
        m500Button.render(window);
        m1000Button.render(window);
        m2000Button.render(window);
    } else {
        window.draw(mBorrowActiveText);
        int currentYear = mTimeSystem.getYear();
        int currentDay = mTimeSystem.getDay();
        int currentHour = mTimeSystem.getHour();
        int currentMinute = mTimeSystem.getMinute();

        int elapsedYears, elapsedDays, elapsedHours, elapsedMinutes;
        int hourDecrement = 0;
        int dayDecrement = 0;
        int yearDecrement = 0;

        if (currentMinute >= mStartMinute) {
            elapsedMinutes = currentMinute - mStartMinute;
        } else {
            elapsedMinutes = 60 + currentMinute - mStartMinute;
            hourDecrement = 1;
        }

        if (currentHour >= mStartHour) {
            elapsedHours = currentHour - mStartHour - hourDecrement;
        } else {
            elapsedHours = 24 + currentHour - mStartHour - hourDecrement;
            dayDecrement = 1;
        }

        if (currentDay >= mStartDay) {
            elapsedDays = currentDay - mStartDay - dayDecrement;
        } else {
            elapsedDays = 365 + currentDay - mStartDay - dayDecrement;
            yearDecrement = 1;
        }

        elapsedYears = currentYear - mStartYear - yearDecrement;

        int remainingHours = 23 - elapsedHours;
        int remainingMinutes = 59 - elapsedMinutes;
        int remainingDays = mDaysToRepayment - elapsedDays - 1;

        std::ostringstream timerText;
        timerText << "Remaining: " << remainingDays << "d " << remainingHours << "h " << remainingMinutes << "m";

        sf::Text timerDisplay;
        timerDisplay.setFont(mFont);
        timerDisplay.setCharacterSize(16);
        timerDisplay.setFillColor(sf::Color::White);
        timerDisplay.setString(timerText.str());
        timerDisplay.setPosition(sf::Vector2f(mMenuShape.getPosition().x + mMenuShape.getSize().x * 2.f / 3 - 65.f,
            mMenuShape.getPosition().y + mMenuShape.getSize().y * 2.f / 3 - 170.f));

        window.draw(timerDisplay);
    }

    if (mConfirmShowing) {
        mConfirmButton.render(window);
        mBackButton.render(window);
    }
}

void BorrowMenu::handleClicks(const sf::Vector2f& mousePos) {
    if (m500Button.isMouseOver(mousePos))
        m500Button.onClick();
    if (m1000Button.isMouseOver(mousePos))
        m1000Button.onClick();
    if (m2000Button.isMouseOver(mousePos))
        m2000Button.onClick();

    if (mConfirmShowing) {
        if (mConfirmButton.isMouseOver(mousePos))
            mConfirmButton.onClick();
        if (mBackButton.isMouseOver(mousePos))
            mBackButton.onClick();
    }
}

void BorrowMenu::updateHover(const sf::Vector2f& mousePos) {
    m500Button.updateHover(mousePos);
    m1000Button.updateHover(mousePos);
    m2000Button.updateHover(mousePos);

    if (mConfirmShowing) {
        mConfirmButton.updateHover(mousePos);
        mBackButton.updateHover(mousePos);
    }
}

void BorrowMenu::update() {
    if (mConfirmShowing) {
        mRepayment.setString("Repayment by: Day " + std::to_string(mRepaymentDay));
        mInterestText.setString("Interest Rate: " + std::to_string(mInterest) + "%");
        mTotalRepay.setString("Total to Repay: " + std::to_string(mAmountToRepay));
        mPenaltyText.setString("Penalty: +" + std::to_string(mPenalty) + "% per late day");
    } else {
        mRepayment.setString("");
        mInterestText.setString("");
        mTotalRepay.setString("");
        mPenaltyText.setString("");
    }

    if (mHasBorrowActive) {
        int currentYear = mTimeSystem.getYear();
        int currentDay = mTimeSystem.getDay();
        int currentHour = mTimeSystem.getHour();
        int currentMinute = mTimeSystem.getMinute();

        int elapsedDays, elapsedHours, elapsedMinutes;
        int hourDecrement = 0;
        int dayDecrement = 0;
        int yearDecrement = 0;

        if (currentMinute >= mStartMinute) {
            elapsedMinutes = currentMinute - mStartMinute;
        } else {
            elapsedMinutes = 60 + currentMinute - mStartMinute;
            hourDecrement = 1;
        }

        if (currentHour >= mStartHour) {
            elapsedHours = currentHour - mStartHour - hourDecrement;
        } else {
            elapsedHours = 24 + currentHour - mStartHour - hourDecrement;
            dayDecrement = 1;
        }

        if (currentDay >= mStartDay) {
            elapsedDays = currentDay - mStartDay - dayDecrement;
        } else {
            elapsedDays = 365 + currentDay - mStartDay - dayDecrement;
            yearDecrement = 1;
        }

        if (elapsedDays == mDaysToRepayment) {
            reset();
            mCrystals -= mAmountToRepay;
        }
    }
}

int BorrowMenu::getPenalty() {
    return mPenalty;
}

int BorrowMenu::getDaysToRepayment() {
    return mDaysToRepayment;
}

int BorrowMenu::getInterest() {
    return mInterest;
}

int BorrowMenu::getAmounToRepay() {
    return mAmountToRepay;
}

int BorrowMenu::getStartYear() {
    return mStartYear;
}

int BorrowMenu::getStartDay() {
    return mStartDay;
}

int BorrowMenu::getStartHour() {
    return mStartHour;
}

int BorrowMenu::getStartMinute() {
    return mStartMinute;
}

bool BorrowMenu::getActiveBorrow() {
    return mHasBorrowActive;
}

void BorrowMenu::restart() {
    mConfirmShowing = false;
}

void BorrowMenu::reset() {
    mAmountToBorrow = 0;
    mAmountToRepay = 0;
    mInterest = 0;
    mDaysToRepayment = 0;
    mRepaymentDay = 0;
    mPenalty = 0;
    mStartYear = 0;
    mStartDay = 0;
    mStartHour = 0;
    mStartMinute = 0;
    mStartDay = 0;
    mHasBorrowActive = false;
}

void BorrowMenu::setStats(bool hasBorrowActive, int penalty, int interest, int amountToRepay, int daysToRepayment,
                          int startYear, int startDay, int startHour, int startMinute) {
    mHasBorrowActive = hasBorrowActive;
    mPenalty = penalty;
    mInterest = interest;
    mAmountToRepay = amountToRepay;
    mDaysToRepayment = daysToRepayment;
    mRepaymentDay = startDay + mDaysToRepayment;
    mStartYear = startYear;
    mStartDay = startDay;
    mStartHour = startHour;
    mStartMinute = startMinute;
}

