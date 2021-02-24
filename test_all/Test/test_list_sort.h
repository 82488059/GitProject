#pragma once
#include<string>

class test_list_sort
{
public:
    test_list_sort(const std::string& v):m_value(v){}
    test_list_sort(const test_list_sort& r) :m_value(r.value()) {};
    test_list_sort& operator=(const test_list_sort& r){ 
        m_value = r.value();
        return *this;
    };

    const std::string& value() const { return m_value; }
    bool operator<(const test_list_sort& r) { return value() < r.value(); }
private:
    std::string m_value{ "" };
};

int test_sort();
