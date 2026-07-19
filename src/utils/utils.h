#pragma once
#include <SFML/Graphics.hpp>
#include <string>

std::vector<sf::String> wrapText(const sf::String& text, const sf::Font& font, unsigned int characterSize, float boxWidth);
