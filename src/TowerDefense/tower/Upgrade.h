#pragma once
#include <string>
#include <functional>

class Upgrade
{
public:
    Upgrade() = default;
    Upgrade(const std::string& name, int cost, float increment, const std::function<void()>& action);

    std::string getName() const;
    int getCost() const;
    float getIncrement() const;
    void apply() const;

private:
    std::string mName;
    int mCost;
    float mIncrement;
    std::function<void()> mAction;
};
