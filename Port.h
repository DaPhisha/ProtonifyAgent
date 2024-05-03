/*
File: Port.h
Date Created: April 28, 2024
Authors: DaPhisha
Description: Defines a class for managing various types of port interfaces on Arduino devices, supporting operations
such as read, write, and state management according to the port's type (analog/digital input/output).
Version: 1.0.0
*/

#ifndef PORT_H
#define PORT_H

#include <Arduino.h>
#include <time.h>

constexpr int MAX_DESCRIPTION = 16;
constexpr int MAX_STATE_TXT = 32;

enum CIRCUIT_TYPE {
    NOT_ASSIGNED, ONOFF, MA420, CTEMP, VALVE, FILL, PULSE, CIRCUIT_TYPE_COUNT
};

enum PIN_TYPE {
    ANALOG_INPUT, ANALOG_OUTPUT, DIGITAL_INPUT, DIGITAL_OUTPUT, PROGRAMMABLE_IO, PTEMP, HMI, ENCODER, PIN_TYPE_COUNT
};

class Port {
public:
    int portNumber;
    int pinNumber;
    PIN_TYPE pinType;
    CIRCUIT_TYPE circuitType;
    char pinDescription[MAX_DESCRIPTION + 1]; // Includes space for null terminator
    float currentReading;
    float lastReading;
    char state[MAX_STATE_TXT + 1]; // Includes space for null terminator
    bool isActive;
    bool isSimulated;
    time_t lastUpdated;

    Port(int port, int pin, PIN_TYPE pType, CIRCUIT_TYPE cType, const char* description, bool active);
    virtual ~Port() {}

    void initialize();
    void read();
    void write(float value);
    void simulateReading();
    void updateState();
    void serialize(uint8_t* dest) const;
    void deserialize(const uint8_t* src);
    String toString();
    String toHTML();
    String pinTypeToString(PIN_TYPE type);
    String circuitTypeToString(CIRCUIT_TYPE type);
};

#endif // PORT_H
