#include "AnalyzeMenu.h"
#include <iostream>
#include <cmath>
#include <sstream>
#include <stdlib.h>
#include <time.h>
#include <algorithm>

AnalyzeMenu::AnalyzeMenu(sf::RenderWindow& window, Inventory& inventory, TimeSystem& timeSystem,
	std::vector<int>& availableTowers, const sf::Vector2f& slotSize, int& crystals)
	: mInventory(inventory), mTimeSystem(timeSystem), mInSlot(false), mExtracting(false), 
	mHoveredSlot(-1), mSlotItem(nullptr), mCompleted(false),
	mStartButton(sf::Vector2f(0.0f, 0.0f), sf::Vector2f(110.f, 35.f), "Start"),
	mCancelButton(sf::Vector2f(0.0f, 0.0f), sf::Vector2f(110.f, 35.f), "Cancel"),
	mCompleteButton(sf::Vector2f(0.0f, 0.0f), sf::Vector2f(110.f, 35.f), "Complete"),
	mTimerActive(false), mStartYear(0), mStartDay(0), mStartHour(0), mStartMinute(0)
{
	if (!mFont.loadFromFile("assets/fonts/gameFont.ttf")) {
		std::cerr << "Failed to load font for AnalyzeMenu!" << std::endl;
	}

	mMenuShape.setSize(sf::Vector2f(window.getSize().x / 2.0f, window.getSize().y / 2.0f));
	mMenuShape.setFillColor(sf::Color(50, 50, 50, 255));
	mMenuShape.setPosition((window.getSize().x - mMenuShape.getSize().x) / 2.f,
                           (window.getSize().y - mMenuShape.getSize().y) / 2.f);

	mHoveredZoneShape.setSize(sf::Vector2f(window.getSize().x * 3.0f / 4.0f, 40));
	mHoveredZoneShape.setFillColor(sf::Color(10, 10, 10, 100));
	mHoveredZoneShape.setPosition(sf::Vector2f((window.getSize().x - mMenuShape.getSize().x) / 2.0f,
		                                       (window.getSize().y - mMenuShape.getSize().y) / 2.0f));

	mMenuText.setFont(mFont);
	mMenuText.setCharacterSize(20);
	mMenuText.setFillColor(sf::Color::White);
	mMenuText.setString("Analyzer");
	mMenuText.setPosition(sf::Vector2f(mMenuShape.getPosition().x + mMenuShape.getSize().x / 2.f - 50.f,
                                       mMenuShape.getPosition().y + 8.f));

    mErrorText.setFillColor(sf::Color::White);
    mErrorText.setFont(mFont);
    mErrorText.setCharacterSize(18);
    mErrorText.setString("Only extract Tower Blueprint!");
    mErrorText.setPosition(sf::Vector2f(mMenuShape.getPosition().x + mMenuShape.getSize().x * 0.469f,
                                        mMenuShape.getPosition().y + mMenuShape.getSize().y * 0.657f));

	mStartButton.setPosition(sf::Vector2f(mMenuShape.getPosition().x + mMenuShape.getSize().x * 2.f / 3 - 47.f,
		                                  mMenuShape.getPosition().y + mMenuShape.getSize().y * 2.f / 3));
	mCancelButton.setPosition(sf::Vector2f(mMenuShape.getPosition().x + mMenuShape.getSize().x * 2.f / 3 - 47.f,
		                                   mMenuShape.getPosition().y + mMenuShape.getSize().y * 2.f / 3));
	mCompleteButton.setPosition(sf::Vector2f(mMenuShape.getPosition().x + mMenuShape.getSize().x * 2.f / 3 - 47.f,
		                                     mMenuShape.getPosition().y + mMenuShape.getSize().y * 2.f / 3));

	mStartButton.setCallback([&]() {
		mInSlot = false;
		mExtracting = true;
		mCompleted = false;
		mTimerActive = true;

		mStartYear = mTimeSystem.getYear();
		mStartDay = mTimeSystem.getDay();
		mStartHour = mTimeSystem.getHour();
		mStartMinute = mTimeSystem.getMinute();
        });

	mCancelButton.setCallback([&]() {
		mInSlot = true;
		mExtracting = false;
		mCompleted = false;
		mTimerActive = false;
		});

	mCompleteButton.setCallback([&]() {
        if (mSlotItem) {
            srand(static_cast<unsigned int>(time(NULL)));
            int border;
            if (mSlotItem->getId() == 2)
                border = 888;
            else if (mSlotItem->getId() == 3)
                border = 777;
            else if (mSlotItem->getId() == 4)
                border = 666;
            else if (mSlotItem->getId() == 5)
                border = 555;

            int itemChance = rand() % 1000;
			if (itemChance <= border) {
				if (std::find(availableTowers.begin(), availableTowers.end(), 1) == availableTowers.end()) {
					availableTowers.push_back(1);
				} else {
					std::cout << "TowerBlueprint already owned! -> Laser Tower" << std::endl;
					crystals += 25;
				}
			} else {
				if (std::find(availableTowers.begin(), availableTowers.end(), 2) == availableTowers.end()) {
					availableTowers.push_back(2);
				} else {
					std::cout << "TowerBlueprint already owned! -> Flame Turret" << std::endl;
					crystals += 40;
				}
			}
		}
		mSlotItem = nullptr;
		mCompleted = false;
		mInSlot = false;
		mExtracting = false;
		mTimerActive = false;
		});

	sf::Vector2f mPosition(mMenuShape.getPosition().x + 50.f, mMenuShape.getPosition().y + mHoveredZoneShape.getSize().y + 90.f);
	int totalSlots = mInventory.getSlotCount();
	mSlots.resize(totalSlots);
	for (int i = 0; i < totalSlots; ++i) {
		mSlots[i].setSize(slotSize);
		mSlots[i].setFillColor(sf::Color(0, 0, 0, 220));
		mSlots[i].setOutlineColor(sf::Color::White);
		mSlots[i].setOutlineThickness(2.0f);
		int row = i / 3;
		int col = i % 3;
		mSlots[i].setPosition(mPosition.x + col * slotSize.x, mPosition.y + row * slotSize.y);
	}

	mSlot.setSize(slotSize);
	mSlot.setFillColor(sf::Color(0, 0, 0, 220));
	mSlot.setOutlineColor(sf::Color::White);
	mSlot.setOutlineThickness(2.0f);
	mSlot.setPosition(sf::Vector2f(mMenuShape.getPosition().x + mMenuShape.getSize().x * 2.f / 3 - 25.f,
		                           mMenuShape.getPosition().y + mMenuShape.getSize().y * 1.f / 3 + 10.f));

	mTooltipText.setFont(mFont);
	mTooltipText.setCharacterSize(14);
	mTooltipText.setFillColor(sf::Color::White);

	mTooltipBackground.setFillColor(sf::Color(50, 50, 50, 200));
	mTooltipBackground.setOutlineColor(sf::Color::White);
	mTooltipBackground.setOutlineThickness(1.0f);
}

void AnalyzeMenu::render(sf::RenderWindow& window)
{
	window.draw(mMenuShape);
	window.draw(mHoveredZoneShape);
	window.draw(mMenuText);
	window.draw(mSlot);

	if (mInSlot)
		mStartButton.render(window);
	else if (mExtracting)
		mCancelButton.render(window);
	else if (mCompleted)
		mCompleteButton.render(window);
	
	updateSlotColors();

	for (int i = 0; i < mSlots.size(); ++i) {
		window.draw(mSlots[i]);

		const Item* item = mInventory.getItemAt(i);
		if (item) {
			sf::RectangleShape icon = item->getIcon();
			icon.setPosition(mSlots[i].getPosition());
			window.draw(icon);
		}
	}

	if (mHoveredSlot != -1) {
		if (mHoveredSlot == -2) {
			if (mSlotItem) {
				window.draw(mTooltipBackground);
				window.draw(mTooltipText);
			}
		} else {
			const Item* item = mInventory.getItemAt(mHoveredSlot);
			if (item) {
				window.draw(mTooltipBackground);
				window.draw(mTooltipText);
			}
		}
    }

    // If the error text is visible gradually make it dissapear and render it
    if (mShowText) {
        float elapsedTime = mClock.getElapsedTime().asSeconds();
        if (elapsedTime > 1.8f) {
            mShowText = false;
            mErrorText.setString("");
        }
        else {
            int alpha = static_cast<int>(255 * (1.0f - (elapsedTime / 1.8f)));
            mErrorText.setFillColor(sf::Color(255, 255, 255, alpha));
        }
        window.draw(mErrorText);
    }

	if (mSlotItem) {
		sf::RectangleShape icon = mSlotItem->getIcon();
		icon.setPosition(mSlot.getPosition());
		window.draw(icon);
	}

	if (mTimerActive) {
		int currentYear = mTimeSystem.getYear();
		int currentDay = mTimeSystem.getDay();
		int currentHour = mTimeSystem.getHour();
		int currentMinute = mTimeSystem.getMinute();

		int elapsedYears, elapsedDays, elapsedHours, elapsedMinutes;
		int hourDecrement = 0;
		int dayDecrement = 0;
		int yearDecrement = 0;

		if (currentMinute >= mStartMinute)
			elapsedMinutes = currentMinute - mStartMinute;
		else {
			elapsedMinutes = 60 + currentMinute - mStartMinute;
			hourDecrement = 1;
		}

		if (currentHour >= mStartHour)
			elapsedHours = currentHour - mStartHour - hourDecrement;
		else {
			elapsedHours = 24 + currentHour - mStartHour - hourDecrement;
			dayDecrement = 1;
		}

		if (currentDay >= mStartDay)
			elapsedDays = currentDay - mStartDay - dayDecrement;
		else {
			elapsedDays = 365 + currentDay - mStartDay - dayDecrement;
			yearDecrement = 1;
		}

		elapsedYears = currentYear - mStartYear - yearDecrement;

		int remainingHours = 23 - elapsedHours;
		int remainingMinutes = 59 - elapsedMinutes;

		std::ostringstream timerText;
		timerText << "Remaining: " << remainingHours << "h " << remainingMinutes << "m";

		sf::Text timerDisplay;
		timerDisplay.setFont(mFont);
		timerDisplay.setCharacterSize(16);
		timerDisplay.setFillColor(sf::Color::White);
		timerDisplay.setString(timerText.str());
		timerDisplay.setPosition(sf::Vector2f(mMenuShape.getPosition().x + mMenuShape.getSize().x * 2.f / 3 - 65.f ,
			                                  mMenuShape.getPosition().y + mMenuShape.getSize().y * 2.f / 3 - 170.f));

		window.draw(timerDisplay);
	}
}

void AnalyzeMenu::handleMouseClick(const sf::Vector2f& mousePos)
{
	if (mInSlot) {
	    if (mStartButton.isMouseOver(mousePos))
			mStartButton.onClick();

		if (mousePos.x >= mSlot.getPosition().x && mousePos.x <= mSlot.getPosition().x + mSlot.getSize().x &&
			mousePos.y >= mSlot.getPosition().y && mousePos.y <= mSlot.getPosition().y + mSlot.getSize().y) {
            if (mSlotItem->getId() == 2) {
			    std::unique_ptr<TowerBlueprint> towerB = std::make_unique<TowerBlueprint>();
			    mInventory.addItem(std::move(towerB), 1);
            } else if (mSlotItem->getId() == 3) {
                std::unique_ptr<TowerBlueprintRare> towerBR = std::make_unique<TowerBlueprintRare>();
                mInventory.addItem(std::move(towerBR), 1);
            } else if (mSlotItem->getId() == 4) {
                std::unique_ptr<TowerBlueprintEpic> towerBE = std::make_unique<TowerBlueprintEpic>();
                mInventory.addItem(std::move(towerBE), 1);
            } else if (mSlotItem->getId() == 5) {
                std::unique_ptr<TowerBlueprintMythic> towerBM = std::make_unique<TowerBlueprintMythic>();
                mInventory.addItem(std::move(towerBM), 1);
            }
			mSlotItem = nullptr;
			mInSlot = false;
            mCompleted = false;
            mExtracting = false;
		}
	} else if (mExtracting) {
		if (mCancelButton.isMouseOver(mousePos))
			mCancelButton.onClick();
	} else if (mCompleted) {
		if (mCompleteButton.isMouseOver(mousePos))
			mCompleteButton.onClick();
	}

	if (!mInSlot && !mExtracting && !mCompleted) {
		int hoveredSlot = getSlotIndexAtPosition(mousePos);
		if (mInventory.getItemAt(hoveredSlot)) {
			if (mInventory.getItemAt(hoveredSlot)->getId() == 2 ||
                mInventory.getItemAt(hoveredSlot)->getId() == 3 ||
                mInventory.getItemAt(hoveredSlot)->getId() == 4 ||
                mInventory.getItemAt(hoveredSlot)->getId() == 5) {
				if (mInventory.getItemQuantityAt(hoveredSlot) == 1) {
                    // if it is the last tower blueprint of the kind the removeItemAt call makes the pointer nullptr
                    // so const_char<Item*> didnt work, tried by new Item(*mInventory.getItemAt(hoveredSlot))
                    // Item is an abstract class so I cant directly instantiate itemChance
                    // maybe will add a clone method to the Item class in the future
                    if (mInventory.getItemAt(hoveredSlot)->getId() == 2)
                        mSlotItem = new TowerBlueprint();
                    else if(mInventory.getItemAt(hoveredSlot)->getId() == 3)
                        mSlotItem = new TowerBlueprintRare();
                    else if(mInventory.getItemAt(hoveredSlot)->getId() == 4)
                        mSlotItem = new TowerBlueprintEpic();
                    else if(mInventory.getItemAt(hoveredSlot)->getId() == 5)
                        mSlotItem = new TowerBlueprintMythic();
					mInventory.removeItemAt(hoveredSlot);
                } else {
                    mSlotItem = const_cast<Item*>(mInventory.getItemAt(hoveredSlot));
					mInventory.setItemQuantityAt(hoveredSlot, mInventory.getItemQuantityAt(hoveredSlot) - 1);
                }
				mInSlot = true;
			} else {
				mSlotItem = nullptr;
                mErrorText.setPosition(sf::Vector2f(mMenuShape.getPosition().x + mMenuShape.getSize().x * 0.469f,
                                                    mMenuShape.getPosition().y + mMenuShape.getSize().y * 0.657f));
                mErrorText.setString("Only extract Tower Blueprint!");
                mErrorText.setFillColor(sf::Color::White);
                mClock.restart();
                mShowText = true;
            }
		}
	} else {
        int hoveredSlot = getSlotIndexAtPosition(mousePos);
        if (hoveredSlot != -1) {
            mErrorText.setPosition(sf::Vector2f(mMenuShape.getPosition().x + mMenuShape.getSize().x * 0.091f,
                                                mMenuShape.getPosition().y + mMenuShape.getSize().y * 0.83f));
            mErrorText.setString("Already extracting!");
            mErrorText.setFillColor(sf::Color::White);
            mClock.restart();
            mShowText = true;
        }
    }
}

void AnalyzeMenu::updateHover(const sf::Vector2f& mousePos)
{
	if (mInSlot)
		mStartButton.updateHover(mousePos);
	else if (mExtracting)
		mCancelButton.updateHover(mousePos);
	else if (mCompleted)
		mCompleteButton.updateHover(mousePos);

	int hoveredSlot = getSlotIndexAtPosition(mousePos);
	if (mInventory.getItemAt(hoveredSlot))
		mHoveredSlot = hoveredSlot;
	else if (mousePos.x >= mSlot.getPosition().x && mousePos.x <= mSlot.getPosition().x + mSlot.getSize().x &&
		     mousePos.y >= mSlot.getPosition().y && mousePos.y <= mSlot.getPosition().y + mSlot.getSize().y)
		mHoveredSlot = -2;
	else
		mHoveredSlot = -1;
	updateTooltip();
}

void AnalyzeMenu::update()
{
	if (mTimerActive) {
		int currentYear = mTimeSystem.getYear();
		int currentDay = mTimeSystem.getDay();
		int currentHour = mTimeSystem.getHour();
		int currentMinute = mTimeSystem.getMinute();

		int elapsedYears, elapsedDays, elapsedHours, elapsedMinutes;
		int hourDecrement = 0;
		int dayDecrement = 0;
		int yearDecrement = 0;

		if (currentMinute >= mStartMinute)
			elapsedMinutes = currentMinute - mStartMinute;
		else {
			elapsedMinutes = 60 + currentMinute - mStartMinute;
			hourDecrement = 1;
		}

		if (currentHour >= mStartHour)
			elapsedHours = currentHour - mStartHour - hourDecrement;
		else {
			elapsedHours = 24 + currentHour - mStartHour - hourDecrement;
			dayDecrement = 1;
		}

		if (currentDay >= mStartDay)
			elapsedDays = currentDay - mStartDay - dayDecrement;
		else {
			elapsedDays = 365 + currentDay - mStartDay - dayDecrement;
			yearDecrement = 1;
		}

		elapsedYears = currentYear - mStartYear - yearDecrement;

		if (elapsedDays == 1) {
			mTimerActive = false;
			mCompleted = true;
			mExtracting = false;
		}
	}
}

void AnalyzeMenu::getInfo(int& extracting, int& inSlot, int& completed, int& timerActive, int& startYear, 
	int& startDay, int& startHour, int& startMinute, int& slotItemId)
{
	extracting = mExtracting;
	inSlot = mInSlot;
	completed = mCompleted;
	timerActive = mTimerActive;
	startYear = mStartYear;
	startDay = mStartDay;
	startHour = mStartHour;
	startMinute = mStartMinute;

	if (mSlotItem)
		slotItemId = mSlotItem->getId();
	else
		slotItemId = -1;
}

void AnalyzeMenu::setInfo(int extracting, int inSlot, int completed, int timerActive, int startYear, 
	                      int startDay, int startHour, int startMinute, int slotItemId)
{
	mExtracting = extracting;
	mInSlot = inSlot;
	mCompleted = completed;
	mTimerActive = timerActive;
	mStartYear = startYear;
	mStartDay = startDay;
	mStartHour = startHour;
	mStartMinute = startMinute;

    if(slotItemId == -1)
        mSlotItem = nullptr;
    else if(slotItemId == 2)
        mSlotItem = new TowerBlueprint();
    else if(slotItemId == 3)
        mSlotItem = new TowerBlueprintRare();
    else if(slotItemId == 4)
        mSlotItem = new TowerBlueprintEpic();
    else if(slotItemId == 5)
        mSlotItem = new TowerBlueprintMythic();
}

int AnalyzeMenu::getSlotIndexAtPosition(const sf::Vector2f& pos) const
{
	for (int i = 0; i < mSlots.size(); ++i)
		if (mSlots[i].getGlobalBounds().contains(pos))
			return i;
	return -1;
}

void AnalyzeMenu::updateTooltip()
{
	if (mHoveredSlot != -1) {
		if (mHoveredSlot == -2) {
			if (mSlotItem) {
				mTooltipText.setString(mSlotItem->getName() + "\n" + mSlotItem->getDescription());

				sf::Vector2f slotPosition = mSlot.getPosition();
				mTooltipText.setPosition(slotPosition.x + 86.5f, slotPosition.y - mTooltipBackground.getGlobalBounds().height + 25.f);

				sf::FloatRect textBounds = mTooltipText.getGlobalBounds();
				mTooltipBackground.setSize(sf::Vector2f(textBounds.width + 10, textBounds.height + 10));
				mTooltipBackground.setPosition(mTooltipText.getPosition().x - 5, mTooltipText.getPosition().y - 5);
			}
		}
		else {
			const Item* item = mInventory.getItemAt(mHoveredSlot);
			if (item) { // Check if item exists
				mTooltipText.setString(item->getName() + "   " + std::to_string(mInventory.getItemQuantityAt(mHoveredSlot))
					+ "\n" + item->getDescription());

				sf::Vector2f slotPosition = mSlots[mHoveredSlot].getPosition();
				mTooltipText.setPosition(slotPosition.x + 10, slotPosition.y - mTooltipBackground.getGlobalBounds().height - 5);

				sf::FloatRect textBounds = mTooltipText.getGlobalBounds();
				mTooltipBackground.setSize(sf::Vector2f(textBounds.width + 10, textBounds.height + 10));
				mTooltipBackground.setPosition(mTooltipText.getPosition().x - 5, mTooltipText.getPosition().y - 5);
			}
		}
	}
}

void AnalyzeMenu::updateSlotColors()
{
	for (int i = 0; i < mSlots.size(); ++i) {
		if (i == mHoveredSlot)
			mSlots[i].setFillColor(sf::Color(120, 120, 120, 210));
		else
			mSlots[i].setFillColor(sf::Color(0, 0, 0, 220));
	}

	if (mHoveredSlot == -2)
		mSlot.setFillColor(sf::Color(120, 120, 120, 210));
	else
		mSlot.setFillColor(sf::Color(0, 0, 0, 120));
}

void AnalyzeMenu::reset()
{
    mSlotItem = nullptr;
    mExtracting = false;
    mInSlot = false;
    mCompleted = false;
    mHoveredSlot = -1;
    mTimerActive = false;
    mStartMinute = 0;
    mStartHour = 0;
    mStartHour = 0;
    mStartYear = 0;
    mShowText = false;
    mClock.restart();
}
