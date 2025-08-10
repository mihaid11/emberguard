#pragma once
#include <string>
#include <unordered_map>

class StoryManager {
public:
    StoryManager() = default;

    void setFlag(const std::string& name, bool value);
    bool getFlag(const std::string& name) const;

    void setChapter(int chapter);
    int getChapter() const;

private:
    std::unordered_map<std::string, bool> mFlags;
    int mChapter;
};
