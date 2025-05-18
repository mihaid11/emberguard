#include "SkillTree.h"

SkillTree::SkillTree()
{
	addSkillNode("Fireball", 10);
	addSkillNode("Ice Blast", 10);
	addSkillNode("Lightning Strike", 20);
}

void SkillTree::addSkillNode(const std::string& name, int cost)
{
	mSkillNodes.push_back(std::make_shared<SkillNode>(name, cost));
}

void SkillTree::unlockSkill(const std::string& name)
{
	for (auto& node : mSkillNodes) {
		if (node->getName() == name && !node->isUnlocked()) {
			node->unlock();
			break;
		}
	}
}

const std::vector<std::shared_ptr<SkillNode>>& SkillTree::getSkillNodes() const
{
	return mSkillNodes;
}
