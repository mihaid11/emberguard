#pragma once
#include "NPC.h"
#include "../dialogueSystem/Dialogue.h"

class VincentHale : public NPC
{
public:
	VincentHale(const sf::Vector2f& position);

private:
	Dialogue mGreetingDialogue;
};

