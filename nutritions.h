#pragma once
#include "fstream"
#include "string"

class nutritions {
public:
    int calories;
    int protein;
    int fats;
    int carbonates;
    int water_L;
	int weight_g;

    
};
enum class Dish {
    Mushroom_Risotto,
    Chicken_Pasta_Sauce,
    Chicken_With_Vegetables,
    CaesarSalad,
    UkrainianBorscht,
    MashedPotatoesWithGravy,
    BeefPilaf,
    BuckwheatWithMushrooms,
    VegetableStew,
    SteamedTurkeyCutlet,
};
