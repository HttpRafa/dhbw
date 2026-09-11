#include <iostream>

#include "Cat.hpp"

void Cat::makeSound() {
    std::cout << "[" << this->m_name << "]: Meow" << std::endl;
}