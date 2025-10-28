#include "nutritions.h"
#include <fstream>
#include <string>

nutritions::nutritions(const Meal& meal, double g) 
    : meal_info(meal), meal_weight(g) {}

double nutritions::get_calories() const{
    return (meal_info.protein * 4 + meal_info.fat * 9 + meal_info.carbs * 4) * meal_weight / 100.0;
}

double nutritions::get_protein() const{ 
    return meal_info.protein * meal_weight / 100.0; 
}
double nutritions::get_fat() const{ 
    return meal_info.fat * meal_weight / 100.0; 
}
double nutritions::get_carbs() const{ 
    return meal_info.carbs * meal_weight / 100.0; 
}
double nutritions::get_meal_weight() const{ 
    return meal_weight; 
}

Meal Meal::find_dish(const std::string& dish_find) {
    std::fstream file("meals_list.txt");
    std::string dish_name;
    double protein, fat, carbs;
    if(dish_name == "Water"){
        return Meal("Water", 0, 0, 0);
	}
    while (file >> dish_name >> protein >> fat >> carbs) {
        if (dish_name == dish_find) {
            return Meal(dish_name, protein, fat, carbs);
        }
    }
}