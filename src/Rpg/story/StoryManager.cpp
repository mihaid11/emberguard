#include "StoryManager.h"

void StoryManager::setFlag(const std::string& name, bool value) {
    mFlags[name] = value;
}

bool StoryManager::getFlag(const std::string& name) const {
    auto it = mFlags.find(name);
    if (it != mFlags.end()) {
        return it->second;
    }
    return false;
}

void StoryManager::setChapter(int chapter) {
    mChapter = chapter;
}

int StoryManager::getChapter() const {
    return mChapter;
}
