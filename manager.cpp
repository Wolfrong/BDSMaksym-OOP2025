#include "calendar.h"
#include "manager.h"
#include "mood.h"
#include "nutritions.h"
#include "activity.h"
#include <iostream>
#include <limits>
#include <ctime>

manager::manager(std::string nm, std::string g, int age, double w, double h, bool incl)
: name(std::move(nm)), gender(std::move(g)), age(age), weightKg(w), heightCm(h), inclusive(incl) {}

std::string manager::today_ddmmyyyy()
{
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

void manager::test_run() const
{
    CalendarHealth log;
    const std::string today = today_ddmmyyyy();

    int moodVal = 0;
    std::cout << "today`s mood (1..5): ";
    std::cin  >> moodVal;
    if (moodVal < 1 || moodVal > 5) {
        std::cout << "error: mood out of range\n";
    } else {
        std::string note = (moodVal >=4 ? "great_day" : (moodVal==3 ? "ok" : "tired"));
        if (!log.addMood(today, moodVal, note)) std::cout << "write error (mood)\n";
    }

    int nFood = 0;
    std::cout << "\nHow many meals have you had?: ";
    std::cin  >> nFood;
    for (int i = 0; i < nFood; ++i)
    {
        nutritions rec;
        std::cout << "\n Meal " << (i+1) << "\nDish: ";
        std::cin  >> rec.dish;
        std::cout << "Meal weight (kg): ";
        std::cin  >> rec.prod_weight;
        std::cout << "Water drunk (L): ";
        std::cin  >> rec.water;

        rec.calories = rec.calculate_calories();
        const int calInt = (int)(rec.calories + 0.5);
        if (!log.addCalories(today, rec.dish, calInt)) std::cout << "write error (calories)\n";
    }

    int nAct = 0;
    std::cout << "\n How many activities have you done?: ";
    std::cin  >> nAct;
    for (int i = 0; i < nAct; ++i)
    {
        activity a;
        std::cout << "\n Enter activity " << (i+1) << ":\n"
                  << " 1 - Running\n 2 - Swimming\n 3 - Cycling\n"
                  << " 4 - Walking\n 5 - StrengthTraining\n 6 - Yoga\n"
                  << "Your choice : ";
        int code = 1; std::cin >> code;
        switch (code)
        {
            case 1: a.type = ExerciseType::Running; break;
            case 2: a.type = ExerciseType::Swimming; break;
            case 3: a.type = ExerciseType::Cycling; break;
            case 4: a.type = ExerciseType::Walking; break;
            case 5: a.type = ExerciseType::StrengthTraining; break;
            case 6: a.type = ExerciseType::Yoga; break;
            default: a.type = ExerciseType::Running; break;
        }
        std::cout << "Duration (Min): ";
        std::cin  >> a.durationMin;
        a.weight = (weightKg > 0.0 ? weightKg : 70.0);

        if (!log.addActivity(today, a.type_to_string(), a.durationMin, a.weight))
            std::cout << "write error (activity)\n";
    }

    std::string qdate;
    std::cout << "\nEnter date to show all logs (dd-mm-YYYY), empty for today: ";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::getline(std::cin, qdate);
    if (qdate.empty()) qdate = today;

    std::vector<std::string> lines;
    if (log.listByDate(qdate, lines)) {
        std::cout << "\n=== All on " << qdate << " ===\n";
        for (auto& s : lines) std::cout << s << "\n";
    } else {
        std::cout << "read error\n";
    }
}

