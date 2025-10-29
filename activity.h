#pragma once
#include <string>
// aggregation of activity information
class ActivityInfo {
public:
    std::string name;
    double MET;

    ActivityInfo() = default;
    ActivityInfo(const std::string& n, double met);

    static ActivityInfo find_activity(const std::string& activity_name);
};

class activity {
public:
    ActivityInfo info;
    double durationMin{}; 
    double weight{};      

    activity() = default;
    activity(const ActivityInfo& i, double d, double w);

    double calculate_calories() const;
};
