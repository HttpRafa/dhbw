#include <vector>

#include "Animal.hpp"
#include "Dog.hpp"
#include "Cat.hpp"

int main() {
    Dog* levy = new Dog(5, "Levy");
    Cat* some = new Cat(4, "Peter");

    std::vector<Animal*> animals = {levy, some};

    for(Animal* animal : animals) {
        animal->introduceYourself();
    }

    return 0;
}