#pragma once
#include <string>

<<<<<<< HEAD
enum class ExerciseType {
    Running, 
    Swimming, 
    Cycling, 
    Walking, 
    StrengthTraining, 
    Yoga
=======
class ActivityInfo {
public:
    std::string name;
    double MET;

    ActivityInfo() = default;
    ActivityInfo(const std::string& n, double met);

    static ActivityInfo find_activity(const std::string& activity_name);
>>>>>>> origin/activitybranch2
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
