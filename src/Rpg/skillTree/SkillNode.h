#pragma once
#include <string>
#include <memory>
#include <vector>

class SkillNode
{
public:
    SkillNode(const std::string& name, int cost);

    void unlock();
    bool isUnlocked();
    const std::string& getName() const;
    int getCost();

private:
    std::string mName;
    int mCost;
    bool mUnlocked;
};

