#pragma once
#include <string>

class nutritions {
private:
    Meal meal_info;
    double meal_weight;

public:
    nutritions(const Meal& meal, double g = 100);
    // звичайні гетери для КБЖУ, їх берем в менеджері
    double get_calories() const;
    double get_protein() const;
    double get_fat() const;
    double get_carbs() const;
    double get_meal_weight() const;
};

// класс для самих страв
class Meal{
public:
    std::string dish_name;
    // скільки БЖУ на 100г страви
    double protein;
    double fat;
    double carbs;

    Meal(const std::string& n = "", double p = 0, double f = 0, double c = 0)
        : dish_name(n), protein(p), fat(f), carbs(c) {
    }
	// крч ми читаєм наш файлік зі стравами і БЖУ
    static Meal find_dish(const std::string& meal_find);
};

// Для виклику в менеджері можна прописувати типу: "nutritions first_meal = nutritions(Meal::find_dish(вот тут берем string страви що впише користувач), Вага у грамах)"
// тобто ми створюємо прийом їжі де шукаємо страву що вибирає користувач (на даний момент перелік збережено у txt) і вказуємо вагу
// далі просто гетерами можна витягувати КБЖУ і вагу прийому їжі
// А потім вся в відповідальність на менеджері на запис у календар, бо немає сенсу його ускладнювати
// Якщо треба добавить воду то прописувати той самий об'єкт а в назву Water і грамовку, доки воно буде рахуватись прийомом їжі окремим))