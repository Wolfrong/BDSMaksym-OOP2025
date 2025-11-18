#include "activity.h"
#include <fstream>
#include <sstream>
#include <iostream>

ActivityInfo::ActivityInfo(const std::string& n, double met)
    : name(n), MET(met) {
}

ActivityInfo ActivityInfo::find_activity(const std::string& activity_name) {
    std::ifstream file("activity_MET.txt");
    std::string name;
    double met;

    if (!file.is_open()) {
        std::cerr << "Error: could not open activity_MET.txt\n";
        return ActivityInfo(activity_name, 1.0);
    }

    while (file >> name >> met) {
        if (name == activity_name) {
            return ActivityInfo(name, met);
        }
    }

    std::cerr << "Warning: activity \"" << activity_name
        << "\" not found in catalog. Default MET = 1.0\n";
    return ActivityInfo(activity_name, 1.0);
}

activity::activity(const ActivityInfo& i, double d, double w, ActivityKind k)
    : info(i), durationMin(d), weightKg(w), kind(k) {
}

std::string activity::getName() const {
    return info.name;
}
double activity::calculate(double durationMin, double weightKg) const {
    switch (kind) {

    case ActivityKind::Simple:
        // всі low-intensity активності
        return durationMin * info.MET * weightKg * 0.0175;

    case ActivityKind::Cardio:
        // cardio більш енерговитратне
        return durationMin * info.MET * weightKg * 0.02;

    case ActivityKind::Strength:
        // силові мають іншу фізіологію
        return durationMin * weightKg * 0.03;

    default:
        return durationMin * info.MET * weightKg * 0.0175;
    }
}

double activity::calculate() const {
    return calculate(durationMin, weightKg);
}

