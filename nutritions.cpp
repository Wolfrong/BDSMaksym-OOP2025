#include "nutritions.h"
#include <iostream>
#include <fstream>
#include <ctime>
#include <sstream>

std::string get_date() {
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

double nutritions::calculate_calories() const {
    return prod_weight * 1.2;
}

void nutritions::save_to_file(const std::string& filename) const {
    std::ofstream out(filename, std::ios::app);
    out << get_date() << " " << dish << " " << prod_weight << " " << calories << " " << water << "\n";
}
std::vector<nutritions> nutritions::get_from_file(const std::string& filename, const std::string& date) {
    std::ifstream in(filename);
    std::vector<nutritions> result;
    if (!in) return result;
    std::string d, dish;
    nutritions record;
    while (in >> d >> record.dish >> record.prod_weight >> record.calories >> record.water) {
        if (d == date) {
            result.push_back(record);
        }
    }
    return result;
}

int nutritions::get_daily_calories(const std::string& filename, const std::string& date)
{
    auto records = get_from_file(filename, date);
	int total_calories = 0;
	for (const auto& record : records) {
		total_calories += record.get_calories();
	}
	return total_calories;
}
double nutritions::get_daily_water(const std::string& filename, const std::string& date)
{
    auto records = get_from_file(filename, date);
    double total_water = 0.0;
    for (const auto& record : records) {
        total_water += record.get_water();
    }
    return total_water;
}
double nutritions::get_daily_prod_weight(const std::string& filename, const std::string& date)
{
    auto records = get_from_file(filename, date);
    double total_prod_weight = 0.0;
    for (const auto& record : records) {
        total_prod_weight += record.get_prod_weight();
    }
    return total_prod_weight;
}