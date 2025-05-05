#ifndef TIME_H
#define TIME_H

#include <string>
#include <stdexcept>

class Time {
private:
    int hours;    // Часы (0-23)
    int minutes;  // Минуты (0-59)

public:
    Time();
    Time(int h, int m);
    
    static Time fromString(const std::string& timeStr);
    std::string toString() const;
    
    bool operator<(const Time& other) const;
    bool operator<=(const Time& other) const;
    bool operator==(const Time& other) const;
    
    int toMinutes() const;
    
    Time operator+(const Time& other) const;
    Time operator-(const Time& other) const;
    
    void addMinutes(int m);
};

#endif 