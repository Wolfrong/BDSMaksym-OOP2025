#pragma once
#include <fstream>

class nutritions{
    double weight;
    double water;
    double calculate_calories();

public:
    std::string dish;
    nutritions(std::string& dish, double weight, double water);
    nutritions(std::istream& in);
    void saveToFile(const std::string& filename);
    static void GetFromFile(const std::string& filename, const std::string& date);
};
