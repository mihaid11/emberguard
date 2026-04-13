#pragma once
#include <SFML/Graphics.hpp>
#include "Button.h"

class GameManager;

class MainMenu {
public:
    MainMenu(sf::RenderWindow& window, GameManager* gameManager);
    void processEvents();
    void update();
    void render();

    void updateSaves();
    void reset();

private:
    sf::RenderWindow& mWindow;
    GameManager* mGameManager;

    sf::Text mTitleText;
    sf::Font mFont;

    Button mPlayButton;
    Button mOptionsButton;
    Button mExitButton;
    std::vector<Button> mButtons;

    bool mShowGameButtons;
    Button mGame1Button;
    Button mGame2Button;
    Button mGame3Button;
    Button mBackPlayButton;
    Button mDelete1Button;
    Button mDelete2Button;
    Button mDelete3Button;
    std::vector<Button> mPlayButtons;

    sf::RectangleShape mSave1Rectangle;
    sf::RectangleShape mSave2Rectangle;
    sf::RectangleShape mSave3Rectangle;

    sf::Text mSave1Text;
    sf::Text mSave2Text;
    sf::Text mSave3Text;

    // Display the time for each save
    sf::Text mTime1Text;
    sf::Text mTime2Text;
    sf::Text mTime3Text;
    int mYear1, mYear2, mYear3, mDay1, mDay2, mDay3,
        mHour1, mHour2, mHour3, mMinute1, mMinute2, mMinute3;

    // Display the crystals for each save
    sf::Text mCrystals1Text;
    sf::Text mCrystals2Text;
    sf::Text mCrystals3Text;
    int mCrystals1, mCrystals2, mCrystals3;
};

