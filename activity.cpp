#include "activity.h"
#include <fstream>
#include <sstream>
#include <iostream>

ActivityInfo::ActivityInfo(const std::string& n, double met)
    : name(n), MET(met) {
}

ActivityInfo ActivityInfo::find_activity(const std::string& activity_name) {
    std::ifstream file("activity_MET.txt");
    std::string n;
    double met;

    while (file >> n >> met)
        if (n == activity_name)
            return ActivityInfo(n, met);

    return ActivityInfo(activity_name, 1.0);
}

activity::activity(const ActivityInfo& i, double d, double w)
    : info(i), durationMin(d), weightKg(w)
{
    assignStrategy();
}

void activity::assignStrategy() {
    std::string n = info.name;

    if (n == "Running" || n == "Cycling" || n == "Swimming" ||
        n == "Hiking" || n == "Rowing" || n == "JumpRope")
    {
        calcStrategy = [](double dur, double w, double MET) {
            return dur * MET * w * 0.0200;
            };
        return;
    }

    if (n == "StrengthTraining" || n == "Crossfit" || n == "Boxing")
    {
        calcStrategy = [](double dur, double w, double MET) {
            return dur * w * 0.0300;
            };
        return;
    }

    calcStrategy = [](double dur, double w, double MET) {
        return dur * MET * w * 0.0175;
        };
}

std::string activity::getName() const {
    return info.name;
}

double activity::calculate(double durationMin, double weightKg) const {
    return calcStrategy(durationMin, weightKg, info.MET);
}

double activity::calculate() const {
    return calcStrategy(durationMin, weightKg, info.MET);
}

