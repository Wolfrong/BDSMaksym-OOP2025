#pragma once
#include <string>

enum class ActivityKind {
    Simple,     
    Cardio,     
    Strength    
};

class ActivityBase {
public:
    virtual ~ActivityBase() = default;

    virtual std::string getName() const = 0;

    // Динамічний поліморфізм- різна формула для різних типів
    virtual double calculate(double durationMin, double weightKg) const = 0;
};

class ActivityInfo {
public:
    std::string name;
    double MET;

    ActivityInfo() = default;
    ActivityInfo(const std::string& n, double met);

    static ActivityInfo find_activity(const std::string& activity_name);
};

class activity : public ActivityBase {
public:
    ActivityInfo info;     // агрегація
    double durationMin{};
    double weightKg{};
    ActivityKind kind = ActivityKind::Simple; 

    activity() = default;
    activity(const ActivityInfo& i, double d, double w, ActivityKind k);

    std::string getName() const override;
    double calculate(double durationMin, double weightKg) const override;

    double calculate() const;
};