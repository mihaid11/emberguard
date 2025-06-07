#pragma once
#include <SFML/Graphics.hpp>
#include "Button.h"
#include <vector>

class GameEngine;
class GameManager;

class LevelCompleteMenu {
public:
    LevelCompleteMenu(sf::RenderWindow& window, GameEngine* game, GameManager* gameManager,
                      int level, int crystals);

    void render(sf::RenderWindow& window);
    void handleMouseClick(const sf::Vector2f& mousePos);
    void updateHover(const sf::Vector2f& mousePos);
    void updateCrystals(int crystals);

private:
    sf::RectangleShape mMenuShape;

    std::vector<Button> mButtons;
    Button continueButton;

    GameEngine* mGame;
    GameManager* mGameManager;
    int mLevel;
    int mCrystals;
};

