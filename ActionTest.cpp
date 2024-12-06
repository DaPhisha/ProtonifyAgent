/*
File: Protonify Agent 2.0.0 ActionTest.cpp File for IDE 2.3.2
Date Created: Sept 5 29, 2024
Authors: DaPhisha
Description: Action Object Test Object
Version: 1.0.0
*/
// ActionTest.cpp
#include "ActionTest.h"  // Include its own header file for declarations
#include "LogManager.h"

// Constructor
ActionTest::ActionTest(Ports* port, int priority, String msg)
    : ActionObject(port, priority, msg) {}

// Override the init method
void ActionTest::init() {
    // Initialization logic for the test action
    // Example: pinMode(port->readPinNumber, INPUT);
}

// Override the execute method
void ActionTest::execute() {
    if (port != nullptr) {
        // Example logic for reading a digital pin
        // int value = digitalRead(port->readPinNumber);
        // port->currentReading = value;
        // Serial.println("Read Digital Pin: " + String(port->readPinNumber) + " Value: " + String(value));
        if(port->isSimulated == true){
          //LOG("ActionTest - Port SIMULATED: " + String(port->pinDescription) + " PRI: " + priority + " MSG: " + msg);
          port->lastUpdated = LogManager::getInstance().getCurrentTime();
          return;
        }
        LOG("ActionTest - Port TEST: " + String(port->pinDescription) + " PRI: " + priority + " MSG: " + msg);
        port->lastUpdated = LogManager::getInstance().getCurrentTime();
    }
}

// Override the stop method
void ActionTest::stop() {
    // Logic to stop the action if needed
}

