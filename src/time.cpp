#include "time.h"
#include <iomanip>
#include <sstream>

Time::Time() : hours(0), minutes(0) {}
Time::Time(int h, int m) : hours(h), minutes(m) {}

Time Time::fromString(const std::string& timeStr) {
    size_t colonPos = timeStr.find(':');
    if (colonPos == std::string::npos || colonPos == 0 || colonPos == timeStr.length() - 1) {
        throw std::invalid_argument("Invalid time format");
    }

    int h = std::stoi(timeStr.substr(0, colonPos));
    int m = std::stoi(timeStr.substr(colonPos + 1));

    if (h < 0 || h > 23 || m < 0 || m > 59) {
        throw std::invalid_argument("Invalid time value");
    }

    return Time(h, m);
}

std::string Time::toString() const {
    std::ostringstream oss;
    oss << std::setw(2) << std::setfill('0') << hours << ":"
        << std::setw(2) << std::setfill('0') << minutes;
    return oss.str();
}

bool Time::operator<(const Time& other) const {
    if (hours != other.hours) {
        return hours < other.hours;
    }
    return minutes < other.minutes;
}

bool Time::operator<=(const Time& other) const {
    return !(other < *this);
}

bool Time::operator==(const Time& other) const {
    return hours == other.hours && minutes == other.minutes;
}

int Time::toMinutes() const {
    return hours * 60 + minutes;
}

Time Time::operator+(const Time& other) const {
    int totalMinutes = minutes + other.minutes;
    int carryHours = totalMinutes / 60;
    totalMinutes %= 60;

    int totalHours = hours + other.hours + carryHours;
    totalHours %= 24;

    return Time(totalHours, totalMinutes);
}

Time Time::operator-(const Time& other) const {
    int thisMinutes = toMinutes();
    int otherMinutes = other.toMinutes();

    if (thisMinutes < otherMinutes) {
        thisMinutes += 24 * 60;
    }

    int diffMinutes = thisMinutes - otherMinutes;
    return Time(diffMinutes / 60, diffMinutes % 60);
}

void Time::addMinutes(int m) {
    minutes += m;
    hours += minutes / 60;
    minutes %= 60;
    hours %= 24;
}