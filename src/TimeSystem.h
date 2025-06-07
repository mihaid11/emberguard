#pragma once
#include <SFML/System/Clock.hpp>
#include <SFML/Graphics.hpp>
#include <string>

class TimeSystem {
public:
    TimeSystem(float timeScale);

    void update(float dt);
    std::string getTimeString() const;
    std::string getDateString() const;
    void resetTimeAccumulator();

    int getYear() const;
    int getDay() const;
    int getHour() const;
    int getMinute() const;

    void setYear(int year);
    void setDay(int day);
    void setHour(int hour);
    void setMinute(int minute);

private:
    int mYear;
    int mDay;
    int mHour;
    int mMinute;
    float mTimeAccumulator;
    float mTimeScale;
};

