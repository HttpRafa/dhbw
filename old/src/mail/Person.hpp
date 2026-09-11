#ifndef PERSON_H
#define PERSON_H

#include <string>

class Person {
protected:
    std::string m_name;
    std::string m_address;
    std::string m_email;

public:
    Person(std::string name, std::string address, std::string email);

    std::string getName();
};

#endif