#pragma once
#include <string>
#include <vector>

class CalendarHealth {
public:

    CalendarHealth(std::string path = "calendar.txt");

    bool addActivity(const std::string& date,
                     const std::string& type,
                     double durationMin,
                     double weightKg);

    bool addCalories(const std::string& date,
                     const std::string& dish,
                     int calories);

    bool addMood(const std::string& date,
                 int mood,
                 const std::string& note);


    bool listByDate(const std::string& date,
                    std::vector<std::string>& out) const;

    static std::string today_ddmmyyyy();

private:
    std::string path_;

    static std::string trim(const std::string& s);
    static bool startsWith(const std::string& s, const std::string& pfx);

    bool ensureSkeleton() const;
    bool insertUnderSection(const std::string& section, const std::string& line) const;
};
