#pragma once
#include <string>
#include <vector>

enum class ExerciseType {
    Running, 
    Swimming, 
    Cycling, 
    Walking, 
    StrengthTraining, 
    Yoga, 
    Dancing,
    Hiking,
    Aerobics,
    Boxing,
    Rowing,
    JumpRope,
    Basketball,
    Football,
    Pilates,
    Tennis,
    Skating,
    Skiing,
    Stretching,
    StairClimbing,
    Crossfit,
    HouseCleaning,
    Gardening,
    Climbing
};

class activity {
public:
    ExerciseType type;
    double durationMin;
    double weight;

    activity() = default;
    double calculate_calories() const;

    std::string type_to_string() const;
    static ExerciseType string_to_type(const std::string& str);

    void save_to_file(const std::string& filename) const;
    static std::vector<activity> get_from_file(const std::string& filename, const std::string& date);

    static double get_daily_burned_calories(const std::string& filename, const std::string& date);
};
