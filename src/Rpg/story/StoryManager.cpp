#include "StoryManager.h"
#include "../npcs/NPCManager.h"

void StoryManager::bindNPCManager(NPCManager* manager) {
    mNPCManager = manager;
}

void StoryManager::setFlag(const std::string& name, bool value) {
    mFlags[name] = value;
    if (mNPCManager) mNPCManager->refreshNPCDialogues();
}

void StoryManager::setChapter(int chapter) {
    mChapter = chapter;
    if (mNPCManager) mNPCManager->refreshNPCDialogues();
}

bool StoryManager::getFlag(const std::string& name) const {
    auto it = mFlags.find(name);
    if (it != mFlags.end()) {
        return it->second;
    }
    return false;
}

int StoryManager::getChapter() const {
    return mChapter;
}

const std::unordered_map<std::string, bool>& StoryManager::getAllFlags() const {
    return mFlags;
}

void StoryManager::setAllFlags(const std::unordered_map<std::string, bool>& flags) {
    mFlags = flags;
}
