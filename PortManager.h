/*
File: PortManager.h
Date Created: April 29, 2024
Authors: DaPhisha
Description: Defines the PortManager class which manages an array of Port objects. It handles loading and saving port configurations to flash memory, and ensures robust memory management and error handling.
Version: 1.0.0
*/

#ifndef PORT_MANAGER_H
#define PORT_MANAGER_H
#include <Arduino_PortentaMachineControl.h>
#include <Arduino.h>
#include "FlashAPILimits.h"
#define MAX_PORTS 56 //Portenta Machine Control
#define MAX_LOG_ENTRIES 100
#define MAX_LOG_STRING_CHAR 256
#define MAX_USERNAME 32
#define MAX_SSID 32
#define MAX_PASSWORD 32
#define MAX_SERIAL 26
#define MAX_MODEL 32
#define MAX_SHARED_SECRET 32
#define MAX_CALL_HOME_URL 128
#define MAX_SIGNATURE 16
constexpr int MAX_DESCRIPTION = 16;
constexpr int MAX_STATE_TXT = 32;
//*******************************************************************
//*******************************************************************
//*******************************************************************
const char DATA_SIGNATURE[MAX_SIGNATURE] = "DAPHISH05252024";

enum CIRCUIT_TYPE {
    NOT_ASSIGNED, ONOFF, MA420, CTEMP, VALVE, FILL, PULSE, CIRCUIT_TYPE_COUNT
};

enum PIN_TYPE {
    ANALOG_INPUT, ANALOG_OUTPUT, DIGITAL_INPUT, DIGITAL_OUTPUT, PROGRAMMABLE_IO, PTEMP, HMI, ENCODER, PIN_TYPE_COUNT
};


//constexpr uint8_t DATA_SIGNATURE_UINT8[] = {0xAB, 0xCD, 0xEF, 0x01};
struct Ports{
  int readPinNumber;
  int writePinNumber;
  PIN_TYPE pinType;
  CIRCUIT_TYPE circuitType;
  char pinDescription[MAX_DESCRIPTION + 1]; // Includes space for null terminator
  float currentReading;
  float lastReading;
  char state[MAX_STATE_TXT + 1]; // Includes space for null terminator
  bool isActive;
  bool isSimulated;
  time_t lastUpdated;
};

struct AdminSettings {
        char Admin_USERNAME[MAX_USERNAME+1];//plus end of line
        char Admin_PASSWORD[MAX_PASSWORD+1];//plus end of line
        char WIFI_SSID[MAX_SSID+1];//plus end of line
        char WIFI_PASSWORD[MAX_PASSWORD+1];//plus end of line
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
        Ports ports[MAX_PORTS];
    };

class PortManager {
private:
    int numPorts; // Number of ports managed
    bool isInitialized;
    size_t getDataSizeWithSignature() const;
    String OnePortToString(int i);
    //calculate the total size of the AdminSettings Struct pls signature
    size_t getDataSizeWithSignature();
    void loadPortDefaults();
    void setPortValues(int indexPort, bool isActive, bool isSimulated, int readPinNum, int writePinNum, PIN_TYPE pinType, CIRCUIT_TYPE circuitType, String pinDescription, float curReading, float lastReading, time_t currentTime, String stateStr);
public:
    AdminSettings settings; // Includes all settings and ports array
    PortManager();
    ~PortManager();
    void init(); 
    static PortManager& getInstance();
    String toString();
    String toHTML();
    bool loadFromFlash();
    void initializeDefaults();
    void writeToFlash();
    String AllPortsToString();
    String pinTypeToString(PIN_TYPE type);
    String circuitTypeToString(CIRCUIT_TYPE type);
    int getActivePortCount();
    String ipToString(const uint8_t ip[4]);
    bool stringToIP(const String &ipString, uint8_t ip[4]);
    bool stringToMAC(const String &macString, byte mac[6]);
    String macToString(const byte mac[6]);
    String timeToString(time_t t);
};

#endif // PORT_MANAGER_H
