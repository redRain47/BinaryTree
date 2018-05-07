#include "stdafx.h"
#include "Student.h"
#include <string>

void Student::setSurname(std::basic_string<TCHAR> str)
{
    _surname = str;
}

void Student::setName(std::basic_string<TCHAR> str)
{
    _name = str;
}

void Student::setPatronymic(std::basic_string<TCHAR> str)
{
    _patronymic = str;
}

void Student::setYearOfBirth(int y)
{
    _yearOfBirth = y;
}

void Student::setAverageMark(double a)
{
    _averageMark = a;
}

std::basic_string<TCHAR> 
Student::getSurname() const
{
    return _surname;
}

std::basic_string<TCHAR> 
Student::getName() const
{
    return _name;
}

std::basic_string<TCHAR> 
Student::getPatronymic() const
{
    return _patronymic;
}

unsigned short int  Student::getYearOfBirth() const
{
    return _yearOfBirth;
}

float Student::getAverageMark() const
{
    return _averageMark;
}

Student& Student::operator=(Student& right)
{
    _surname = right._surname;
    _name = right._name;
    _patronymic = right._patronymic;
    _yearOfBirth = right._yearOfBirth;
    _averageMark = right._averageMark;
    return *this;
}

bool Student::operator==(Student& right)
{
    return (compareStudents(*this, right) == 0);
}

bool Student::operator!=(Student& right)
{
    return (compareStudents(*this, right) != 0);
}

bool Student::operator>(Student& right)
{
    return (compareStudents(*this, right) > 0);
}

bool Student::operator<(Student& right)
{
    return (compareStudents(*this, right) < 0);
}

bool Student::operator>=(Student& right)
{
    return (compareStudents(*this, right) >= 0);
}

bool Student::operator<=(Student& right)
{
    return (compareStudents(*this, right) <= 0);
}

short int Student::compareStudents(Student t1, Student t2)
{
    short int result = t1.getSurname().compare(t2.getSurname());
    if (result == 0)
    {
        result = t1.getName().compare(t2.getName());
        if (result == 0)
        {
            return (t1.getPatronymic().compare(t2.getPatronymic()));
        }
    }

    return result;
}