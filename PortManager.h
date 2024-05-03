/*
File: PortManager.h
Date Created: April 29, 2024
Authors: DaPhisha
Description: Defines the PortManager class which manages an array of Port objects. It handles loading and saving port configurations to flash memory, and ensures robust memory management and error handling.
Version: 1.0.0
*/

#ifndef PORT_MANAGER_H
#define PORT_MANAGER_H

#include "Port.h"
#define MAX_PORTS 10
#define MAX_LOG_ENTRIES 100
#define MAX_LOG_STRING_CHAR 256
#define MAX_USERNAME 32
#define MAX_PASSWORD 32
#define MAX_SERIAL 26
#define MAX_MODEL 32
#define MAX_SHARED_SECRET 32
#define MAX_CALL_HOME_URL 128
#define MAX_SIGNATURE 16
//*******************************************************************
//*******************************************************************
//*******************************************************************
const char DATA_SIGNATURE[MAX_SIGNATURE] = "DAPHISH04302024";
constexpr uint8_t DATA_SIGNATURE_UINT8[] = {0xAB, 0xCD, 0xEF, 0x01};


struct AdminSettings {
        char Admin_USERNAME[MAX_USERNAME+1];//plus end of line
        char Admin_PASSWORD[MAX_PASSWORD+1];//plus end of line
        uint8_t IP_ADDRESS[4];//always four no end of line
        uint8_t GATEWAY[4];//always four no end of line
        uint8_t DNS[4];//always four no end of line
        uint8_t SUBNET[4];//always four no end of line
        byte MAC[6];//always six no end of line
        char SERIAL_NUMBER[MAX_SERIAL+1]; //plus end of line
        char MODEL[MAX_MODEL+1];
        uint32_t REFRESH_RATE;
        char SHARED_SECRET[MAX_SHARED_SECRET + 1];//32 plus end of line
        char CALL_HOME_URL[MAX_CALL_HOME_URL +1];//128 plus end of line
        bool REGISTRATION_STATUS;
        char logEntries[MAX_LOG_ENTRIES][MAX_LOG_STRING_CHAR];
        size_t currentLogIndex;
        time_t DATE_LAST_UPDATED;
        time_t DATE_LAST_REBOOT;
        Port * ports[MAX_PORTS];
    };

class PortManager {
private:
    AdminSettings settings; // Includes all settings and ports array
    int numPorts; // Number of ports managed
    bool isInitialized;

public:
    PortManager();
    ~PortManager();
    void init(); 
    static PortManager& getInstance();
    String toString();
    String toHTML();
    bool loadFromFlash();
    void initializeDefaults();
    void writeToFlash();
};

#endif // PORT_MANAGER_H
