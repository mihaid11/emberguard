#pragma once
#include <SFML/Graphics.hpp>
#include "../skillTree/SkillTree.h"
#include "../../ui/Menu.h"
#include "../../ui/Button.h"

class SkillTreeMenu : public Menu {
public:
    SkillTreeMenu(const sf::Vector2f& position, const sf::Vector2f& size, SkillTree& skillTree);

    void render(sf::RenderWindow& window) override;
    void handleMouseClick(const sf::Vector2f& mousePos) override;
    void updateHover(const sf::Vector2f& mousePos) override;
    void update(float dt) override;

private:
    SkillTree& mSkillTree;
    std::vector<std::unique_ptr<Button>> mSkillButtons;

    void initializeSkillButtons();
    void refreshButtons();
};
