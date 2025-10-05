#include "nutritions.h"
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <sstream>

std::string getDate(){
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

nutritions::nutritions(std::string& dish, double weight, double water)
	: dish(dish), weight(weight), water(water) {
}
double nutritions::calculate_calories(){
	return weight * 1.2;
}
void nutritions::saveToFile(const std::string& meal_calendar) {
	std::string date = getDate();
    double calories = calculate_calories();
    std::ofstream out(meal_calendar, std::ios::app);
    out << date << " " << dish << " " << weight << " " << water << " " << calories << "\n";
    out.close();
    std::cout << "Saved to " << meal_calendar << " successfully.\n";
}

void nutritions::GetFromFile(const std::string& meal_calendar, const std::string& date) {
    std::ifstream in(meal_calendar);
    std::string line;
    std::stringstream ss(line);
    std::string d, dishName;
    double weight, water, calories;
    while (in >> d >> dishName >> weight >> water >> calories) {
        if (d == date) {
            std::cout << "Calories on " << d << ": " << calories << " kcal\n";
            return;
        }
    }
    std::cerr << "No record found for date: " << date << std::endl;
}