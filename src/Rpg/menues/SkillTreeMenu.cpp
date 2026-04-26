#include "SkillTreeMenu.h"

SkillTreeMenu::SkillTreeMenu(const sf::Vector2f& position, const sf::Vector2f& size, SkillTree& skillTree)
    : Menu(size, position, true), mSkillTree(skillTree) {

    initializeSkillButtons();
    refreshButtons();

    mIsActive = true;
}

void SkillTreeMenu::render(sf::RenderWindow& window) {
    if (!mIsActive)
        return;

    window.draw(mMenuShape);

    for (auto& button : mSkillButtons)
        button->render(window);
}

void SkillTreeMenu::handleMouseClick(const sf::Vector2f& mousePos) {
    for (auto& button : mSkillButtons) {
        if (button->isMouseOver(mousePos))
            button->onClick();
    }
}

void SkillTreeMenu::updateHover(const sf::Vector2f& mousePos) {
    for (auto& button : mSkillButtons)
        button->updateHover(mousePos);
}

void SkillTreeMenu::update(float dt) {
    if (!mIsActive)
        return;
}

void SkillTreeMenu::initializeSkillButtons() {
    sf::Vector2f buttonSize(80.0f, 30.0f);
    float startX = mMenuShape.getPosition().x + 20.0f;
    float startY = mMenuShape.getPosition().y + 20.0f;
    float gap = 10.0f;

    const auto& skillNodes = mSkillTree.getSkillNodes();

    for (size_t i = 0; i < skillNodes.size(); ++i) {
        auto skillButton = std::make_unique<Button>(sf::Vector2f(startX, startY + i * (buttonSize.y + gap)), buttonSize, skillNodes[i]->getName());

        std::string skillName = skillNodes[i]->getName();
        skillButton->setCallback([this, skillName]() {
            mSkillTree.unlockSkill(skillName);
            this->refreshButtons();
        });

        mSkillButtons.push_back(std::move(skillButton));
    }
}

void SkillTreeMenu::refreshButtons() {
    const auto& skillNodes = mSkillTree.getSkillNodes();

    for (size_t i = 0; i < mSkillButtons.size(); ++i) {
        if (i >= skillNodes.size())
            break;

        if (skillNodes[i]->isUnlocked()) {
            // Set the button color to indicate it's unlocked
            mSkillButtons[i]->setBackgroundColor(sf::Color(0, 255, 0));
        } else {
            // Set the button color to indicate it's locked
            mSkillButtons[i]->setBackgroundColor(sf::Color(255, 0, 0));
        }

        if (!skillNodes[i]->isUnlocked())
            mSkillButtons[i]->setText(skillNodes[i]->getName() + " (Cost: " + std::to_string(skillNodes[i]->getCost()) + ")");
    }
}
