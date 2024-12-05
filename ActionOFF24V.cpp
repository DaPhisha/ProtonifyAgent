/*
File: Protonify Agent 2.0.0 ActionOFF24V.cpp File for IDE 2.3.2
Date Created: Dec 5, 2024
Authors: DaPhisha
Description: OFF24V Action Object 
Version: 1.0.0
*/

#include "ActionOFF24V.h"  // Include its own header file for declarations
#include "LogManager.h"

// Constructor
ActionOFF24V::ActionOFF24V(Ports* port, int priority, String msg)
    : ActionObject(port, priority, msg) {}

// Override the init method
void ActionOFF24V::init() {
    // Initialization logic for the test action
    // Example: pinMode(port->readPinNumber, INPUT);
}

// Override the execute method
void ActionOFF24V::execute() {
    if (port != nullptr) {

        if(port->isSimulated == true){
            port->lastReading = port->currentReading;
            strcpy(port->state, String("OFF").c_str());
            port->lastUpdated = LogManager::getInstance().getCurrentTime(); 
            //LOG("ActionOFF24V - Port SIMULATED: " + String(port->pinDescription) + " PRI: " + priority + " MSG: " + msg);
            port->lastUpdated = LogManager::getInstance().getCurrentTime(); 
            return;
        }

        // Example logic for reading a digital pin
        //ADD VALIDATION CODE BASED ON THE PORT TYPE
        analogWrite(port->writePinNumber, 0);
        strcpy(port->state, String("OFF").c_str());
        port->lastReading = port->currentReading;
        //LOG("ActionOFF24V - Port ACTUAL SENDING 0V: " + String(port->pinDescription) + " PRI: " + priority + " MSG: " + msg);
        port->lastUpdated = LogManager::getInstance().getCurrentTime(); 
    }
}

// Override the stop method
void ActionOFF24V::stop() {
    // Logic to stop the action if needed
}



