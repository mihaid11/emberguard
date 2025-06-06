#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class Item {
public:
    Item(const std::string& name, const std::string& description, int id, int price, const sf::RectangleShape& icon);
    virtual ~Item() = default;
    virtual std::string getType() const = 0;

    const std::string& getName() const;
    const std::string& getDescription() const;
    int getId() const;
    int getPrice() const;
    const sf::RectangleShape& getIcon() const;

private:
    std::string mName;
    std::string mDescription;
    int mId;
    int mPrice;
    sf::RectangleShape mIcon;
};
