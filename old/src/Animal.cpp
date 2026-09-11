#include <iostream>

#include "Animal.hpp"

void Animal::introduceYourself() {
    this->makeSound();
    std::cout << "I am " << this->m_name << " and I am " << this->m_age << " years old." << std::endl;
}