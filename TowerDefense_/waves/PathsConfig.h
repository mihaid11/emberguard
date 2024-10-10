#pragma once
#include <vector>
#include <SFML/Graphics.hpp>

using Path = std::vector<sf::Vector2f>;

const std::vector<std::vector<Path>> Paths = {
    std::vector<Path> {
        Path {
            sf::Vector2f(0, 250), sf::Vector2f(250, 250), sf::Vector2f(250, 120),
            sf::Vector2f(850, 120), sf::Vector2f(850, 350), sf::Vector2f(1280, 350)
        },
        Path {
            sf::Vector2f(0, 500), sf::Vector2f(250, 500), sf::Vector2f(250, 625),
            sf::Vector2f(850, 625), sf::Vector2f(850, 400), sf::Vector2f(1280, 400)
        },
            /*Path {
                sf::Vector2f(0, 300), sf::Vector2f(200, 300), sf::Vector2f(200, 600),
                sf::Vector2f(600, 600), sf::Vector2f(600, 300), sf::Vector2f(1280, 300)
            },
            Path {
                sf::Vector2f(1280, 420), sf::Vector2f(600, 420), sf::Vector2f(600, 720),
                sf::Vector2f(200, 720), sf::Vector2f(200, 420), sf::Vector2f(0, 420)
            }*/
        },
        std::vector<Path> {
            Path {
                sf::Vector2f(100, 100), sf::Vector2f(200, 200), sf::Vector2f(300, 300)
            }
                //TODO : Add more waves/content to the game
            }
};
