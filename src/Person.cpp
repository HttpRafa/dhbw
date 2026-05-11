#include "Person.hpp"

Person::Person(std::string name, std::string address, std::string email): m_name(name), m_address(address), m_email(email) {}

std::string Person::getName() {
    return this->m_name;
}