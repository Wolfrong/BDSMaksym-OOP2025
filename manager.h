#ifndef MANAGER_H
#define MANAGER_H

#include <string>
#include "calendar.h"

class manager {
public:
    std::string name;
    std::string gender;
    int age{25};
    double weightKg{72.0};
    double heightCm{178.0};
    bool inclusive{false};

    std::string jokesFile = "anekdoty.txt";

    manager() = default;
    manager(std::string nm, std::string g, int age, double w, double h, bool incl = false);
    ~manager();

    void test_run();

private:
    CalendarBase* log{nullptr};
    static std::string today_ddmmyyyy();
};

#endif // MANAGER_H
