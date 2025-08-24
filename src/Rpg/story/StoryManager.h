#pragma once
#include <string>
#include <unordered_map>

class NPCManager;

class StoryManager {
public:
    StoryManager() = default;

    void setFlag(const std::string& name, bool value);
    bool getFlag(const std::string& name) const;

    void setChapter(int chapter);
    int getChapter() const;

    const std::unordered_map<std::string, bool>& getAllFlags() const;
    void setAllFlags(const std::unordered_map<std::string, bool>& flags);

    void bindNPCManager(NPCManager* manager);

private:
    std::unordered_map<std::string, bool> mFlags;
    int mChapter = 1;
    NPCManager* mNPCManager;
};
