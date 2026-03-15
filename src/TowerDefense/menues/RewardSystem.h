#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <random>

struct Reward {
    std::string name;
    int quantity;
    int itemId;
    sf::Texture icon;
    sf::Sprite sprite;
};

class RewardSystem {
public:
    static Reward generateTowerLevelReward(int level);
    static Reward generateReward(int level);
};

