#pragma once
#include <string>
#include <functional>

// inheritance changed to strategy type pattern
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
    double weightKg{};

private:
    std::function<double(double, double, double)> calcStrategy;

public:
    activity() = default;
    activity(const ActivityInfo& i, double d, double w);

    std::string getName() const;
    double calculate(double durationMin, double weightKg) const;
    double calculate() const;

private:
    void assignStrategy();  // вибір стратегії
};
