#include <iostream>

#include "Dog.hpp"

void Dog::makeSound() {
    std::cout << "[" << this->m_name << "]: Wuff" << std::endl;
}