#pragma once
#include <string>
#include "SkillNode.h"
#include <memory>

class SkillTree {
public:
    SkillTree();

    void addSkillNode(const std::string& name, int cost);
    void unlockSkill(const std::string& name);
    const std::vector<std::shared_ptr<SkillNode>>& getSkillNodes() const;

private:
    std::vector<std::shared_ptr<SkillNode>> mSkillNodes;
};

