#pragma once
#include <SFML/Graphics.hpp>
#include "Dialogue.h"
#include <string>

class DialogueManager
{
public:
	DialogueManager();

	void startDialogue(const Dialogue& dialogue);
	std::string getCurrentDialogueText();
	Dialogue& getCurrentDialogue();
	bool isDialogueInProgress() const;
	void resetDialogue();

	bool isDialogueCompleted(const std::string& npcName, const std::string& questName);
	void updateNPCDialogue(const std::string& npcName, const std::string& dialogueKey);
	void setNPCDialogue(const std::string& npcName, const std::string& dialogueKey);

private:
	Dialogue mCurrentDialogue;
};

