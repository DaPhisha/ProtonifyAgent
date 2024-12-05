/*
File: Protonify Agent 2.0.0 ActionON24V.cpp File for IDE 2.3.2
Date Created: Dec 5, 2024
Authors: DaPhisha
Description: OFNAction Object 
Version: 1.0.0
*/

#include "ActionON24V.h"  // Include its own header file for declarations
#include "LogManager.h"

// Constructor
ActionON24V::ActionON24V(Ports* port, int priority, String msg)
    : ActionObject(port, priority, msg) {}

// Override the init method
void ActionON24V::init() {
    // Initialization logic for the test action
    // Example: pinMode(port->readPinNumber, INPUT);
}

// Override the execute method
void ActionON24V::execute() {
    if (port != nullptr) {

        if(port->isSimulated == true){
            port->lastReading = port->currentReading;
            strcpy(port->state, String("ON").c_str());
            port->lastUpdated = LogManager::getInstance().getCurrentTime(); 
            //LOG("ActionON24V - Port SIMULATED: " + String(port->pinDescription) + " PRI: " + priority + " MSG: " + msg);
            port->lastUpdated = LogManager::getInstance().getCurrentTime(); 
            return;
        }

        // Example logic for reading a digital pin
        //ADD VALIDATION CODE BASED ON THE PORT TYPE
        analogWrite(port->writePinNumber, 255);
        strcpy(port->state, String("ON").c_str());
        port->lastReading = port->currentReading;
        //LOG("ActionON24V - Port ACTUAL SENDING 10V: " + String(port->pinDescription) + " PRI: " + priority + " MSG: " + msg);
        port->lastUpdated = LogManager::getInstance().getCurrentTime(); 
    }
}

// Override the stop method
void ActionON24V::stop() {
    // Logic to stop the action if needed
}



