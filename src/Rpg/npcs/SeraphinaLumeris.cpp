#include "SeraphinaLumeris.h"

SeraphinaLumeris::SeraphinaLumeris(const sf::Vector2f& position) 
    : NPC(position)
{
    mShape.setFillColor(sf::Color(100, 150, 10, 255));

    mGreetingDialogue.addSegment(DialogueSegment("Ah, I knew you would come. The crystals have whispered your name for some time now. It’s good to see you again, my old friend."));
    mGreetingDialogue.addSegment(DialogueSegment("Much has changed since we last crossed paths. But the crystals… they never forget. I can sense a new energy around you—strong, yet uncertain. You seek something, don’t you? Something only the ancient stones can provide."));
    mGreetingDialogue.addSegment(DialogueSegment("I have what you need. But be warned, the crystals are alive in their own way. They will only reveal their true power to those who are ready."));

    setDialogue("greeting", mGreetingDialogue);

    setActiveDialogue("greeting");

    mPath.push_back(sf::Vector2f(-40.0f, 960.0f));
    mPath.push_back(sf::Vector2f(190.0f, 960.0f));
    mPath.push_back(sf::Vector2f(190.0f, 1100.0f));
    mPath.push_back(sf::Vector2f(-40.0f, 1100.0f));
}
