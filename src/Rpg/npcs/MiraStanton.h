#pragma once
#include "NPC.h"

class MiraStanton : public NPC {
public:
    MiraStanton(const sf::Vector2f& position);

private:
    Dialogue mGreeting1Dialogue;
};

