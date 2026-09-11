#ifndef CAT_H
#define CAT_H

#include "Animal.hpp"

class Cat : public Animal {
    public:
        Cat(int age, std::string name) : Animal(age, name) {}

    private:
        void makeSound() override;
};

#endif