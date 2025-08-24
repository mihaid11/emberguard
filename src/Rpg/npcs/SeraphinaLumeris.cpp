#include "SeraphinaLumeris.h"

SeraphinaLumeris::SeraphinaLumeris(const sf::Vector2f& position, const std::string& id)
    : NPC(position, id) {
    mPath.push_back(sf::Vector2f(-40.0f, 960.0f));
    mPath.push_back(sf::Vector2f(190.0f, 960.0f));
    mPath.push_back(sf::Vector2f(190.0f, 1100.0f));
    mPath.push_back(sf::Vector2f(-40.0f, 1100.0f));
}

