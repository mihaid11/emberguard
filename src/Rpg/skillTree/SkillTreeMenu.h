#pragma once
#include <SFML/Graphics.hpp>
#include "SkillTree.h"
#include "../../TowerDefense/menues/Button.h"

class SkillTreeMenu
{
public:
	SkillTreeMenu(const sf::Vector2f& position, const sf::Vector2f& size, SkillTree& skillTree);

	void render(sf::RenderWindow& window);
	void handleMouseClick(const sf::Vector2f& mousePos);
	void updateHover(const sf::Vector2f& mousePos);

	void update(const SkillTree& skillTree);

private:
	sf::RectangleShape mMenuShape;
	SkillTree& mSkillTree;
	std::vector<Button> mSkillButtons;

	void initializeSkillButtons();
	void updateSkillButtons();
};

