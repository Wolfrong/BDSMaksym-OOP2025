#ifndef MANAGER_H
#define MANAGER_H

#include <string>

class manager {
public:
    std::string name;
    std::string gender;
    int age{25};
    double weightKg{72.0};
    double heightCm{178.0};
    bool inclusive{false};

    std::string foodFile = "nutrition_log.txt";
    std::string activityFile = "activity_log.txt";
    std::string moodLogFile = "mood_log.txt";
    std::string jokesFile = "anekdoty.txt";

    manager() = default;
    manager(std::string nm, std::string g, int age, double w, double h, bool incl = false);

    void test_run() const;

private:
    static std::string today_ddmmyyyy();
};

#endif // MANAGER_H
