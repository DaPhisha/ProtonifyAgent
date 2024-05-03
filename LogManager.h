/*
File: LogManager.h
  Date Created: April 24, 2024
  Authors: DaPhisha
  Description: Logging Handler
  Version: 1.0.0
*/
#ifndef LOGMANAGER_H
#define LOGMANAGER_H

#include <Arduino.h>
#include <time.h>
#include <stdexcept>  // Include for std::runtime_error

#define LOG(message) LogManager::getInstance().writeLog(message)


class LogManager {
public:
    void init();
    void writeLog(String message);
    static LogManager& getInstance();
    String timeToString(time_t t);
private:
    LogManager() {} // Constructor is private for singleton pattern
};

#endif // LOGMANAGER_H