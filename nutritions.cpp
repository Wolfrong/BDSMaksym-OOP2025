#include "nutritions.h"
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <iostream>

nutritions::nutritions(const ProductInfo& p, double g) 
    : product_info(p), meal_weight(g) {}

double nutritions::get_calories() const{
    return (product_info.protein * 4 + product_info.fat * 9 + product_info.carbs * 4) * meal_weight / 100.0;
}

double nutritions::get_protein() const{ 
    return product_info.protein * meal_weight / 100.0; 
}
double nutritions::get_fat() const{ 
    return product_info.fat * meal_weight / 100.0; 
}
double nutritions::get_carbs() const{ 
    return product_info.carbs * meal_weight / 100.0; 
}

ProductInfo ProductInfo::find_meal(const std::string& meal_find) {
    std::ifstream in("meals_list.txt");
    if (!in) return ProductInfo();
    std::string meal_name;
    double protein, fat, carbs;

    while (in >> meal_name >> protein >> fat >> carbs) {
        if (meal_name == meal_find) {
            return ProductInfo(meal_name, protein, fat, carbs);
        }
    }
}