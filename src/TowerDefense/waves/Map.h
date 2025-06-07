#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

class Map {
public:
    Map(int level);

    void render(sf::RenderWindow& window);
    bool canPlaceTower(const sf::Vector2f& position) const;

private:
    void initializeLevel(int level);
    void createRoad(const std::vector<sf::Vector2f>& path);

    std::vector<sf::RectangleShape> mRoad;
};

