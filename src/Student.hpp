#ifndef STUDENT_H
#define STUDENT_H

#include <string>

#include "Person.hpp"

class Student : public Person {
private:
    double m_averageGrade;

public:
    Student(std::string name, std::string address, std::string email);
};

#endif