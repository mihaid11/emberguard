#include "MiraStanton.h"

MiraStanton::MiraStanton(const sf::Vector2f& position) 
	: NPC(position)
{
	mShape.setFillColor(sf::Color::White);

	mGreeting1Dialogue.addSegment(DialogueSegment("Alden! There you are! I was just thinking about that shortcut we found through the forest last week."));
	mGreeting1Dialogue.addSegment(DialogueSegment("You up for another adventure? Or maybe just a quick run to gather supplies? You know me, I can't sit still for too long!"));
	mGreeting1Dialogue.addSegment(DialogueSegment("But seriously, what's up? Got any plans, or are you just here to hang out?"));

	setDialogue("greeting1", mGreeting1Dialogue);

	setActiveDialogue("greeting1");

	mPath.push_back(sf::Vector2f(1000.0f, 900.0f));
	mPath.push_back(sf::Vector2f(700.0f, 900.0f));
	mPath.push_back(sf::Vector2f(700.0f, 1000.0f));
	mPath.push_back(sf::Vector2f(500.0f, 1000.0f));
}