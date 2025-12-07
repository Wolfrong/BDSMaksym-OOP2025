#pragma once
#include <string>

class ActivityInfo {
public:
    std::string name;
    double MET;

    ActivityInfo() = default;
    ActivityInfo(const std::string& n, double met);

    static ActivityInfo find_activity(const std::string& activity_name);
};

class ActivityBase {
public:
    virtual ~ActivityBase() = default;

    virtual std::string getName() const = 0;

    // Template Method
    virtual double calculate(double durationMin, double weightKg) const = 0;
};

class activity : public ActivityBase {
public:
    ActivityInfo info;    
    double durationMin{};
    double weightKg{};

    activity() = default;
    activity(const ActivityInfo& i, double d, double w);

    std::string getName() const override;

    // Template Method
    double calculate(double durationMin, double weightKg) const override;
    double calculate() const;

private:
    // Hook
    double calcFormula(double duration, double weight, double MET) const;
};
