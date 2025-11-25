#include "manager.h"
#include "calendar.h"
#include "mood.h"
#include "nutritions.h"
#include "activity.h"
#include <iostream>
#include <limits>

static void safeIgnoreLine()
{
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}
manager::manager(std::string nm, std::string g, int age_, double w, double h, bool incl)
    : name(std::move(nm))
    , gender(std::move(g))
    , age(age_)
    , weightKg(w)
    , heightCm(h)
    , inclusive(incl)
    , log(nullptr)
{
    log = new CalendarHealth("calendar.txt");
}

manager::~manager()
{
    delete log;
    log = nullptr;
}

std::string manager::today_ddmmyyyy()
{
    return CalendarHealth::today_ddmmyyyy();
}


void manager::test_run()
{
    if (!log)
    {
        log = new CalendarHealth("calendar.txt");
    }

    const std::string today = today_ddmmyyyy();

    int moodVal = 0;
    std::cout << "today`s mood (1..5): ";
    std::cin  >> moodVal;

    if (moodVal >= 1 && moodVal <= 5) {
        if (!log->addMood(today, moodVal, "")) {
            std::cout << "write error (mood)\n";
        }

        if (moodVal < 2) {
            Mood m(nullptr, jokesFile.c_str());
            m.setMood(moodVal);
            char buf[512];
            if (m.randomJoke(buf, sizeof(buf))) {
                std::cout << "\njoke: " << buf << "\n";
            }
        }
    } else {
        std::cout << "error: mood out of range\n";
    }

    int nFood = 0;
    std::cout << "\nHow many meals have you had?: ";
    std::cin  >> nFood;
    safeIgnoreLine();

    for (int i = 0; i < nFood; ++i)
    {
        std::cout << "\nMeal " << (i + 1)
                  << "\nDish name (as in meals_list.txt): ";
        std::string dish;
        std::getline(std::cin, dish);

        double grams = 0.0;
        std::cout << "Meal weight (grams): ";
        std::cin  >> grams;

        double waterL = 0.0;
        std::cout << "Water drunk with meal (L): ";
        std::cin  >> waterL;
        safeIgnoreLine();

        Dishes d = Dishes::Dish(dish);
        nutritions nut(&d, grams);

        const double kcal = nut.get_calories();
        const double p    = nut.get_proteins();
        const double f    = nut.get_fats();
        const double c    = nut.get_carbs();

        if (!log->addFood(today, dish, grams, kcal, p, f, c, waterL))
            std::cout << "write error (food)\n";
    }

    int nAct = 0;
    std::cout << "\nHow many activities have you done?: ";
    std::cin  >> nAct;
    safeIgnoreLine();

    for (int i = 0; i < nAct; ++i)
    {
        std::cout << "\nEnter activity name (exact as in activity_MET.txt): ";
        std::string actName;
        std::getline(std::cin, actName);

        double durMin = 0.0;
        std::cout << "Duration (minutes): ";
        std::cin  >> durMin;

        double w = (weightKg > 0.0 ? weightKg : 70.0);
        safeIgnoreLine();

        ActivityInfo info = ActivityInfo::find_activity(actName);

        if (!log->addActivity(today, info.name, durMin, w))
            std::cout << "write error (activity)\n";
    }

    if (!log->writeCaloriesSummary(today))
    {
        std::cout << "warning: calories summary write failed\n";
    }

    std::string qdate;
    std::cout << "\nEnter date to show all logs (dd-mm-YYYY), empty for today: ";
    std::getline(std::cin, qdate);
    if (qdate.empty()) qdate = today;

    std::vector<std::string> lines;
    if (log->listByDate(qdate, lines))
    {
        std::cout << "\n=== All on " << qdate << " ===\n";
        for (auto& s : lines)
        {
            std::cout << s << "\n";
        }
    } else {
        std::cout << "read error\n";
    }
}
