/*
File: Protonify Agent 2.0.0 ActionMA420.cpp File for IDE 2.3.2
Date Created: Sept 5, 2024
Authors: DaPhisha
Description: MA420 Action Object 
Version: 1.0.0
*/

#include "ActionMA420.h"  // Include its own header file for declarations
#include "LogManager.h"

// Constructor
ActionMA420::ActionMA420(Ports* port, int priority, String msg)
    : ActionObject(port, priority, msg) {}

// Override the init method
void ActionMA420::init() {
    // Initialization logic for MA420 action
    // Example: Setup analog input, sensor calibration, etc.
}

// Override the execute method
void ActionMA420::execute() {
    if (port != nullptr) {

        //ADD VALIDATION CODE BASED ON THE PORT TYPE
        if(port->isSimulated == true){
            //LOG("ActionMA420 - Port SIMULATED: " + String(port->pinDescription) + " PRI: " + String(priority) + " MSG: " + msg);

            port->lastReading = port->currentReading;

            // Simulation code for MA420 (e.g., simulating current levels in 4-20mA range)
            int randomValue = random(0, 3);  // Generate a random number between 0 and 2
            if (randomValue == 0) {
                port->currentReading = 0;
                strcpy(port->state, String("No Signal").c_str());
            } else if (randomValue == 1) {
                port->currentReading = 4;
                strcpy(port->state, String("4 mA").c_str());
            } else {
                port->currentReading = 20;
                strcpy(port->state, String("20 mA").c_str());
            }
            port->lastUpdated = LogManager::getInstance().getCurrentTime(); 
            return;
        }
        LOG("ActionMA420 - Port ACTUAL NOT IMPLEMETED: " + String(port->pinDescription) + " PRI: " + String(priority) + " MSG: " + msg);
        //port->lastUpdated = LogManager::getInstance().getCurrentTime(); 
    }
}

// Override the stop method
void ActionMA420::stop() {
    // Logic to stop the action if needed
    // For example, disable sensor reading or reset state
}
