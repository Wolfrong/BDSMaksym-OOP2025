#include "nutritions.h"
#include <iostream>
#include <fstream>
#include <unordered_map>

std::unordered_map<std::string, nutritions> loadMeals(const std::string& filename) {
    std::unordered_map<std::string, nutritions> meals;
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Error: cannot open " << filename << "\n";
        return meals;
    }

    std::string name;
    nutritions n;

    while (file >> name >> n.calories >> n.protein >> n.fats >> n.carbonates) {
        meals[name] = n;
    }
    return meals;
}

int main() {
	auto meals = loadMeals("meals.txt");
	for (const auto& [name, n] : meals) {
		std::cout << "Meal: " << name << "\n"
			<< "  Calories: " << n.calories << "\n"
			<< "  Protein: " << n.protein << "g\n"
			<< "  Fats: " << n.fats << "g\n"
			<< "  Carbonates: " << n.carbonates << "g\n\n";
	}
	return 0;
}