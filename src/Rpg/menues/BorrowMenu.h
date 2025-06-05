#pragma once
#include <SFML/Graphics.hpp>
#include "../../TowerDefense/menues/Button.h"
#include "../../TimeSystem.h"

class BorrowMenu
{
public:
	BorrowMenu(sf::RenderWindow& window, const sf::Vector2f position,
		const sf::Vector2f size, int& crystals, bool& hasBorrowActive,
		TimeSystem& timeSystem);
	void render(sf::RenderWindow& window);
	void handleClicks(const sf::Vector2f& mousePos);
	void updateHover(const sf::Vector2f& mousePos);
	void update();

	int getPenalty();
	int getDaysToRepayment();
	int getInterest();
	int getAmounToRepay();
	int getStartYear();
	int getStartDay();
	int getStartHour();
	int getStartMinute();

	bool getActiveBorrow();

	void restart();
	void reset();
	void setStats(bool hasBorrowActive, int penalty, int interest, int amountToRepay, int daysToRepayment, 
                  int startYear, int startDay, int startHour, int startMinute);

private:
	sf::RectangleShape mMenuShape;
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
	sf::Font mFont;

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

