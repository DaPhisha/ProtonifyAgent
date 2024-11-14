/*
File: Protonify Agent 2.0.0 ActionCTEMP.cpp File for IDE 2.3.2
Date Created: Sept 5 29, 2024
Authors: DaPhisha
Description: CTEMP Action Object 
Version: 1.0.0
*/
/*
File: Protonify Agent 2.0.0 ActionCTEMP.cpp File for IDE 2.3.2
Date Created: Sept 5, 2024
Authors: DaPhisha
Description: CTEMP Action Object 
Version: 1.0.0
*/

#include "ActionCTEMP.h"  // Include its own header file for declarations
#include "LogManager.h"

// Constructor
ActionCTEMP::ActionCTEMP(Ports* port, int priority, String msg)
    : ActionObject(port, priority,msg) {}

// Override the init method
void ActionCTEMP::init() {
    // Initialization logic for CTEMP action
    // Example: Setup temperature sensor, calibration, etc.
}

// Override the execute method
void ActionCTEMP::execute() {
    if (port != nullptr) {
        LOG("ActionCTEMP - Port SIMULATED: " + String(port->pinDescription) + " PRI: " + String(priority) + " MSG: " + msg);

        port->lastReading = port->currentReading;

        // Enhanced temperature simulation: Add a small random change to simulate real-world fluctuations
        int tempChange = random(-5, 6);  // Small change between -5 and 5
        port->currentReading += tempChange;

        // Ensure the simulated temperature stays within a realistic range
        if (port->currentReading < -40) {
            port->currentReading = -40;  // Lower limit
        } else if (port->currentReading > 100) {
            port->currentReading = 100;  // Upper limit
        }

        // Determine the state based on the last and current readings
        if (port->lastReading < port->currentReading) {
            strcpy(port->state, "Warming");
        } else if (port->lastReading > port->currentReading) {
            strcpy(port->state, "Cooling");
        } else {
            strcpy(port->state, "Constant");
        }

        port->lastUpdated = LogManager::getInstance().getCurrentTime();
    }
}

// Override the stop method
void ActionCTEMP::stop() {
    // Logic to stop the action if needed
    // For example, turn off the temperature sensor or reset state
}
