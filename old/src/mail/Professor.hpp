#ifndef PROFESSOR_H
#define PROFESSOR_H

#include <string>

#include "Person.hpp"

class Professor : public Person {
private:
    std::string m_iban;

public:
    Professor(std::string name, std::string address, std::string email);
};

#endif