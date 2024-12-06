/*
File: Protonify Agent 2.0.0 ActionON10V.cpp File for IDE 2.3.2
Date Created: Dec 5, 2024
Authors: DaPhisha
Description: ON10V Action Object 
Version: 1.0.0
*/

#include "ActionON10V.h"  // Include its own header file for declarations
#include "LogManager.h"

// Constructor
ActionON10V::ActionON10V(Ports* port, int priority, String msg)
    : ActionObject(port, priority, msg) {}

// Override the init method
void ActionON10V::init() {
    // Initialization logic for the test action
    // Example: pinMode(port->readPinNumber, INPUT);
}

// Override the execute method
void ActionON10V::execute() {
    if (port != nullptr) {

        //check to see if is an analog port
        if(port->pinType != ANALOG_OUTPUT){
          //LOG("ActionON10V - INCORRECT PIN TYPE FOR ON10V");
          strcpy(port->state, String("ERROR").c_str());
          return;
        }

        if(port->isSimulated == true){
            port->lastReading = port->currentReading = 1;
            strcpy(port->state, String("ON").c_str());
            port->lastUpdated = LogManager::getInstance().getCurrentTime(); 
            //LOG("ActionOFF10V - Port SIMULATED: " + String(port->pinDescription) + " PRI: " + priority + " MSG: " + msg);
            port->lastUpdated = LogManager::getInstance().getCurrentTime(); 
            return;
        }

        if(port->currentReading == 1){
          //do nothing as its already on
          return;
        }
        //else turn it on
        port->lastReading = 0;

        // Example logic for reading a digital pin
        //ADD VALIDATION CODE BASED ON THE PORT TYPE
        //analogWrite(port->writePinNumber, 0);
        MachineControl_AnalogOut.write(port->writePinNumber,255);
        strcpy(port->state, String("ON").c_str());
        port->currentReading = 1;
        LOG("ActionOFF10V - Port ACTUAL SENDING 10V: " + String(port->pinDescription) + " PRI: " + priority + " MSG: " + msg);
        port->lastUpdated = LogManager::getInstance().getCurrentTime(); 
    }
}

// Override the stop method
void ActionON10V::stop() {
    // Logic to stop the action if needed
}



