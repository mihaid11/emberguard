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
	void updateHover(const sf::Vector2f& mousePosition, int crystals);

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

	sf::RectangleShape mStatsRectangle;
	sf::CircleShape mTowerIcon;
	sf::Text mDamageText;
	sf::Text mFireRateText;
	sf::Text mRangeText;
	sf::Text mSellPriceText;
	sf::Vector2f mStatsPosition;

	void initializeHexagon();
	void initializeLabels();
	void initializeZones();
	void initializeStats();
	bool isPointInConvexShape(const sf::ConvexShape& shape, const sf::Vector2f& point) const;
	void drawLines(sf::RenderWindow& window) const;
	void formatStats(int i);
};

