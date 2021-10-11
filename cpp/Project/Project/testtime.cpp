#define _CRT_SECURE_NO_WARNINGS

#include <ctime>
#include <iostream>

// Time manipulation

// clock
// returns raw processor clock time since the program is started 
// (function)

// time 
// returns the current time of the system as time since epoch
// (function)

// difftime
// computes the difference between times
// (function)

// timespec_get(C++17)
// returns the calendar time based on a given time base
// (function)


// Format conversions

// ctime
// converts a std::time_t object to a textual representation
// (function)

// asctime
// converts a std::tm object to a textual representation
// (function)

// strftime
// converts a std::tm object to custom textual representation
// (function)

// wcsftime
// converts a std::tm object to custom wide string textual representation
// (function)

// gmtime
// converts time since epoch to calendar time expressed as Universal Coordinated Time
// (function)

// localtime
// converts time since epoch to calendar time expressed as local time
// (function)

// mktime
// converts calendar time to time since epoch
// (function)


int testtime()
{
    time_t tm1 = std::time(nullptr);
    clock_t cl1 = std::clock();
    char* pc = std::ctime(&tm1);

    std::cout << "time:\t" << tm1 << std::endl;
    std::cout << "clock:\t" << cl1 << std::endl;
    std::cout << "ctime:\t" << pc << std::endl;

    char buffer[80];
    wchar_t wcbuffer[80];
    // 本地时间
    std::cout << "local " << std::endl;
    std::tm* plocal = std::localtime(&tm1);
    time_t mk1 = std::mktime(plocal);
    std::cout << "mktime:\t" << mk1 << std::endl;
    std::strftime(buffer, 80, "%Y-%m-%d %H:%M:%S", plocal);
    std::cout << "strftime:\t" << buffer << std::endl;
    std::wcsftime(wcbuffer, 80, L"%Y-%m-%d %H:%M:%S", plocal);
    std::wcout << L"wcsftime:\t" << wcbuffer << std::endl;
    char* asc = std::asctime(plocal);
    std::cout << "asctime:\t" << asc << std::endl;


    // 通用时间
    std::cout << "Universal Coordinated" << std::endl;
    std::tm* ptime = std::gmtime(&tm1);
    time_t mk2 = std::mktime(ptime);
    std::cout << "mktime:\t" << mk2 << std::endl;
    std::strftime(buffer, 80, "%Y-%m-%d %H:%M:%S", ptime);
    std::cout << "strftime:\t" << buffer << std::endl;
    std::wcsftime(wcbuffer, 80, L"%Y-%m-%d %H:%M:%S", ptime);
    std::wcout << L"wcsftime:\t" << wcbuffer << std::endl;
    char* asctime = std::asctime(ptime);
    std::cout << "asctime:\t" << asctime << std::endl;

    // c++17
    // windows下timespec不在std里
    timespec ts;
    timespec_get(&ts, TIME_UTC);
    std::strftime(buffer, sizeof(buffer), "%D %T", std::gmtime(&ts.tv_sec));
    std::cout << "Current time:\t" << buffer << '.' << ts.tv_nsec << " UTC" << std::endl;
    
    time_t tm2 = std::time(nullptr);
    double d = std::difftime(tm2, tm1);
    std::cout << "Wall time passed:\t " << d << " s." << std::endl;
    return 0;
}