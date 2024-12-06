/*
File: Protonify Agent 2.0.0 ActionONOFF.cpp File for IDE 2.3.2
Date Created: Sept 5 29, 2024
Authors: DaPhisha
Description: ONOFF Action Object 
Version: 1.0.0
*/

#include "ActionONOFF.h"  // Include its own header file for declarations
#include "LogManager.h"

// Constructor
ActionONOFF::ActionONOFF(Ports* port, int priority, String msg)
    : ActionObject(port, priority, msg) {}

// Override the init method
void ActionONOFF::init() {
    // Initialization logic for the test action
    // Example: pinMode(port->readPinNumber, INPUT);
}

// Override the execute method
void ActionONOFF::execute() {
    if (port != nullptr) {
        // Example logic for reading a digital pin
        //ADD VALIDATION CODE BASED ON THE PORT TYPE
        
        if(port->isSimulated == true){
            
            port->lastReading = port->currentReading;
            
            //SIMULATION CODE
            port->currentReading = port->currentReading == 0 ? 1 : 0;

            if(port->currentReading == 0){

              strcpy(port->state, String("OFF").c_str());
        
            }else{
              strcpy(port->state, String("ON").c_str());
            }
            //LOG("ActionTest - Port SIMULATED: " + String(port->pinDescription) + " PRI: " + priority + " MSG: " + msg);
            port->lastUpdated = LogManager::getInstance().getCurrentTime();
            return;
        }
        LOG("ActionONOFF - Port ACTUAL NOT IMPLEMENTED: " + String(port->pinDescription) + " PRI: " + priority + " MSG: " + msg);
        //port->lastUpdated = LogManager::getInstance().getCurrentTime();
    }
}

// Override the stop method
void ActionONOFF::stop() {
    // Logic to stop the action if needed
}



