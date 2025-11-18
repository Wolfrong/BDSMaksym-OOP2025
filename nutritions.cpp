#include "nutritions.h"
#include <fstream>
#include <string>

double nutritions::get_calories() const { return meal->get_calories() * weight / 100.0; }
double nutritions::get_proteins() const { return meal->get_proteins() * weight / 100.0; }
double nutritions::get_fats() const { return meal->get_fats() * weight / 100.0; }
double nutritions::get_carbs() const { return meal->get_carbs() * weight / 100.0; }
double nutritions::get_meal_weight() const { return weight; }

Dishes Dishes::Dish(const std::string& dish_find) {
    std::fstream file("meals_list.txt");
    std::string name;
    double protein, fat, carbs;
    if (dish_find == "Water") {
        return Dishes("Water", 0, 0, 0);
    }

    while (file >> name >> protein >> fat >> carbs) {
        if (name == dish_find) {
            return Dishes(name, protein, fat, carbs);
        }
    }
    return Dishes("Blank", 0, 0, 0);
}

Dishes Dishes::Ingredient(const std::string& ingredient_find) {
    std::ifstream file("ingredients_list.txt");
    std::string name;
    double protein, fat, carbs;

    while (file >> name >> protein >> fat >> carbs) {
        if (name == ingredient_find) {
            return Dishes(name, protein, fat, carbs);
        }
    }
    return Dishes("Blank", 0, 0, 0);
}

void Self_Cook::addIngredient(const Dishes& ingredient, double grams) {
    components.push_back({ ingredient, grams });
}
std::string Self_Cook::get_name() const { return name; }

double Self_Cook::get_proteins() const {
    double sum = 0;
    for (auto& c : components)
        sum += c.ingredient.get_proteins() * c.grams / 100.0;
    return sum;
}

double Self_Cook::get_fats() const {
    double sum = 0;
    for (auto& c : components)
        sum += c.ingredient.get_fats() * c.grams / 100.0;
    return sum;
}

double Self_Cook::get_carbs() const {
    double sum = 0;
    for (auto& comps : components)
        sum += comps.ingredient.get_carbs() * comps.grams / 100.0;
    return sum;
}