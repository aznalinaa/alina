#include <iostream>
#include <vector>
#include <list>
#include <stdlib.h>
#include <ctime>
#include "patterntemplates.h"

using namespace std;

// Цвет кошки
enum class CatColor : int {
    White,
    Black,
    Gray,
    Unknown
};

// Реализация паттерна "Стратегия" для способов кормления кошек
enum class EatingMannerEnum : int {
    Fast,
    Slow,
    Playful,
    None
};

class EatingStrategy {
public:
    virtual ~EatingStrategy() {}
    virtual void Eat() = 0;
};

class FastEatingStrategy : public EatingStrategy {
    void Eat() { cout << "Eating FAST... "; }
};

class SlowEatingStrategy : public EatingStrategy {
    void Eat() { cout << "Eating SLOWLY... "; }
};

class PlayfulEatingStrategy : public EatingStrategy {
    void Eat() { cout << "Eating PLAYFULLY... "; }
};

// Фабрика стратегий кормления
EatingStrategy* CreateEatingStrategy(EatingMannerEnum eatingManner) {
    switch(eatingManner) {
        case EatingMannerEnum::Fast: return new FastEatingStrategy;
        case EatingMannerEnum::Slow: return new SlowEatingStrategy;
        case EatingMannerEnum::Playful: return new PlayfulEatingStrategy;
        default: return nullptr;
    }
}

// Базовый класс "Кошка" с шаблонным методом Eat()
class Cat {
private:
    CatColor Color;
    double Weight;
    EatingStrategy* EatingManner;

protected:
    bool IsHungry;

    void DoEatUsingStrategy() {
        if (EatingManner == nullptr) {
            cout << "No eating strategy set! ";
            return;
        } else {
            EatingManner->Eat();
        }
    }

    void DetectHunger() {
        if (IsHungry) {
            cout << "HUNGRY : ";
        } else {
            cout << "FULL : ";
        }
    }

public:
    Cat(CatColor color) : Color(color), Weight(0.0), IsHungry(true), EatingManner(nullptr) {
        IsHungry = static_cast<bool>(rand() % 2);
        Weight = static_cast<double>(rand() % 100) / 10.0;
    }

    virtual ~Cat() {
        if (EatingManner != nullptr) delete EatingManner;
    }

    bool Hungry() const { return IsHungry; }
    CatColor GetColor() const { return Color; }
    double GetWeight() const { return Weight; }

    virtual void PrintType() = 0;

    // Шаблонный метод Eat()
    void Eat() {
        // 1. Вывести тип кошки
        PrintType();
        cout << " : ";

        // 2. Определить, голодна ли кошка
        DetectHunger();

        // 3. Использовать стратегию кормления
        DoEatUsingStrategy();

        // 4. Вывести звуки, которые издает кошка
        MakeSound();

        cout << endl;
    }

    virtual void MakeSound() = 0;

    void SetEatingManner(EatingStrategy* eatingManner) {
        EatingManner = eatingManner;
    }
};

class Siamese : public Cat {
public:
    Siamese() : Cat(CatColor::White) {
        SetEatingManner(CreateEatingStrategy(EatingMannerEnum::Slow));
    }
    ~Siamese() {}

    void PrintType() override { cout << "Siamese"; }
    void MakeSound() override { cout << "Meow meow..."; }
};

class MaineCoon : public Cat {
public:
    MaineCoon() : Cat(CatColor::Gray) {
        SetEatingManner(CreateEatingStrategy(EatingMannerEnum::Playful));
    }
    ~MaineCoon() {}

    void PrintType() override { cout << "Maine Coon"; }
    void MakeSound() override { cout << "Mrrr mrrr..."; }
};

class BritishShorthair : public Cat {
public:
    BritishShorthair() : Cat(CatColor::Black) {
        SetEatingManner(CreateEatingStrategy(EatingMannerEnum::Fast));
    }
    ~BritishShorthair() {}

    void PrintType() override { cout << "British Shorthair"; }
    void MakeSound() override { cout << "Purr purr..."; }
};

// Фабрика кошек
enum class CatType : int {
    Siamese = 1,
    MaineCoon = 2,
    BritishShorthair = 3,
    Undefined = 0
};

Cat* CreateCat(CatType type) {
    if (type == CatType::Siamese) return new Siamese();
    if (type == CatType::MaineCoon) return new MaineCoon();
    if (type == CatType::BritishShorthair) return new BritishShorthair();
    return nullptr;
}

// Декоратор по цвету
class CatColorDecorator : public IteratorDecorator<Cat*> {
private:
    CatColor TargetColor;
public:
    CatColorDecorator(Iterator<Cat*>* it, CatColor color)
        : IteratorDecorator<Cat*>(it), TargetColor(color) {}
    void First() override {
        It->First();
        while (!It->IsDone() && It->GetCurrent()->GetColor() != TargetColor) {
            It->Next();
        }
    }
    void Next() override {
        do {
            It->Next();
        } while (!It->IsDone() && It->GetCurrent()->GetColor() != TargetColor);
    }
};

// Декоратор по голодности
class CatHungerDecorator : public IteratorDecorator<Cat*> {
private:
    bool TargetHunger;
public:
    CatHungerDecorator(Iterator<Cat*>* it, bool isHungry)
        : IteratorDecorator<Cat*>(it), TargetHunger(isHungry) {}
    void First() override {
        It->First();
        while (!It->IsDone() && It->GetCurrent()->Hungry() != TargetHunger) {
            It->Next();
        }
    }
    void Next() override {
        do {
            It->Next();
        } while (!It->IsDone() && It->GetCurrent()->Hungry() != TargetHunger);
    }
};

// Декоратор по весу
class CatWeightDecorator : public IteratorDecorator<Cat*> {
private:
    double MinWeight;
public:
    CatWeightDecorator(Iterator<Cat*>* it, double weight)
        : IteratorDecorator<Cat*>(it), MinWeight(weight) {}
    void First() override {
        It->First();
        while (!It->IsDone() && It->GetCurrent()->GetWeight() < MinWeight) {
            It->Next();
        }
    }
    void Next() override {
        do {
            It->Next();
        } while (!It->IsDone() && It->GetCurrent()->GetWeight() < MinWeight);
    }
};

// Функция для кормления всех кошек через итератор
void FeedAll(Iterator<Cat*>* it) {
    for (it->First(); !it->IsDone(); it->Next()) {
        Cat* current = it->GetCurrent();
        current->Eat();
    }
}

int main() {
    setlocale(LC_ALL, "Russian");
    srand(static_cast<unsigned>(time(0)));
    size_t N = 20;

    // Массив кошек
    ArrayClass<Cat*> catArray;
    for (size_t i = 0; i < N; i++) {
        CatType catType = static_cast<CatType>(rand() % 3 + 1);
        catArray.Add(CreateCat(catType));
    }
    cout << "Массив кошек, размер: " << catArray.Size() << endl;

    // Список кошек
    list<Cat*> catList;
    for (size_t i = 0; i < N; i++) {
        CatType catType = static_cast<CatType>(rand() % 3 + 1);
        catList.push_back(CreateCat(catType));
    }
    cout << "Список кошек, размер: " << catList.size() << endl;

    // Все кошки
    cout << "\n--- Все кошки ---\n";
    FeedAll(catArray.GetIterator());

    // Все голодные
    cout << "\n--- Только голодные ---\n";
    FeedAll(new CatHungerDecorator(catArray.GetIterator(), true));

    // Только черные (BritishShorthair)
    cout << "\n--- Только черные кошки ---\n";
    FeedAll(new CatColorDecorator(catArray.GetIterator(), CatColor::Black));

    // Голодные серые кошки
    cout << "\n--- Голодные серые кошки ---\n";
    FeedAll(new CatHungerDecorator(
        new CatColorDecorator(catArray.GetIterator(), CatColor::Gray), true));

    // Итератор по списку
    cout << "\n--- Голодные белые кошки из списка ---\n";
    auto listIt = new ConstIteratorAdapter<list<Cat*>, Cat*>(&catList);
    FeedAll(new CatHungerDecorator(
        new CatColorDecorator(listIt, CatColor::White), true));

    // Кошки с весом больше 5.0
    cout << "\n--- Кошки с весом больше 5.0 ---\n";
    FeedAll(new CatWeightDecorator(catArray.GetIterator(), 5.0));

    return 0;
}
