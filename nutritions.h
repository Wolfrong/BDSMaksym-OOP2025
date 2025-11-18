#pragma once
#include <string>
#include <vector>

class Meal {
public:
    virtual ~Meal() = default;

    virtual std::string get_name() const = 0;

    virtual double get_proteins() const = 0;
    virtual double get_fats() const = 0;
    virtual double get_carbs() const = 0;
    virtual double get_calories() const { return get_proteins() * 4 + get_fats() * 9 + get_carbs() * 4; }
};

class Dishes : public Meal {
private:
    std::string name;
    double protein;
    double fat;
    double carbs;

public:
    Dishes(const std::string& n, double p, double f, double c) : name(n), protein(p), fat(f), carbs(c) {}

    std::string get_name() const override { return name; }
    double get_proteins() const override { return protein; }
    double get_fats() const override { return fat; }
    double get_carbs() const override { return carbs; }

    static Dishes Dish(const std::string& name);
	static Dishes Ingredient(const std::string& name);
};

class Self_Cook : public Meal {
private:
    std::string name;

    struct Component {
        Dishes ingredient;
        double grams;
    };

    std::vector<Component> components;

public:
    Self_Cook(const std::string& n) : name(n) {}
    ~Self_Cook() override = default;

    void add_ingredient(const std::string& ingredient_name, double grams);
    std::string get_name() const override;
    double get_proteins() const override;
    double get_fats() const override;
    double get_carbs() const override;
};

class nutritions {
private:
    Meal* meal;
    double weight;

public:
    nutritions(Meal* m, double g = 100) : meal(m), weight(g) {}

    double get_calories() const;
    double get_proteins() const;
    double get_fats() const;
    double get_carbs() const;
	double get_meal_weight() const;
};