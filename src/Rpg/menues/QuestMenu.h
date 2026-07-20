#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "../../ui/Menu.h"
#include "../../core/quests/QuestManager.h"

class QuestMenu: public Menu {
public:
    QuestMenu(const sf::Vector2f& position, const sf::Vector2f& size, QuestManager& questManager);

    void render(sf::RenderWindow& window) override;
    void handleMouseClick(const sf::Vector2f& mousePos) override;
    void updateHover(const sf::Vector2f& mousePos) override;
    void update(float dt) override;

    void refresh();
private:
    QuestManager& mQuestManager;
    sf::Text mNoQuestsText;
    sf::Vector2f mPosition;
    sf::Vector2f mSize;

    struct ObjectiveUI {
        std::vector<sf::Text> descriptionLines;
        sf::RectangleShape progressBar;
        sf::RectangleShape completionBar;
        sf::Text progressionText;
    };

    struct QuestBox {
        sf::RectangleShape box;
        sf::Text title;
        std::vector<ObjectiveUI> objectivesUI;
    };
    std::vector<QuestBox> mQuestBoxes;
};
