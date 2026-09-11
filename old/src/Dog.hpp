#ifndef DOG_H
#define DOG_H

#include "Animal.hpp"

class Dog : public Animal {
    public:
        Dog(int age, std::string name) : Animal(age, name) {}

    private:
        void makeSound() override;
};

#endif