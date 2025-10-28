#pragma once
#include <string>
#include <unordered_map>

class nutritions {
private:
    ProductInfo product_info;
    double meal_weight;

public:
    nutritions(const ProductInfo& p, double g = 100);
    // звичайні гетери для КБЖУ, їх берем в менеджері
    double get_calories() const;
    double get_protein() const;
    double get_fat() const;
    double get_carbs() const;
};

// класс для самих страв
class ProductInfo{
public:
    std::string dish_name;
    // скільки БЖУ на 100г страви
    double protein;
    double fat;
    double carbs;

    ProductInfo(const std::string& n = "", double p = 0, double f = 0, double c = 0)
        : dish_name(n), protein(p), fat(f), carbs(c) {
    }
	// крч ми читаєм наш файлік зі стравами і БЖУ
    ProductInfo ProductInfo::find_meal(const std::string& meal_find);
};