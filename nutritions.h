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

    virtual double get_calories() const {
        return get_proteins() * 4 + get_fats() * 9 + get_carbs() * 4;
    }
};

class Dishes : public Meal {
private:
    std::string name;
    double protein;
    double fat;
    double carbs;

public:
    Dishes(const std::string& names, double proteins, double fats, double carb)
        : name(names), protein(proteins), fat(fats), carbs(carb) {
    }

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

    void add_ingredient(const std::string& ingredient_name, double grams);

    std::string get_name() const override { return name; }

    double get_proteins() const override;
    double get_fats() const override;
    double get_carbs() const override;
};

class NutritionDecorator : public Meal {
private:
    Meal* wrappee;
    double weight;

public:
    NutritionDecorator(Meal* m, double w = 100) : wrappee(m), weight(w) {}

    std::string get_name() const override { return wrappee->get_name(); }

    double get_proteins() const override {
        return wrappee->get_proteins() * weight / 100.0;
    }

    double get_fats() const override {
        return wrappee->get_fats() * weight / 100.0;
    }

    double get_carbs() const override {
        return wrappee->get_carbs() * weight / 100.0;
    }

    double get_calories() const override {
        return Meal::get_calories();
    }

    double get_weight() const { return weight; }
};

class MealFactory {
public:
    virtual Meal* create(const std::string& name) = 0;
    virtual ~MealFactory() = default;
};

class DishFactory : public MealFactory {
public:
    Meal* create(const std::string& name) override {
        return new Dishes(Dishes::Dish(name));
    }
};

class IngredientFactory : public MealFactory {
public:
    Meal* create(const std::string& name) override {
        return new Dishes(Dishes::Ingredient(name));
    }
};

class SelfCookFactory : public MealFactory {
public:
    Meal* create(const std::string& name) override {
        return new Self_Cook(name);
    }
};