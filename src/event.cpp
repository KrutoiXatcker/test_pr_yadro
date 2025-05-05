#include "event.h"

Event::Event(Time t, int i, const std::string& name, int table, const std::string& error)
    : time(t), id(i), clientName(name), tableNumber(table), errorMsg(error) {}

std::string Event::toString() const {
    std::ostringstream oss;
    oss << time.toString() << " " << id;
    if (!clientName.empty()) {
        oss << " " << clientName;
    }
    if (tableNumber != 0) {
        oss << " " << tableNumber;
    }
    if (!errorMsg.empty()) {
        oss << " " << errorMsg;
    }
    return oss.str();
}