#pragma once
#include <string>
#include <vector>

class CalendarBase {
public:
    virtual ~CalendarBase() = default;

    virtual bool addActivity(const std::string& date,
                             const std::string& type,
                             double durationMin,
                             double weightKg) = 0;

    virtual bool addFood(const std::string& date,
                         const std::string& dish,
                         double grams,
                         double kcal,
                         double protein,
                         double fat,
                         double carbs,
                         double waterLiters) = 0;

    virtual bool addCalories(const std::string& date,
                             const std::string& dish,
                             int calories) = 0;

    virtual bool addMood(const std::string& date,
                         int mood,
                         const std::string& note) = 0;

    virtual bool writeCaloriesSummary(const std::string& date) = 0;

    virtual bool listByDate(const std::string& date,
                            std::vector<std::string>& out) const = 0;
};

class CalendarHealth : public CalendarBase {
public:
    CalendarHealth(std::string path = "calendar.txt");

    bool addActivity(const std::string& date,
                     const std::string& type,
                     double durationMin,
                     double weightKg) override;

    bool addFood(const std::string& date,
                 const std::string& dish,
                 double grams,
                 double kcal,
                 double protein,
                 double fat,
                 double carbs,
                 double waterLiters) override;

    bool addCalories(const std::string& date,
                     const std::string& dish,
                     int calories) override;

    bool addMood(const std::string& date,
                 int mood,
                 const std::string& note) override;

    bool writeCaloriesSummary(const std::string& date) override;

    bool listByDate(const std::string& date,
                    std::vector<std::string>& out) const override;

    static std::string today_ddmmyyyy();

private:
    std::string path_;

    static std::string trim(const std::string& s);
    static bool startsWith(const std::string& s, const std::string& pfx);

    bool ensureSkeleton() const;
    bool insertUnderSection(const std::string& section,
                            const std::string& line) const;

    bool computeCaloriesForDate(const std::string& date,
                                double& kcalIn,
                                double& kcalOut) const;
};

class CalendarDecorator : public CalendarBase {
public:
    explicit CalendarDecorator(CalendarBase* inner);
    ~CalendarDecorator() override;

    bool addActivity(const std::string& date,
                     const std::string& type,
                     double durationMin,
                     double weightKg) override;

    bool addFood(const std::string& date,
                 const std::string& dish,
                 double grams,
                 double kcal,
                 double protein,
                 double fat,
                 double carbs,
                 double waterLiters) override;

    bool addCalories(const std::string& date,
                     const std::string& dish,
                     int calories) override;

    bool addMood(const std::string& date,
                 int mood,
                 const std::string& note) override;

    bool writeCaloriesSummary(const std::string& date) override;

    bool listByDate(const std::string& date,
                    std::vector<std::string>& out) const override;

protected:
    CalendarBase* inner_;
};

class LoggingCalendarDecorator : public CalendarDecorator {
public:
    explicit LoggingCalendarDecorator(CalendarBase* inner);

    bool addActivity(const std::string& date,
                     const std::string& type,
                     double durationMin,
                     double weightKg) override;

    bool addFood(const std::string& date,
                 const std::string& dish,
                 double grams,
                 double kcal,
                 double protein,
                 double fat,
                 double carbs,
                 double waterLiters) override;

    bool addMood(const std::string& date,
                 int mood,
                 const std::string& note) override;

    bool writeCaloriesSummary(const std::string& date) override;

    bool listByDate(const std::string& date,
                    std::vector<std::string>& out) const override;
};

class ValidatingCalendarDecorator : public CalendarDecorator {
public:
    explicit ValidatingCalendarDecorator(CalendarBase* inner);

    bool addActivity(const std::string& date,
                     const std::string& type,
                     double durationMin,
                     double weightKg) override;

    bool addFood(const std::string& date,
                 const std::string& dish,
                 double grams,
                 double kcal,
                 double protein,
                 double fat,
                 double carbs,
                 double waterLiters) override;

    bool addMood(const std::string& date,
                 int mood,
                 const std::string& note) override;
};
