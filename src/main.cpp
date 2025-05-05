#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <queue>
#include <map>
#include <set>
#include <regex>

#include "time.h"
#include "event.h"
#include "computer_club.h"


// Функция для разделения строки по разделителю
std::vector<std::string> split(const std::string& s, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}


// Проверка корректности имени клиента
bool isValidClientName(const std::string& name) {
    static const std::regex pattern("^[a-zA-Z0-9_-]+$");
    return std::regex_match(name, pattern);
}


int main(int argc, char* argv[]) {
   
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <input_file>" << std::endl;
        return 1;
    }

    // Инпут файла 
    std::ifstream inputFile(argv[1]);
    if (!inputFile.is_open()) {
        std::cerr << "Error opening file: " << argv[1] << std::endl;
        return 1;
    }

    std::string line;
    int lineNumber = 0;
    int tablesCount = 0;
    Time openTime, closeTime;
    int hourCost = 0;
    std::vector<Event> events;

    try {
        // Чтение количества столов
        if (!std::getline(inputFile, line)) {
            throw std::runtime_error("Missing number of tables");
        }
        lineNumber++;
        tablesCount = std::stoi(line);
        if (tablesCount <= 0) {
            throw std::runtime_error("Invalid number of tables");
        }

        // Чтение времени работы
        if (!std::getline(inputFile, line)) {
            throw std::runtime_error("Missing working hours");
        }
        lineNumber++;
        auto times = split(line, ' ');
        if (times.size() != 2) {
            throw std::runtime_error("Invalid working hours format");
        }
        openTime = Time::fromString(times[0]);
        closeTime = Time::fromString(times[1]);
        if (closeTime <= openTime) {
            throw std::runtime_error("Close time must be after open time");
        }

        // Чтение стоимости часа
        if (!std::getline(inputFile, line)) {
            throw std::runtime_error("Missing hour cost");
        }
        lineNumber++;
        hourCost = std::stoi(line);
        if (hourCost <= 0) {
            throw std::runtime_error("Invalid hour cost");
        }

        // Чтение событий
        while (std::getline(inputFile, line)) {
            lineNumber++;
            if (line.empty()) continue;

            auto parts = split(line, ' ');
            if (parts.size() < 2) {
                throw std::runtime_error("Invalid event format");
            }

            Time time = Time::fromString(parts[0]);
            int id = std::stoi(parts[1]);

            // Обработка разных типов событий
            switch (id) {
                case 1: // Клиент пришел
                    if (parts.size() != 3) {
                        throw std::runtime_error("Invalid client arrived event");
                    }
                    if (!isValidClientName(parts[2])) {
                        throw std::runtime_error("Invalid client name");
                    }
                    events.emplace_back(time, id, parts[2]);
                    break;
                    
                case 2: // Клиент сел за стол
                    if (parts.size() != 4) {
                        throw std::runtime_error("Invalid client sat event");
                    }
                    if (!isValidClientName(parts[2])) {
                        throw std::runtime_error("Invalid client name");
                    }
                    {
                        int table = std::stoi(parts[3]);
                        if (table < 1 || table > tablesCount) {
                            throw std::runtime_error("Invalid table number");
                        }
                        events.emplace_back(time, id, parts[2], table);
                    }
                    break;
                    
                case 3: // Клиент ожидает
                    if (parts.size() != 3) {
                        throw std::runtime_error("Invalid client waiting event");
                    }
                    if (!isValidClientName(parts[2])) {
                        throw std::runtime_error("Invalid client name");
                    }
                    events.emplace_back(time, id, parts[2]);
                    break;
                    
                case 4: // Клиент ушел
                    if (parts.size() != 3) {
                        throw std::runtime_error("Invalid client left event");
                    }
                    if (!isValidClientName(parts[2])) {
                        throw std::runtime_error("Invalid client name");
                    }
                    events.emplace_back(time, id, parts[2]);
                    break;
                    
                default:
                    throw std::runtime_error("Unknown event ID");
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Error on line " << lineNumber << ": " << e.what() << std::endl;
        return 1;
    }

    
    ComputerClub club(tablesCount, openTime, closeTime, hourCost);
    std::cout << openTime.toString() << std::endl;

    // Обработка всех событий
    for (const auto& event : events) {
        club.processEvent(event);
    }

    // Завершение рабочего дня и вывод результатов
    club.handleEndOfDay();
    club.printResults();

    return 0;
}