#pragma once

#include <string>

class Animal {
    protected:
        int m_age;
        std::string m_name;

    public:
        Animal(int age, std::string name) : m_age(age), m_name(name) {}

        void introduceYourself();

    private:
        virtual void makeSound() = 0;
};