#include "MainMenu.h"
#include "../core/GameManager.h"
#include <clocale>
#include <iostream>

MainMenu::MainMenu(sf::RenderWindow& window, GameManager* gameManager)
    : mWindow(window), mGameManager(gameManager), mShowGameButtons(false),
      mCrystals1(0), mCrystals2(0), mCrystals3(0), mYear1(1), mYear2(1),
      mYear3(1), mDay1(1), mDay2(1), mDay3(1), mHour1(6), mHour2(6),
      mHour3(6), mMinute1(0), mMinute2(0), mMinute3(0),
      mPlayButton(sf::Vector2f(0.0f, 0.0f), sf::Vector2f(140.f, 50.f), "Play"),
      mOptionsButton(sf::Vector2f(0.0f, 0.0f), sf::Vector2f(140.f, 50.f), "Options"),
      mExitButton(sf::Vector2f(0.0f, 0.0f), sf::Vector2f(140.f, 50.f), "Quit"),
      mGame1Button(sf::Vector2f(0.0f, 0.0f), sf::Vector2f(140.f, 50.f), "New Game"),
      mGame2Button(sf::Vector2f(0.0f, 0.0f), sf::Vector2f(140.f, 50.f), "New Game"),
      mGame3Button(sf::Vector2f(0.0f, 0.0f), sf::Vector2f(140.f, 50.f), "New Game"),
      mBackPlayButton(sf::Vector2f(0.0f, 0.0f), sf::Vector2f(140.f, 50.f), "Back"),
      mDelete1Button(sf::Vector2f(0.0f, 0.0f), sf::Vector2f(70.f, 28.f), "Delete"),
      mDelete2Button(sf::Vector2f(0.0f, 0.0f), sf::Vector2f(70.f, 28.f), "Delete"),
      mDelete3Button(sf::Vector2f(0.0f, 0.0f), sf::Vector2f(70.f, 28.f), "Delete") {

    if (!mFont.loadFromFile("assets/fonts/gameFont.ttf"))
        std::cout << "Couldn't load font from file" << std::endl;

    mTitleText.setFont(mFont);
    mTitleText.setCharacterSize(80);
    mTitleText.setString("Emberguard");
    mTitleText.setFillColor(sf::Color::White);
    mTitleText.setPosition(sf::Vector2f((mWindow.getSize().x - mTitleText.getLocalBounds().width) / 2.f,
                                         mWindow.getSize().y / 3.8f));

    float paddingRect = 33.f;
    mSave1Rectangle.setSize(sf::Vector2f(172.f, 215.f));
    mSave1Rectangle.setFillColor(sf::Color(50, 50, 50, 255));
    mSave1Rectangle.setPosition(sf::Vector2f((mWindow.getSize().x - 3 * mSave1Rectangle.getSize().x) / 2.f - paddingRect,
                                              mWindow.getSize().y * 2 / 3.8f));

    mSave2Rectangle.setSize(sf::Vector2f(172.f, 215.f));
    mSave2Rectangle.setFillColor(sf::Color(50, 50, 50, 255));
    mSave2Rectangle.setPosition(sf::Vector2f((mWindow.getSize().x - mSave1Rectangle.getSize().x) / 2.f,
                                              mWindow.getSize().y * 2 / 3.8f));

    mSave3Rectangle.setSize(sf::Vector2f(172.f, 215.f));
    mSave3Rectangle.setFillColor(sf::Color(50, 50, 50, 255));
    mSave3Rectangle.setPosition(sf::Vector2f((mWindow.getSize().x + mSave1Rectangle.getSize().x) / 2.f + paddingRect,
                                              mWindow.getSize().y * 2 / 3.8f));

    mSave1Text.setFont(mFont);
    mSave1Text.setCharacterSize(17);
    mSave1Text.setString("Slot 1");
    mSave1Text.setFillColor(sf::Color::White);
    mSave1Text.setPosition(sf::Vector2f(mSave1Rectangle.getPosition().x +
                                        (mSave1Rectangle.getSize().x - mSave1Text.getLocalBounds().width) / 2.f,
                                        mSave1Rectangle.getPosition().y +
                                        mSave1Rectangle.getSize().y * 0.85f));
    
    mSave2Text.setFont(mFont);
    mSave2Text.setCharacterSize(17);
    mSave2Text.setString("Slot 2");
    mSave2Text.setFillColor(sf::Color::White);
    mSave2Text.setPosition(sf::Vector2f(mSave2Rectangle.getPosition().x +
                                        (mSave2Rectangle.getSize().x - mSave2Text.getLocalBounds().width) / 2.f,
                                        mSave2Rectangle.getPosition().y +
                                        mSave2Rectangle.getSize().y * 0.85f));

    mSave3Text.setFont(mFont);
    mSave3Text.setCharacterSize(17);
    mSave3Text.setString("Slot 3");
    mSave3Text.setFillColor(sf::Color::White);
    mSave3Text.setPosition(sf::Vector2f(mSave3Rectangle.getPosition().x +
                                        (mSave3Rectangle.getSize().x - mSave3Text.getLocalBounds().width) / 2.f,
                                        mSave3Rectangle.getPosition().y +
                                        mSave3Rectangle.getSize().y * 0.85f));

    mTime1Text.setFont(mFont);
    mTime1Text.setCharacterSize(12);
    mTime1Text.setString("Time: ");
    mTime1Text.setFillColor(sf::Color(230, 230, 230, 255));
    mTime1Text.setPosition(sf::Vector2f((mWindow.getSize().x - 3 * mSave1Rectangle.getSize().x) / 2.f - paddingRect + 10.f,
                                         mWindow.getSize().y * 2 / 3.f));

    mTime2Text.setFont(mFont);
    mTime2Text.setCharacterSize(12);
    mTime2Text.setString("Time: ");
    mTime2Text.setFillColor(sf::Color(230, 230, 230, 255));
    mTime2Text.setPosition(sf::Vector2f((mWindow.getSize().x - mSave1Rectangle.getSize().x) / 2.f + 10.f,
                                         mWindow.getSize().y * 2 / 3.f));

    mTime3Text.setFont(mFont);
    mTime3Text.setCharacterSize(12);
    mTime3Text.setString("Time: ");
    mTime3Text.setFillColor(sf::Color(230, 230, 230, 255));
    mTime3Text.setPosition(sf::Vector2f((mWindow.getSize().x + mSave1Rectangle.getSize().x) / 2.f + paddingRect + 10.f,
                                         mWindow.getSize().y * 2 / 3.f));

    mCrystals1Text.setFont(mFont);
    mCrystals1Text.setCharacterSize(12);
    mCrystals1Text.setString("Crystals: ");
    mCrystals1Text.setFillColor(sf::Color(230, 230, 230, 255));
    mCrystals1Text.setPosition(sf::Vector2f((mWindow.getSize().x - 3 * mSave1Rectangle.getSize().x) / 2.f - paddingRect + 10.f,
                                         mWindow.getSize().y * 2 / 2.75f));

    mCrystals2Text.setFont(mFont);
    mCrystals2Text.setCharacterSize(12);
    mCrystals2Text.setString("Crystals: ");
    mCrystals2Text.setFillColor(sf::Color(230, 230, 230, 255));
    mCrystals2Text.setPosition(sf::Vector2f((mWindow.getSize().x - mSave1Rectangle.getSize().x) / 2.f + 10.f,
                                         mWindow.getSize().y * 2 / 2.75f));

    mCrystals3Text.setFont(mFont);
    mCrystals3Text.setCharacterSize(12);
    mCrystals3Text.setString("Crystals: ");
    mCrystals3Text.setFillColor(sf::Color(230, 230, 230, 255));
    mCrystals3Text.setPosition(sf::Vector2f((mWindow.getSize().x + mSave1Rectangle.getSize().x) / 2.f + paddingRect + 10.f,
                                         mWindow.getSize().y * 2 / 2.75f));

    float padding = 65.f;
    mPlayButton.setPosition(sf::Vector2f((mWindow.getSize().x - 3 * mPlayButton.getSize().x) / 2.f - padding,
                                         mWindow.getSize().y * 2 / 3.f));
    mOptionsButton.setPosition(sf::Vector2f((mWindow.getSize().x - mPlayButton.getSize().x) / 2.f,
                                            mWindow.getSize().y * 2 / 3.f));
    mExitButton.setPosition(sf::Vector2f((mWindow.getSize().x + mPlayButton.getSize().x) / 2.f + padding,
                                         mWindow.getSize().y * 2 / 3.f));

    mGame1Button.setPosition(sf::Vector2f((mWindow.getSize().x - 3 * mPlayButton.getSize().x) / 2.f - padding,
                                         mWindow.getSize().y * 2 / 3.6f));
    mGame2Button.setPosition(sf::Vector2f((mWindow.getSize().x - mPlayButton.getSize().x) / 2.f,
                                            mWindow.getSize().y * 2 / 3.6f));
    mGame3Button.setPosition(sf::Vector2f((mWindow.getSize().x + mPlayButton.getSize().x) / 2.f + padding,
                                         mWindow.getSize().y * 2 / 3.6f));
    mBackPlayButton.setPosition(sf::Vector2f((mWindow.getSize().x - mPlayButton.getSize().x) / 2.f,
                                              mWindow.getSize().y * 2 / 2.3f));
    mDelete1Button.setPosition(sf::Vector2f(mSave1Rectangle.getPosition().x + (mSave1Rectangle.getSize().x - mDelete1Button.getSize().x) / 2.f,
                                             mSave1Rectangle.getPosition().y + mSave1Rectangle.getSize().y + 10.f));
    mDelete2Button.setPosition(sf::Vector2f(mSave2Rectangle.getPosition().x + (mSave2Rectangle.getSize().x - mDelete2Button.getSize().x) / 2.f,
                                             mSave2Rectangle.getPosition().y + mSave2Rectangle.getSize().y + 10.f));
    mDelete3Button.setPosition(sf::Vector2f(mSave3Rectangle.getPosition().x + (mSave3Rectangle.getSize().x - mDelete3Button.getSize().x) / 2.f,
                                             mSave3Rectangle.getPosition().y + mSave3Rectangle.getSize().y + 10.f));

    mPlayButton.setCallback([&]() {
        mShowGameButtons = true;
    });

    mOptionsButton.setCallback([&]() {

    });
    
    mExitButton.setCallback([&]() {
        mWindow.close();
    });

    mGame1Button.setCallback([&]() {
        mGameManager->enterRPG(1);
    });

    mGame2Button.setCallback([&]() {
        mGameManager->enterRPG(2);
    });

    mGame3Button.setCallback([&]() {
        mGameManager->enterRPG(3);
    });

    mBackPlayButton.setCallback([&]() {
        mShowGameButtons = false;
    });

    mDelete1Button.setCallback([&]() {
        mGameManager->getGameEngine().deleteSave(1);
        updateSaves();
    });
    mDelete2Button.setCallback([&]() {
        mGameManager->getGameEngine().deleteSave(2);
        updateSaves();
    });
    mDelete3Button.setCallback([&]() {
        mGameManager->getGameEngine().deleteSave(3);
        updateSaves();
    });
    
    updateSaves();

    mButtons.push_back(mPlayButton);
    mButtons.push_back(mOptionsButton);
    mButtons.push_back(mExitButton);

    mPlayButtons.push_back(mGame1Button);
    mPlayButtons.push_back(mGame2Button);
    mPlayButtons.push_back(mGame3Button);
    mPlayButtons.push_back(mBackPlayButton);
}

void MainMenu::processEvents() {
    sf::Event event;
    while (mWindow.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            mWindow.close();
        } else if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Escape) {
                if (mShowGameButtons == true)
                    mShowGameButtons = false;
            }
        } else if (event.type == sf::Event::MouseButtonPressed) {
            if (event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f mousePos = mWindow.mapPixelToCoords(sf::Mouse::getPosition(mWindow));
                if (mShowGameButtons == false) {
                    for (auto& button: mButtons) {
                        if (button.isMouseOver(mousePos))
                            button.onClick();
                    }
                } else {
                    for (auto& button: mPlayButtons) {
                        if (button.isMouseOver(mousePos))
                            button.onClick();
                    }

                    if (mGameManager->getGameEngine().saveExists(1) && mDelete1Button.isMouseOver(mousePos))
                        mDelete1Button.onClick();
                    if (mGameManager->getGameEngine().saveExists(2) && mDelete2Button.isMouseOver(mousePos))
                        mDelete2Button.onClick();
                    if (mGameManager->getGameEngine().saveExists(3) && mDelete3Button.isMouseOver(mousePos))
                        mDelete3Button.onClick();
                }
            }
        }
    }
}

void MainMenu::update() {
    sf::Vector2f mousePos = mWindow.mapPixelToCoords(sf::Mouse::getPosition(mWindow));

    if (mShowGameButtons == false) {
        for (auto& button: mButtons)
            button.updateHover(mousePos);
    } else {
        for (auto& button: mPlayButtons) {
            button.updateHover(mousePos);
        }
        if (mGameManager->getGameEngine().saveExists(1))
            mDelete1Button.updateHover(mousePos);
        if (mGameManager->getGameEngine().saveExists(2))
            mDelete2Button.updateHover(mousePos);
        if (mGameManager->getGameEngine().saveExists(3))
            mDelete3Button.updateHover(mousePos);
    }
}

void MainMenu::render() {
    mWindow.clear();
    mWindow.draw(mTitleText);

    if (mShowGameButtons == false) {
        for (auto& button: mButtons)
            button.render(mWindow);
    } else {
        mWindow.draw(mSave1Rectangle);
        mWindow.draw(mSave2Rectangle);
        mWindow.draw(mSave3Rectangle);

        mWindow.draw(mSave1Text);
        mWindow.draw(mSave2Text);
        mWindow.draw(mSave3Text);

        if (mGameManager->getGameEngine().saveExists(1)) {
            mWindow.draw(mTime1Text);
            mWindow.draw(mCrystals1Text);
        }
        if (mGameManager->getGameEngine().saveExists(2)) {
            mWindow.draw(mTime2Text);
            mWindow.draw(mCrystals2Text);
        }
        if (mGameManager->getGameEngine().saveExists(3)) {
            mWindow.draw(mTime3Text);
            mWindow.draw(mCrystals3Text);
        }

        for (auto& button: mPlayButtons) {
            button.render(mWindow);
        }
        if (mGameManager->getGameEngine().saveExists(1))
            mDelete1Button.render(mWindow);
        if (mGameManager->getGameEngine().saveExists(2))
            mDelete2Button.render(mWindow);
        if (mGameManager->getGameEngine().saveExists(3))
            mDelete3Button.render(mWindow);
    }

    mWindow.display();
}

void MainMenu::updateSaves() {
    if (mGameManager->getGameEngine().saveExists(1)) {
        mGameManager->getGameEngine().getSaveSystem().loadPartial("save1.txt", mCrystals1, mYear1, mDay1, mHour1, mMinute1);
        if (mHour1 < 10 && mMinute1 < 10)
            mTime1Text.setString("Time:  " + std::to_string(mYear1) + "Y " +
                                std::to_string(mDay1) + "D 0" + std::to_string(mHour1) +
                                " : 0" + std::to_string(mMinute1));
        else if (mHour1 < 10 && mMinute1 >= 10)
            mTime1Text.setString("Time:  " + std::to_string(mYear1) + "Y " +
                                std::to_string(mDay1) + "D 0" + std::to_string(mHour1) +
                                " : " + std::to_string(mMinute1));
        else if (mHour1 >= 10 && mMinute1 < 10)
            mTime1Text.setString("Time:  " + std::to_string(mYear1) + "Y " +
                                std::to_string(mDay1) + "D " + std::to_string(mHour1) +
                                " : 0" + std::to_string(mMinute1));
        else if (mHour1 >= 10 && mMinute1 >= 10)
            mTime1Text.setString("Time:  " + std::to_string(mYear1) + "Y " +
                                std::to_string(mDay1) + "D " + std::to_string(mHour1) +
                                " : " + std::to_string(mMinute1));

        mCrystals1Text.setString("Crystals:  " + std::to_string(mCrystals1));
        mGame1Button.setText("Continue");
    } else {
        mTime1Text.setString("");
        mCrystals1Text.setString("");
        mGame1Button.setText("New Game");
    }
    if (mGameManager->getGameEngine().saveExists(2)) {
        mGameManager->getGameEngine().getSaveSystem().loadPartial("save2.txt", mCrystals2, mYear2, mDay2, mHour2, mMinute2);
        if (mHour2 < 10 && mMinute2 < 10)
            mTime2Text.setString("Time:  " + std::to_string(mYear2) + "Y " +
                                std::to_string(mDay2) + "D 0" + std::to_string(mHour2) +
                                " : 0" + std::to_string(mMinute2));
        else if (mHour2 < 10 && mMinute2 >= 10)
            mTime2Text.setString("Time:  " + std::to_string(mYear2) + "Y " +
                                std::to_string(mDay2) + "D 0" + std::to_string(mHour2) +
                                " : " + std::to_string(mMinute2));
        else if (mHour2 >= 10 && mMinute2 < 10)
            mTime2Text.setString("Time:  " + std::to_string(mYear2) + "Y " +
                                std::to_string(mDay2) + "D " + std::to_string(mHour2) +
                                " : 0" + std::to_string(mMinute2));
        else if (mHour2 >= 10 && mMinute2 >= 10)
            mTime2Text.setString("Time:  " + std::to_string(mYear2) + "Y " +
                                std::to_string(mDay2) + "D " + std::to_string(mHour2) +
                                " : " + std::to_string(mMinute2));

        mCrystals2Text.setString("Crystals:  " + std::to_string(mCrystals2));
        mGame2Button.setText("Continue");
    } else {
        mTime2Text.setString("");
        mCrystals2Text.setString("");
        mGame2Button.setText("New Game");
    }
    if (mGameManager->getGameEngine().saveExists(3)) {
        mGameManager->getGameEngine().getSaveSystem().loadPartial("save3.txt", mCrystals3, mYear3, mDay3, mHour3, mMinute3);
        if (mHour3 < 10 && mMinute3 < 10)
            mTime3Text.setString("Time:  " + std::to_string(mYear3) + "Y " +
                                std::to_string(mDay3) + "D 0" + std::to_string(mHour3) +
                                " : 0" + std::to_string(mMinute3));
        else if (mHour3 < 10 && mMinute3 >= 10)
            mTime3Text.setString("Time:  " + std::to_string(mYear3) + "Y " +
                                std::to_string(mDay3) + "D 0" + std::to_string(mHour3) +
                                " : " + std::to_string(mMinute3));
        else if (mHour3 >= 10 && mMinute3 < 10)
            mTime3Text.setString("Time:  " + std::to_string(mYear3) + "Y " +
                                std::to_string(mDay3) + "D " + std::to_string(mHour3) +
                                " : 0" + std::to_string(mMinute3));
        else if (mHour3 >= 10 && mMinute3 >= 10)
            mTime3Text.setString("Time:  " + std::to_string(mYear3) + "Y " +
                                std::to_string(mDay3) + "D " + std::to_string(mHour3) +
                                " : " + std::to_string(mMinute3));

        mCrystals3Text.setString("Crystals:  " + std::to_string(mCrystals3));
        mGame3Button.setText("Continue");
    } else {
        mTime3Text.setString("");
        mCrystals3Text.setString("");
        mGame3Button.setText("New Game");
    }
}

void MainMenu::reset() {
    updateSaves();
    mShowGameButtons = false;
}

