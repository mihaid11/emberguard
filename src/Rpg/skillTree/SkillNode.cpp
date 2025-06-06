#include "SkillNode.h"

SkillNode::SkillNode(const std::string& name, int cost)
    : mName(name), mCost(cost), mUnlocked(false) {
}

void SkillNode::unlock()
{
    mUnlocked = true;
}

bool SkillNode::isUnlocked()
{
    return mUnlocked;
}

const std::string& SkillNode::getName() const
{
    return mName;
}

int SkillNode::getCost()
{
    return mCost;
}
