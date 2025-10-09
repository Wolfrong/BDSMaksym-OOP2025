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

void manager::test_run() const{
    Mood mood(moodLogFile.c_str(), jokesFile.c_str());
    int moodVal = 0;
    std::cout << "today`s mood (1..5): ";
    std::cin  >> moodVal;

    if (!mood.setMood(moodVal)){
        std::cout << "error \n";
    } else{
        if (!mood.save()) {
            std::cout << "error " << moodLogFile << "\n";
        }
        if (moodVal <= 2){
            char buf[512];
            if (mood.randomJoke(buf, sizeof(buf))){
                std::cout << "\n joke: " << buf << "\n";
            }
        }
    }

    int nFood = 0;
    std::cout << "\nHow many meals have you had?: ";
    std::cin  >> nFood;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    for (int i = 0; i < nFood; ++i){
        nutritions rec;
        std::cout << "\n Your meal: " << (i+1) << "\n";
        std::cout << "Dish:";
        std::cin  >> rec.dish;
        std::cout << "Meal weight (kg): ";
        std::cin  >> rec.prod_weight;
        std::cout << "Water drunk (L): ";
        std::cin  >> rec.water;
        rec.calories = rec.calculate_calories();
        rec.save_to_file(foodFile);
    }

    int nAct = 0;
    std::cout << "\n How many activities have you done?: ";
    std::cin  >> nAct;

    for (int i = 0; i < nAct; ++i)
    {
        activity a;
        std::cout << "\n Enter activity " << (i+1) << ":\n"
                  << " 1 - Running\n"
                  << " 2 - Swimming\n"
                  << " 3 - Cycling\n"
                  << " 4 - Walking\n"
                  << " 5 - StrengthTraining\n"
                  << " 6 - Yoga\n"
                  << "Your choice : ";
        int code = 1; std::cin >> code;
        switch (code){
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
        a.save_to_file(activityFile);
        std::cout << "Calories burned: " << a.calculate_calories() << "\n";
    }

    const std::string today = today_ddmmyyyy();
    int eaten  = nutritions::get_daily_calories (foodFile, today);
    double water  = nutritions::get_daily_water (foodFile, today);
    double burned = activity  ::get_daily_burned_calories(activityFile, today);

    std::cout << "\n Result on " << today << "\n";
    std::cout << " Calories eaten today: " << eaten  << " cal\n";
    std::cout << " Calories burned today: " << burned << " cal\n";
    std::cout << " Water drank today:     " << water  << " L\n";

}
