#include "computer_club.h"
#include <iostream>
#include <algorithm>

ComputerClub::ComputerClub(int count, Time open, Time close, int cost)
    : tablesCount(count), openTime(open), closeTime(close), hourCost(cost) {
    for (int i = 1; i <= count; ++i) {
        tables[i] = "";
        tableRevenue[i] = 0;
        tableUsageTime[i] = Time(0, 0);
    }
}

void ComputerClub::processEvent(const Event& event) {
    std::cout << event.toString() << std::endl;

    switch (event.id) {
        case 1:
            handleClientArrived(event);
            break;
        case 2:
            handleClientSat(event);
            break;
        case 3:
            handleClientWaiting(event);
            break;
        case 4:
            handleClientLeft(event);
            break;
        default:
            break;
    }
}

void ComputerClub::handleEndOfDay() {
    std::vector<std::string> clientsLeft(clientsInClub.begin(), clientsInClub.end());
    std::sort(clientsLeft.begin(), clientsLeft.end());

    for (const auto& client : clientsLeft) {
        Event leaveEvent(closeTime, 11, client);
        std::cout << leaveEvent.toString() << std::endl;
        
        int table = clientToTable[client];
        if (table != 0) {
            calculateRevenue(table, closeTime);
            tables[table] = "";
        }
    }

    clientsInClub.clear();
    clientToTable.clear();
    waitingQueue = std::queue<std::string>();
}

void ComputerClub::printResults() const {
    std::cout << closeTime.toString() << std::endl;
    for (int i = 1; i <= tablesCount; ++i) {
        std::cout << i << " " << tableRevenue.at(i) << " " << tableUsageTime.at(i).toString() << std::endl;
    }
}

void ComputerClub::handleClientArrived(const Event& event) {
    if (event.time < openTime || closeTime <= event.time) {
        Event errorEvent(event.time, 13, "", 0, "NotOpenYet");
        std::cout << errorEvent.toString() << std::endl;
        return;
    }

    if (clientsInClub.find(event.clientName) != clientsInClub.end()) {
        Event errorEvent(event.time, 13, "", 0, "YouShallNotPass");
        std::cout << errorEvent.toString() << std::endl;
        return;
    }

    clientsInClub.insert(event.clientName);
}

void ComputerClub::handleClientSat(const Event& event) {
    if (clientsInClub.find(event.clientName) == clientsInClub.end()) {
        Event errorEvent(event.time, 13, "", 0, "ClientUnknown");
        std::cout << errorEvent.toString() << std::endl;
        return;
    }

    if (event.tableNumber < 1 || event.tableNumber > tablesCount) {
        Event errorEvent(event.time, 13, "", 0, "PlaceIsBusy");
        std::cout << errorEvent.toString() << std::endl;
        return;
    }

    if (!tables[event.tableNumber].empty() && tables[event.tableNumber] != event.clientName) {
        Event errorEvent(event.time, 13, "", 0, "PlaceIsBusy");
        std::cout << errorEvent.toString() << std::endl;
        return;
    }

    if (clientToTable.find(event.clientName) != clientToTable.end()) {
        int oldTable = clientToTable[event.clientName];
        if (oldTable != 0) {
            calculateRevenue(oldTable, event.time);
            tables[oldTable] = "";
        }
    }

    tables[event.tableNumber] = event.clientName;
    clientToTable[event.clientName] = event.tableNumber;
    tableStartTimes[event.tableNumber] = event.time;
}

void ComputerClub::handleClientWaiting(const Event& event) {
    if (clientsInClub.find(event.clientName) == clientsInClub.end()) {
        Event errorEvent(event.time, 13, "", 0, "ClientUnknown");
        std::cout << errorEvent.toString() << std::endl;
        return;
    }

    for (int i = 1; i <= tablesCount; ++i) {
        if (tables[i].empty()) {
            Event errorEvent(event.time, 13, "", 0, "ICanWaitNoLonger!");
            std::cout << errorEvent.toString() << std::endl;
            return;
        }
    }

    if (waitingQueue.size() >= tablesCount) {
        clientsInClub.erase(event.clientName);
        if (clientToTable.find(event.clientName) != clientToTable.end()) {
            int table = clientToTable[event.clientName];
            if (table != 0) {
                calculateRevenue(table, event.time);
                tables[table] = "";
            }
            clientToTable.erase(event.clientName);
        }
        Event leaveEvent(event.time, 11, event.clientName);
        std::cout << leaveEvent.toString() << std::endl;
        return;
    }

    waitingQueue.push(event.clientName);
}

void ComputerClub::handleClientLeft(const Event& event) {
    if (clientsInClub.find(event.clientName) == clientsInClub.end()) {
        Event errorEvent(event.time, 13, "", 0, "ClientUnknown");
        std::cout << errorEvent.toString() << std::endl;
        return;
    }

    clientsInClub.erase(event.clientName);
    int table = clientToTable[event.clientName];
    clientToTable.erase(event.clientName);

    if (table != 0) {
        calculateRevenue(table, event.time);
        tables[table] = "";

        if (!waitingQueue.empty()) {
            std::string nextClient = waitingQueue.front();
            waitingQueue.pop();

            tables[table] = nextClient;
            clientToTable[nextClient] = table;
            tableStartTimes[table] = event.time;

            Event sitEvent(event.time, 12, nextClient, table);
            std::cout << sitEvent.toString() << std::endl;
        }
    }
}

void ComputerClub::calculateRevenue(int table, const Time& endTime) {
    Time startTime = tableStartTimes[table];
    Time duration = endTime - startTime;

    int hours = duration.toMinutes() / 60;
    if (duration.toMinutes() % 60 != 0) {
        hours++;
    }

    tableRevenue[table] += hours * hourCost;
    tableUsageTime[table] = tableUsageTime[table] + duration;
}