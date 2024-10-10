#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

class TowerSelectionMenu
{
public:
	TowerSelectionMenu();

	void show(const sf::Vector2f& position);
	void hide();
	bool isVisible() const;

	void render(sf::RenderWindow& window);
	int getSelectedTowerType(const sf::Vector2f& point) const;
	bool isInsideMenu(const sf::Vector2f& point) const;
	void updateHover(const sf::Vector2f& mousePosition);

private:
	sf::ConvexShape mHexagon;
	std::vector<sf::Text> mTowerLabels;
	std::vector<sf::ConvexShape> mZoneShapes;
	sf::Vector2f mPosition;
	bool mVisible;
	int mNumberOfTowers;
	float mMenuRadius;
	sf::Font mFont;
	int mHoveredZone;

	void initializeHexagon();
	void initializeLabels();
	void initializeZones();
	bool isPointInConvexShape(const sf::ConvexShape& shape, const sf::Vector2f& point) const;
	void drawLines(sf::RenderWindow& window) const;
	void drawZones(sf::RenderWindow& window) const;
};

