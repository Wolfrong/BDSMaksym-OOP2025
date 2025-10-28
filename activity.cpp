#include "activity.h"
#include <fstream>
#include <sstream>
#include <iostream>

ActivityInfo::ActivityInfo(const std::string& n, double met)
    : name(n), MET(met) {
}

activity::activity(const ActivityInfo& i, double d, double w)
    : info(i), durationMin(d), weight(w) {
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

<<<<<<< HEAD
double activity::get_daily_burned_calories(const std::string& filename, const std::string& date) {
    auto records = get_from_file(filename, date);
    double total = 0.0;
    for (const auto& record : records) {
        total += record.calculate_calories();
    }
    return total;
}
=======
double activity::calculate_calories() const {
    return durationMin * info.MET * weight * 0.0175;
}
>>>>>>> origin/activitybranch2
