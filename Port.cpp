/*
File: Port.cpp
Date Created: April 28, 2024
Authors: DaPhisha
Description: Implementation of the Port class. This file provides functionalities for initializing ports,
reading from and writing to ports based on their type, and managing port state.
Version: 1.0.0
*/


#include "Port.h"
#include "LogManager.h"

//default constructor takes a port #, pin #, PIN_TYPE, CIRCUIT_TYPE, description and active status

Port::Port(int port, int pin, PIN_TYPE pType, CIRCUIT_TYPE cType, const char* description, bool active) :
    portNumber(port), pinNumber(pin), pinType(pType), circuitType(cType), isActive(active), currentReading(0.0),
    lastReading(0.0), lastUpdated(0) {
    strncpy(pinDescription, description, MAX_DESCRIPTION);
    pinDescription[MAX_DESCRIPTION] = '\0'; // Ensure null termination
    state[0] = '\0'; // Initialize state to empty
}

//intialization for the port.  Best practice to setup the port safely
void Port::initialize() {
    isActive = true;
}
void Port::read() {
    if (!isActive) {
        throw std::runtime_error("Attempted to read from an inactive port.");
    }
    if (pinType != ANALOG_INPUT && pinType != DIGITAL_INPUT) {
        throw std::runtime_error("Attempted to read from a non-input port.");
    }
    // Simulate or actually read from hardware
    //currentReading = analogRead(pinNumber); // Placeholder for real reading logic
    lastUpdated = time(NULL);
    //updateState();
}

void Port::write(float value) {
    if (!isActive) {
        throw std::runtime_error("Attempted to write to an inactive port.");
    }
    if (pinType != ANALOG_OUTPUT && pinType != DIGITAL_OUTPUT) {
        throw std::runtime_error("Attempted to write to a non-output port.");
    }
    // Simulate or actually write to hardware
    //analogWrite(pinNumber, static_cast<int>(value)); // Placeholder for real writing logic
    lastUpdated = time(NULL);
    //updateState();
}

void Port::simulateReading() {
    // Optional: Implement simulation logic for testing purposes
    //currentReading += 1.0; // Simple simulation by incrementing reading
    //currentReading = random(0, 1024);
    //updateState();
}

void Port::serialize(uint8_t* dest) const {

    //NOT SURE SO NEED TO HAVE GPT VALIUDATE THIS
    int *destInt = (int*) dest;
    *destInt++ = portNumber;
    *destInt++ = pinNumber;
    *destInt++ = static_cast<int>(pinType);
    *destInt++ = static_cast<int>(circuitType);

    memcpy(destInt, pinDescription, MAX_DESCRIPTION + 1);
    dest += sizeof(int) * 4 + MAX_DESCRIPTION + 1;

    float *destFloat = (float*) dest;
    *destFloat++ = currentReading;
    *destFloat++ = lastReading;

    memcpy(destFloat, state, MAX_STATE_TXT + 1);
    dest += 2 * sizeof(float) + MAX_STATE_TXT + 1;

    long *destLong = (long*) dest;
    *destLong = lastUpdated;
}

void Port::deserialize(const uint8_t* src) {
    //NOT SURE SO NEED TO HAVE GPT VALIUDATE THIS
    int *srcInt = (int*) src;
    portNumber = *srcInt++;
    pinNumber = *srcInt++;
    pinType = static_cast<PIN_TYPE>(*srcInt++);
    circuitType = static_cast<CIRCUIT_TYPE>(*srcInt++);

    memcpy(pinDescription, srcInt, MAX_DESCRIPTION + 1);
    src += sizeof(int) * 4 + MAX_DESCRIPTION + 1;

    float *srcFloat = (float*) src;
    currentReading = *srcFloat++;
    lastReading = *srcFloat++;

    memcpy(state, srcFloat, MAX_STATE_TXT + 1);
    src += 2 * sizeof(float) + MAX_STATE_TXT + 1;

    long *srcLong = (long*) src;
    lastUpdated = *srcLong;
}
String Port::toString() {
    
    char buffer[256]; // Allocate a buffer for the string
    snprintf(buffer, sizeof(buffer), 
             "Port Number: %d, Pin Number: %d, Pin Type: %s, Circuit Type: %s, Description: %s, Current Reading: %.2f, Last Reading: %.2f, State: %s, Active: %s, Simulated: %s, Last Updated: %s",
             portNumber, 
             pinNumber, 
             pinTypeToString(pinType).c_str(), 
             circuitTypeToString(circuitType).c_str(), 
             pinDescription, 
             currentReading, 
             lastReading, 
             state, 
             isActive ? "Yes" : "No", 
             isSimulated ? "Yes" : "No", 
             LogManager::getInstance().timeToString(lastUpdated).c_str());

    return String(buffer);
}

String Port::toHTML(){
    String html = "<div class='port'>";
    html += "<h2>" + String(pinDescription) + " (Port " + String(portNumber) + ", Pin " + String(pinNumber) + ")</h2>";
    html += "<p>Pin Type: " + pinTypeToString(pinType) + "</p>";
    html += "<p>Ciruit Type: " + circuitTypeToString(circuitType) + "</p>";
    html += "<p>Current Reading: " + String(currentReading) + "</p>";
    html += "<p>Last Reading: " + String(lastReading) + "</p>";
    html += "<p>State: " + String(state) + "</p>";
    html += "<p>Active: " + String(isActive ? "Yes" : "No") + "</p>";
    html += "<p>Simulated: " + String(isSimulated ? "Yes" : "No") + "</p>";
    html += "<p>Last Updated: " + LogManager::getInstance().timeToString(lastUpdated) + "</p>";
    html += "</div>";
    return html;
}

//ANALOG_INPUT, ANALOG_OUTPUT, DIGITAL_INPUT, DIGITAL_OUTPUT, PROGRAMMABLE_IO, PTEMP, HMI, ENCODER, PIN_TYPE_COUNT
String Port::pinTypeToString(PIN_TYPE type){
  switch (type) {
        case ANALOG_INPUT: return "ANALOG INPUT";
        case ANALOG_OUTPUT: return "ANALOG_OUTPUT";
        case DIGITAL_INPUT: return "DIGITAL_INPUT";
        case DIGITAL_OUTPUT: return "DIGITAL_OUTPUT";
        case PROGRAMMABLE_IO: return "PROGRAMMABLE_IO";
        case PTEMP: return "PTEMP";
        case HMI: return "HMI";
        case ENCODER: return "ENCODER";
        default: return "UNKNOWN";
    }
}

//NOT_ASSIGNED, ONOFF, MA420, CTEMP, VALVE, FILL, PULSE, CIRCUIT_TYPE_COUNT
String Port::circuitTypeToString(CIRCUIT_TYPE type){
  switch (type) {
        case NOT_ASSIGNED: return "ON OFF 0-24V";
        case ONOFF: return "ON OFF 0-24V";
        case MA420: return "4-20 mA";
        case CTEMP: return "Temperature";
        case VALVE: return "Valve Reading";
        case FILL: return "Tank Fill Level";
        case PULSE: return "Pulse";
        default: return "UNKNOWN";
    }

}


