#ifndef COMPUTER_CLUB_H
#define COMPUTER_CLUB_H

#include <map>
#include <set>
#include <queue>
#include <string>
#include "time.h"
#include "event.h"

class ComputerClub {
private:
    int tablesCount;
    Time openTime;
    Time closeTime;
    int hourCost;
    std::map<int, std::string> tables;
    std::map<int, Time> tableStartTimes;
    std::map<int, int> tableRevenue;
    std::map<int, Time> tableUsageTime;
    std::set<std::string> clientsInClub;
    std::queue<std::string> waitingQueue;
    std::map<std::string, int> clientToTable;

    void handleClientArrived(const Event& event);
    void handleClientSat(const Event& event);
    void handleClientWaiting(const Event& event);
    void handleClientLeft(const Event& event);
    void calculateRevenue(int table, const Time& endTime);

public:
    ComputerClub(int count, Time open, Time close, int cost);
    void processEvent(const Event& event);
    void handleEndOfDay();
    void printResults() const;
};

#endif 