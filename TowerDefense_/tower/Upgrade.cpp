#include "Upgrade.h"

Upgrade::Upgrade(const std::string& name, int cost, float increment, const std::function<void()>& action)
    : mName(name), mCost(cost), mAction(action), mIncrement(increment)
{
}

std::string Upgrade::getName() const {
    return mName;
}

int Upgrade::getCost() const {
    return mCost;
}

float Upgrade::getIncrement() const
{
    return mIncrement;
}

void Upgrade::apply() const {
    if (mAction)
        mAction();
}
