#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "../../ui/Button.h"

class GameEngine;
class GameManager;

class SmallMenu {
public:
    SmallMenu(sf::RenderWindow& window, GameEngine* game, GameManager* gameManager,
              int level, std::vector<int>& availableTowers);

    void render(sf::RenderWindow& window);
    void handleMouseClick(const sf::Vector2f& mousePos);
    void updateHover(const sf::Vector2f& mousePos);

    bool isVisible() const;
    void show();
    void hide();

private:
    sf::RectangleShape mMenuBackground;
    sf::RectangleShape mBackground;

    std::vector<Button> mButtons;
    Button quitButton;
    Button restartButton;

    GameEngine* mGame;
    GameManager* mGameManager;
    int mLevel;
    bool mIsVisible;

    std::vector<int>& mAvailableTowers;
};

