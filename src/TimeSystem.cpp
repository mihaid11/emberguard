#include "TimeSystem.h"
#include <iomanip>
#include <sstream>

TimeSystem::TimeSystem(float timeScale) : mYear(1), mDay(1), mHour(6),
    mMinute(0), mTimeAccumulator(0.f), mTimeScale(timeScale)
{

}

void TimeSystem::update(float dt)
{
    mTimeAccumulator += dt;

    if (mTimeAccumulator >= mTimeScale)
    {
        mMinute += 1;
        if (mMinute >= 60)
        {
            mMinute = 0;
            mHour++;
            if (mHour >= 24)
            {
                mHour = 0;
                mDay++;
                if (mDay >= 365)
                {
                    mDay = 1;
                    mYear++;
                }
            }
        }
        mTimeAccumulator -= mTimeScale;
    }
}

std::string TimeSystem::getTimeString() const
{
    std::ostringstream oss;
    oss << std::setw(2) << std::setfill('0') << mHour << " : "
        << std::setw(2) << std::setfill('0') << mMinute;
    return oss.str();
}

std::string TimeSystem::getDateString() const
{
    std::ostringstream oss;
    oss << "Year " << mYear << ", "
        << "Day " << mDay;
    return oss.str();
}

void TimeSystem::resetTimeAccumulator()
{
    mTimeAccumulator = 0;
}

int TimeSystem::getYear() const
{
    return mYear;
}

int TimeSystem::getDay() const
{
    return mDay;
}

int TimeSystem::getHour() const
{
    return mHour;
}

int TimeSystem::getMinute() const
{
    return mMinute;
}

void TimeSystem::setYear(int year)
{
    mYear = year;
}

void TimeSystem::setDay(int day)
{
    mDay = day;
}

void TimeSystem::setHour(int hour)
{
    mHour = hour;
}

void TimeSystem::setMinute(int minute)
{
    mMinute = minute;
}
