#pragma once

#include <tchar.h>
#include <string>
#include <iostream>

#ifdef UNICODE
#define stud_cout wcout
#define stud_cin wcin
#define stud_istream wistream
#define stud_ostream wostream
#endif 


class Student
{
public:
    Student()
    {
        _yearOfBirth = _averageMark = 0;
    }

    Student(Student &right)
    {
        *this = right;
    }

    ~Student()
    {
        _surname.clear();
        _name.clear();
        _patronymic.clear();
    }
    
    void setSurname(std::basic_string<TCHAR> str);

    void setName(std::basic_string<TCHAR> str);

    void setPatronymic(std::basic_string<TCHAR> str);

    void setYearOfBirth(int y);

    void setAverageMark(double a);

    std::basic_string<TCHAR> getSurname() const;
    
    std::basic_string<TCHAR> getName() const;

    std::basic_string<TCHAR> getPatronymic() const;

    unsigned short int  getYearOfBirth() const;

    float getAverageMark() const;

    Student& operator=(Student& right);

    bool operator==(Student& right);

    bool operator!=(Student& right);

    bool operator>(Student& right);

    bool operator<(Student& right);

    bool operator>=(Student& right);

    bool operator<=(Student& right);
    
    friend std::stud_ostream& operator<<(std::stud_ostream& os, Student& st)
    {
        os << st._surname << _TEXT(" ")
            << st._name << _TEXT(" ")
            << st._patronymic << _TEXT(" ")
            << st._yearOfBirth << _TEXT(" ")
            << st._averageMark;
        return os;
    }

    friend std::stud_istream& operator>>(std::stud_istream& os, Student& st)
    {
        os >> st._surname >> st._name 
            >> st._patronymic >> st._yearOfBirth >> st._averageMark;
       
        return os;
    }

private:
    std::basic_string<TCHAR> _surname;
    std::basic_string<TCHAR> _name;
    std::basic_string<TCHAR> _patronymic;
    unsigned short int _yearOfBirth;
    float _averageMark;

    short int compareStudents(Student t1, Student t2);
};