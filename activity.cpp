#include "activity.h"
#include <fstream>
#include <iostream>

ActivityInfo::ActivityInfo(const std::string& n, double met)
    : name(n), MET(met) {}

ActivityInfo ActivityInfo::find_activity(const std::string& activity_name) {
    std::ifstream file("activity_MET.txt");
    std::string name;
    double met;

    if (!file.is_open()) {
        return ActivityInfo(activity_name, 1.0);
    }

    while (file >> name >> met) {
        if (name == activity_name) {
            return ActivityInfo(name, met);
        }
    }
    std::cerr << "Warning: activity '" << activity_name
        << "' not found. Using default MET = 1.0\n";
    return ActivityInfo(activity_name, 1.0);
}

activity::activity(const ActivityInfo& i, double d, double w)
    : info(i), durationMin(d), weightKg(w)
{
}

std::string activity::getName() const {
    return info.name;
}

double activity::calculate(double durationMin, double weightKg) const {
    // беремо MET передаємо у змінний hook-метод
    return calcFormula(durationMin, weightKg, info.MET);
}

double activity::calculate() const {
    return calcFormula(durationMin, weightKg, info.MET);
}

// (раніше: Cardio / Strength / Simple через switch)
double activity::calcFormula(double duration, double weight, double MET) const {
    return duration * MET * weight * 0.0175;
}
