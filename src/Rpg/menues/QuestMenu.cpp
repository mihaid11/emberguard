#include "QuestMenu.h"
#include "../../utils/utils.h"

QuestMenu::QuestMenu(const sf::Vector2f& position, const sf::Vector2f& size, QuestManager& questManager)
    : Menu(size, position, true) ,mQuestManager(questManager), mPosition(position), mSize(size) {
    mNoQuestsText.setFont(mFont);
    mNoQuestsText.setFillColor(sf::Color::White);
    mNoQuestsText.setCharacterSize(21);
    mNoQuestsText.setString("No active quests!");
    mNoQuestsText.setOrigin(mNoQuestsText.getLocalBounds().left + mNoQuestsText.getLocalBounds().width / 2.f,
                            mNoQuestsText.getLocalBounds().top + mNoQuestsText.getLocalBounds().height / 2.f);
    mNoQuestsText.setPosition(position + size / 2.f);

    refresh();
}

void QuestMenu::render(sf::RenderWindow& window) {
    if (!mIsActive)
        return;

    window.draw(mMenuShape);
    
    if (mQuestBoxes.empty())
        window.draw(mNoQuestsText);

    for (const auto& box: mQuestBoxes) {
        window.draw(box.box);
        window.draw(box.title);

        for (const auto& lineText: box.descriptionLines)
            window.draw(lineText);

        window.draw(box.progressBar);
        window.draw(box.completionBar);
        window.draw(box.progressionText);
    }
}

void QuestMenu::update(float dt) {

}

void QuestMenu::handleMouseClick(const sf::Vector2f& mousePos) {
    if (!mIsActive)
        return;
}

void QuestMenu::updateHover(const sf::Vector2f& mousePos) {
    if (!mIsActive)
        return;
}

void QuestMenu::refresh() {
    mQuestBoxes.clear();

    std::vector<const Quest*> activeQuests = mQuestManager.getActiveQuests();

    float gap = 45.f;
    float startX = mMenuShape.getPosition().x + gap;
    sf::Vector2f boxSize((mSize.x - 4 * gap) / 3.f, mSize.y - gap);
    float startY = mPosition.y + gap / 2.f;

    for (int i = 0; i < activeQuests.size(); ++i) {
        QuestBox questBox;

        questBox.box.setSize(boxSize);
        questBox.box.setFillColor(sf::Color(30, 30, 30, 255));
        questBox.box.setOutlineColor(sf::Color::White);
        questBox.box.setOutlineThickness(2.f);
        questBox.box.setPosition(startX + (boxSize.x + gap) * i, startY);
        
        questBox.title.setFont(mFont);
        questBox.title.setString(activeQuests[i]->title);
        questBox.title.setCharacterSize(15);
        questBox.title.setFillColor(sf::Color::White);
        questBox.title.setPosition(questBox.box.getPosition().x + (questBox.box.getSize().x - questBox.title.getLocalBounds().width) / 2.f,
                                   questBox.box.getPosition().y + 15.f);

        auto lines = wrapText(activeQuests[i]->description, mFont, 12, questBox.box.getSize().x - 20.f);
        float currentY = questBox.box.getPosition().y + 90.f;

        for (const auto& line: lines) {
            sf::Text lineText;

            lineText.setFont(mFont);
            lineText.setString(line);
            lineText.setCharacterSize(12);
            lineText.setFillColor(sf::Color::White);
            lineText.setPosition(questBox.box.getPosition().x + 10.f, currentY);

            questBox.descriptionLines.push_back(lineText);
            currentY += 15.f;
        }

        float barHeight = 10.f;
        questBox.progressBar.setSize(sf::Vector2f(boxSize.x - 20.f, barHeight));
        questBox.progressBar.setOutlineColor(sf::Color::White);
        questBox.progressBar.setOutlineThickness(1.f);
        questBox.progressBar.setFillColor(sf::Color(20, 20, 20, 255));
        questBox.progressBar.setPosition(questBox.box.getPosition().x + 10.f, questBox.box.getPosition().y + questBox.box.getSize().y - 60.f);

        questBox.completionBar.setFillColor(sf::Color::Cyan);
        questBox.completionBar.setSize(sf::Vector2f(0.f, barHeight));
        questBox.completionBar.setPosition(questBox.progressBar.getPosition());
        
        questBox.progressionText.setFont(mFont);
        questBox.progressionText.setString("0/0");
        questBox.progressionText.setCharacterSize(11);
        questBox.progressionText.setFillColor(sf::Color::White);
        questBox.progressionText.setPosition(questBox.progressBar.getPosition().x + (questBox.progressBar.getSize().x - questBox.progressionText.getLocalBounds().width ) / 2.f,
                                             questBox.progressBar.getPosition().y + questBox.progressBar.getSize().y + 10.f);

        mQuestBoxes.push_back(questBox);
    }
}
