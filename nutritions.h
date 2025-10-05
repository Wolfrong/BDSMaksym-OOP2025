#pragma once
#include <fstream>
#include <string>
#include <vector>

class nutritions {
public:
    double prod_weight;
    double water;
    double calories;
    std::string dish;
    double calculate_calories() const;
    nutritions() = default;

    double get_calories() const { return calories; }
	double get_water() const { return water; }
	double get_prod_weight() const { return prod_weight; }

    static int get_daily_calories(const std::string& filename, const std::string& date);
    static double get_daily_water(const std::string& filename, const std::string& date);
	static double get_daily_prod_weight(const std::string& filename, const std::string& date);

    void save_to_file(const std::string& filename) const;
    static std::vector<nutritions> get_from_file(const std::string& filename, const std::string& date);
};
