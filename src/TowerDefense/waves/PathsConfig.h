#pragma once
#include <vector>
#include <SFML/Graphics.hpp>

using Path = std::vector<sf::Vector2f>;

inline const std::vector<std::vector<Path>>& getPaths() {
    static const std::vector<std::vector<Path>> Paths = {
        std::vector<Path>{
            Path{
                sf::Vector2f(0, 250), sf::Vector2f(250, 250), sf::Vector2f(250, 120),
                sf::Vector2f(850, 120), sf::Vector2f(850, 350), sf::Vector2f(1280, 350)
            },
            Path{
                sf::Vector2f(0, 500), sf::Vector2f(250, 500), sf::Vector2f(250, 625),
                sf::Vector2f(850, 625), sf::Vector2f(850, 400), sf::Vector2f(1280, 400)
            }
        },
        std::vector<Path>{
            Path{
                sf::Vector2f(100, 100), sf::Vector2f(200, 200), sf::Vector2f(300, 300)
            }
        }
    };

    return Paths;
}