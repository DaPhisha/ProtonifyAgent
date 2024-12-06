/*
File: Protonify Agent 2.0.0 ActionOFF10V.cpp File for IDE 2.3.2
Date Created: Dec 5, 2024
Authors: DaPhisha
Description: OFF10V Action Object 
Version: 1.0.0
*/

#include "ActionOFF10V.h"  // Include its own header file for declarations
#include "LogManager.h"

// Constructor
ActionOFF10V::ActionOFF10V(Ports* port, int priority, String msg)
    : ActionObject(port, priority, msg) {}

// Override the init method
void ActionOFF10V::init() {
    // Initialization logic for the test action
    // Example: pinMode(port->readPinNumber, INPUT);
}

// Override the execute method
void ActionOFF10V::execute() {
    if (port != nullptr) {

        //check to see if is an analog port
        if(port->pinType != ANALOG_OUTPUT){
          //LOG("ActionOFF10V - INCORRECT PIN TYPE FOR OFF10V");
          strcpy(port->state, String("ERROR").c_str());
          return;
        }

        if(port->currentReading == 0){
          //do nothing as its already on
          return;
        }

        if(port->isSimulated == true){
            port->currentReading = 0;
            strcpy(port->state, String("OFF").c_str());
            port->lastUpdated = LogManager::getInstance().getCurrentTime(); 
            LOG("ActionOFF10V - Port SIMULATED: " + String(port->pinDescription) + " PRI: " + priority + " MSG: " + msg);
            port->lastUpdated = LogManager::getInstance().getCurrentTime(); 
            return;
        }
        //else turn it off
        port->lastReading = 1;

        // Example logic for reading a digital pin
        //ADD VALIDATION CODE BASED ON THE PORT TYPE
        MachineControl_AnalogOut.write(port->writePinNumber,0);
        strcpy(port->state, String("OFF").c_str());
        port->currentReading = 0;
        LOG("ActionOFF10V - Port ACTUAL SENDING 0V: " + String(port->pinDescription) + " PRI: " + priority + " MSG: " + msg);
        port->lastUpdated = LogManager::getInstance().getCurrentTime(); 
    }
}

// Override the stop method
void ActionOFF10V::stop() {
    // Logic to stop the action if needed
}



