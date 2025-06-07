#include "SkillTreeMenu.h"

SkillTreeMenu::SkillTreeMenu(const sf::Vector2f& position, const sf::Vector2f& size, SkillTree& skillTree)
    : mSkillTree(skillTree) {

    mMenuShape.setPosition(position);
    mMenuShape.setSize(size);
    mMenuShape.setFillColor(sf::Color(70, 70, 70, 200));

    initializeSkillButtons();
}

void SkillTreeMenu::render(sf::RenderWindow& window) {
    window.draw(mMenuShape);

    for (auto& button : mSkillButtons)
        button.render(window);
}

void SkillTreeMenu::handleMouseClick(const sf::Vector2f& mousePos) {
    for (auto& button : mSkillButtons) {
        if (button.isMouseOver(mousePos))
            button.onClick();
    }
}

void SkillTreeMenu::updateHover(const sf::Vector2f& mousePos) {
    for (auto& button : mSkillButtons)
        button.updateHover(mousePos);
}

void SkillTreeMenu::update(const SkillTree& skillTree) {
    mSkillTree = skillTree;
    updateSkillButtons();
}

void SkillTreeMenu::initializeSkillButtons() {
    sf::Vector2f buttonSize(80.0f, 30.0f);
    float startX = mMenuShape.getPosition().x + 20.0f;
    float startY = mMenuShape.getPosition().y + 20.0f;
    float gap = 10.0f;

    const auto& skillNodes = mSkillTree.getSkillNodes();

    for (size_t i = 0; i < skillNodes.size(); ++i) {
        Button skillButton(sf::Vector2f(startX, startY + i * (buttonSize.y + gap)), buttonSize, skillNodes[i]->getName());
        skillButton.setText(skillNodes[i]->getName());
        skillButton.setCallback([this, i]() {
            if (!mSkillTree.getSkillNodes()[i]->isUnlocked())
                mSkillTree.unlockSkill(mSkillTree.getSkillNodes()[i]->getName());
        });
        mSkillButtons.push_back(skillButton);
    }
}

void SkillTreeMenu::updateSkillButtons() {
    for (size_t i = 0; i < mSkillButtons.size(); ++i) {
        auto& skillNode = mSkillTree.getSkillNodes()[i];

        if (skillNode->isUnlocked()) {
            // Set the button color to indicate it's unlocked
            mSkillButtons[i].setBackgroundColor(sf::Color(0, 255, 0));
        } else {
            // Set the button color to indicate it's locked
            mSkillButtons[i].setBackgroundColor(sf::Color(255, 0, 0));
        }

        if (!skillNode->isUnlocked())
            mSkillButtons[i].setText(skillNode->getName() + " (Cost: " + std::to_string(skillNode->getCost()) + ")");
    }
}

