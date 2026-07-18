#include "Quest.h"

bool Quest::onEvent(const GameEvent& event) {
    if (state != QuestState::active)
        return false;

    bool updated = false;
    for (auto& objective: objectives) {
        if (!objective.isComplete() && objective.type == event.type && objective.targetId == event.targetId) {
            objective.currentAmount += event.amount;
            if (objective.currentAmount > objective.amount)
                objective.currentAmount = objective.amount;
            updated = true;
        }
    }
    return updated;
}

bool Quest::checkCompletion() const {
    for (const auto& objective: objectives) {
        if (!objective.isComplete())
            return false;
    }

    return true;
}
