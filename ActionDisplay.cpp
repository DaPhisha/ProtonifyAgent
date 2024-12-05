/*
File: Protonify Agent 2.0.0 ActionDislay.cpp File for IDE 2.3.3
Date Created: Sept 5 29, 2024
Authors: DaPhisha
Description: Action  Display Object
Version: 1.0.0
*/
// ActionTest.cpp
#include "ActionDisplay.h"  // Include its own header file for declarations
#include "LogManager.h"

// Constructor
ActionDisplay::ActionDisplay(Ports* port,int priority, String msg)
    : ActionObject(port, priority, msg) {}

// Override the init method
void ActionDisplay::init() {
    // Initialization logic for the Display action
}

// Override the execute method
void ActionDisplay::execute() {
  //LOG("ActionDisplay: PRI: " + String(priority) + " MSG: " + msg);
}

// Override the stop method
void ActionDisplay::stop() {
    // Logic to stop the action if needed
}