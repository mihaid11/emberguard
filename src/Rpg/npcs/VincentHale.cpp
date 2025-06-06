#include "VincentHale.h"

VincentHale::VincentHale(const sf::Vector2f& position)
    : NPC(position)
{
    mShape.setFillColor(sf::Color(100, 200, 2, 255));

    mGreetingDialogue.addSegment(DialogueSegment("Hey there, Alden! Good to see you up and about."));
    mGreetingDialogue.addSegment(DialogueSegment("I was just telling old Mrs. Bea about the time we found that hidden cave up in the mountains. She still thinks I'm making it up!"));
    mGreetingDialogue.addSegment(DialogueSegment("But seriously, how've you been? If you're looking for something to do, I've got a few ideas. You know I'm always up for an adventure or two."));

    setDialogue("greeting", mGreetingDialogue);
    setActiveDialogue("greeting");

    mPath.push_back(sf::Vector2f(600.0f, 400.0f));
    mPath.push_back(sf::Vector2f(700.0f, 400.0f));
    mPath.push_back(sf::Vector2f(700.0f, 500.0f));
    mPath.push_back(sf::Vector2f(600.0f, 500.0f));
}
