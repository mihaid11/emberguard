#include "QuestMenu.h"
#include <algorithm>
#include "../../utils/utils.h"

QuestMenu::QuestMenu(const sf::Vector2f& position, const sf::Vector2f& size, QuestManager& questManager)
    : Menu(size, position, true) ,mQuestManager(questManager), mPosition(position), mSize(size), mCurrentPage(1), mMaxPages(1) {
    mNoQuestsText.setFont(mFont);
    mNoQuestsText.setFillColor(sf::Color::White);
    mNoQuestsText.setCharacterSize(21);
    mNoQuestsText.setString("No active quests!");
    mNoQuestsText.setOrigin(mNoQuestsText.getLocalBounds().left + mNoQuestsText.getLocalBounds().width / 2.f,
                            mNoQuestsText.getLocalBounds().top + mNoQuestsText.getLocalBounds().height / 2.f);
    mNoQuestsText.setPosition(position + size / 2.f);

    mPageNumberText.setFont(mFont);
    mPageNumberText.setCharacterSize(12);
    mPageNumberText.setFillColor(sf::Color::White);

    sf::Vector2f buttonSize(55.f, 20.f);
    auto prevButton = std::make_unique<Button>(sf::Vector2f(position.x + size.x / 2.f - 80.f, position.y + size.y - buttonSize.y - 15.f), buttonSize, "Prev", 11);
    auto nextButton = std::make_unique<Button>(sf::Vector2f(position.x + size.x / 2.f + 80.f, position.y + size.y - buttonSize.y - 15.f), buttonSize, "Next", 11);

    prevButton->setCallback([this]() {
        if (mCurrentPage > 0) {
            mCurrentPage--;
            refresh();
        }
    });

    nextButton->setCallback([this]() {
        if (mCurrentPage < mMaxPages) {
            mCurrentPage++;
            refresh();
        }
    });

    mButtons.push_back(std::move(prevButton));
    mButtons.push_back(std::move(nextButton));

    refresh();
}

void QuestMenu::render(sf::RenderWindow& window) {
    if (!mIsActive)
        return;

    window.draw(mMenuShape);
    
    if (mQuestBoxes.empty()) {
        window.draw(mNoQuestsText);
        return;
    }

    for (const auto& box: mQuestBoxes) {
        window.draw(box.box);
        window.draw(box.title);

        for (const auto& obj: box.objectivesUI) {
            for (const auto& lineText: obj.descriptionLines)
                window.draw(lineText);

            window.draw(obj.progressBar);
            window.draw(obj.completionBar);
            window.draw(obj.progressionText);
        }
    }

    if (mMaxPages >= 0) {
        window.draw(mPageNumberText);

        for (auto& button : mButtons)
            button->render(window);
    }
}

void QuestMenu::update(float dt) {

}

void QuestMenu::handleMouseClick(const sf::Vector2f& mousePos) {
    if (!mIsActive)
        return;

    if (mMaxPages >= 0) {
        for (auto& button : mButtons)
            if (button->isMouseOver(mousePos))
                button->onClick();
    }
}

void QuestMenu::updateHover(const sf::Vector2f& mousePos) {
    if (!mIsActive)
        return;

    if (mMaxPages >= 0) {
        for (auto& button : mButtons)
            button->updateHover(mousePos);
    }
}

void QuestMenu::refresh() {
    mQuestBoxes.clear();

    std::vector<const Quest*> activeQuests = mQuestManager.getActiveQuests();

    if (activeQuests.empty())
        mMaxPages = 0;
    else
        mMaxPages = (activeQuests.size() - 1) / 3;

    if (mCurrentPage > mMaxPages)
        mCurrentPage = mMaxPages;

    float gapX = 50.f;
    float gapY = 85.f;
    float startX = mMenuShape.getPosition().x + gapX;
    sf::Vector2f boxSize((mSize.x - 4 * gapX) / 3.f, mSize.y - gapY);
    float startY = mPosition.y + gapY / 3.f;

    float bottomSpace = mPosition.y + mSize.y - startY - boxSize.y;
    mPageNumberText.setString(std::to_string(mCurrentPage + 1) + " / " + std::to_string(mMaxPages + 1));
    mPageNumberText.setPosition(mPosition.x + mSize.x / 2.f - mPageNumberText.getLocalBounds().width / 2.f,
                                startY + boxSize.y + (bottomSpace - mPageNumberText.getLocalBounds().height) / 2.f);

    float centerGap = 20.f;
    mButtons[0]->setPosition(sf::Vector2f(mPageNumberText.getPosition().x - centerGap - mButtons[0]->getSize().x,
                                          startY + boxSize.y + (bottomSpace - mButtons[0]->getSize().y) / 2.f));
    mButtons[1]->setPosition(sf::Vector2f(mPageNumberText.getPosition().x + mPageNumberText.getLocalBounds().width + centerGap,
                                          startY + boxSize.y + (bottomSpace - mButtons[1]->getSize().y) / 2.f));

    int startIndex = mCurrentPage * 3;
    int endIndex = std::min(startIndex + 3, static_cast<int>(activeQuests.size()));

    for (int i = startIndex; i < endIndex; ++i) {
        QuestBox questBox;
        int localIndex = i - startIndex;

        questBox.box.setSize(boxSize);
        questBox.box.setFillColor(sf::Color(30, 30, 30, 255));
        questBox.box.setOutlineColor(sf::Color(170, 170, 170, 255));
        questBox.box.setOutlineThickness(2.f);
        questBox.box.setPosition(startX + (boxSize.x + gapX) * localIndex, startY);
        
        questBox.title.setFont(mFont);
        questBox.title.setString(activeQuests[i]->title);
        questBox.title.setCharacterSize(15);
        questBox.title.setFillColor(sf::Color::White);
        questBox.title.setPosition(questBox.box.getPosition().x + (questBox.box.getSize().x - questBox.title.getLocalBounds().width) / 2.f,
                                   questBox.box.getPosition().y + 15.f);

        float currentY = questBox.title.getPosition().y + 45.f;

        for (const auto& obj: activeQuests[i]->objectives) {
            ObjectiveUI objUI;

            auto lines = wrapText(obj.description, mFont, 11, questBox.box.getSize().x - 20.f);

            for (const auto& line: lines) {
                sf::Text lineText;

                lineText.setFont(mFont);
                lineText.setString(line);
                lineText.setCharacterSize(11);
                lineText.setFillColor(sf::Color::White);
                lineText.setPosition(questBox.box.getPosition().x + 10.f, currentY);

                objUI.descriptionLines.push_back(lineText);
                currentY += 15.f;
            }
            currentY += 15.f;

            float barHeight = 10.f;
            objUI.progressBar.setSize(sf::Vector2f(boxSize.x - 20.f, barHeight));
            objUI.progressBar.setOutlineColor(sf::Color::White);
            objUI.progressBar.setOutlineThickness(1.f);
            objUI.progressBar.setFillColor(sf::Color(20, 20, 20, 255));
            objUI.progressBar.setPosition(questBox.box.getPosition().x + 10.f, currentY);

            float percent = 0.f;
            if (obj.amount > 0) {
                percent = static_cast<float>(obj.currentAmount) / obj.amount;
                if (percent > 1.f)
                    percent = 1.f;
            }

            objUI.completionBar.setFillColor(sf::Color::Cyan);
            objUI.completionBar.setSize(sf::Vector2f(objUI.progressBar.getSize().x * percent, barHeight));
            objUI.completionBar.setPosition(objUI.progressBar.getPosition());
            currentY += barHeight;

            objUI.progressionText.setFont(mFont);
            objUI.progressionText.setString(std::to_string(obj.currentAmount) + "/" + std::to_string(obj.amount));
            objUI.progressionText.setCharacterSize(10);
            objUI.progressionText.setFillColor(sf::Color::White);
            objUI.progressionText.setPosition(objUI.progressBar.getPosition().x + (objUI.progressBar.getSize().x - objUI.progressionText.getLocalBounds().width ) / 2.f,
                                                 objUI.progressBar.getPosition().y + objUI.progressBar.getSize().y + 10.f);
            currentY += objUI.progressionText.getLocalBounds().height * 2.f;

            questBox.objectivesUI.push_back(objUI);
            currentY += 15.f;
        }

        mQuestBoxes.push_back(questBox);
    }
}
