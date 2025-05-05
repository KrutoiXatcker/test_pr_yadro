#ifndef EVENT_H
#define EVENT_H

#include <string>
#include <sstream>
#include "time.h" 

struct Event {
    Time time;              // Время события
    int id;                 // Идентификатор события
    std::string clientName; // Имя клиента
    int tableNumber;        // Номер стола (если применимо)
    std::string errorMsg;   // Сообщение об ошибке (если применимо)

    Event(Time t, int i, const std::string& name = "", int table = 0, const std::string& error = "");
    
    std::string toString() const;
};

#endif