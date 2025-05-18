#pragma once
#include "NPC.h"

class SeraphinaLumeris : public NPC
{
public:
	SeraphinaLumeris(const sf::Vector2f& position);

private:
	Dialogue mGreetingDialogue;
};

