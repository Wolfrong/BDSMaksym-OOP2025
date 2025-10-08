#include "activity.h"
#include <fstream>
#include <ctime>
#include <sstream>
#include <iostream>

std::string get_the_date() {
    std::time_t t = std::time(nullptr);
    std::tm tm{};
#ifdef _WIN32
    localtime_s(&tm, &t);
#else
    localtime_r(&t, &tm);
#endif
    char date[11];
    std::strftime(date, sizeof(date), "%d-%m-%Y", &tm);
    return std::string(date);
}

std::string activity::type_to_string() const {
    switch (type) {
    case ExerciseType::Running: return "Running";
    case ExerciseType::Swimming: return "Swimming";
    case ExerciseType::Cycling: return "Cycling";
    case ExerciseType::Walking: return "Walking";
    case ExerciseType::StrengthTraining: return "StrengthTraining";
    case ExerciseType::Yoga: return "Yoga";
    default: return "Unknown";
    }
}

ExerciseType activity::string_to_type(const std::string& str) {
    if (str == "Running") return ExerciseType::Running;
    if (str == "Swimming") return ExerciseType::Swimming;
    if (str == "Cycling") return ExerciseType::Cycling;
    if (str == "Walking") return ExerciseType::Walking;
    if (str == "StrengthTraining") return ExerciseType::StrengthTraining;
    if (str == "Yoga") return ExerciseType::Yoga;
    return ExerciseType::Running;
}

double get_MET(ExerciseType type) {
    switch (type) {
    case ExerciseType::Running: return 2.5;
    case ExerciseType::Swimming: return 2.8;
    case ExerciseType::Cycling: return 2.0;
    case ExerciseType::Walking: return 1.8;
    case ExerciseType::StrengthTraining: return 2.2;
    case ExerciseType::Yoga: return 1.5;
    default: return 1.0;
    }
}

double activity::calculate_calories() const {
    return durationMin * get_MET(type) * weight * 0.0175;
}

void activity::save_to_file(const std::string& filename) const {
    std::ofstream out(filename, std::ios::app);
    out << get_the_date() << " " << type_to_string() << " " << durationMin << " " << weight << "\n";
}

std::vector<activity> activity::get_from_file(const std::string& filename, const std::string& date) {
    std::ifstream in(filename);
    std::vector<activity> result;
    if (!in) return result;

    std::string d, typeStr;
    activity record;

    while (in >> d >> typeStr >> record.durationMin >> record.weight) {
        if (d == date) {
            record.type = string_to_type(typeStr);
            result.push_back(record);
        }
    }

    return result;
}

double activity::get_daily_burned_calories(const std::string& filename, const std::string& date) {
    auto records = get_from_file(filename, date);
    double total = 0.0;
    for (const auto& record : records) {
        total += record.calculate_calories();
    }
    return total;
}
