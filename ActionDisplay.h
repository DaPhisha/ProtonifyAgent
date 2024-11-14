/*
File: Protonify Agent 2.0.0 ActionDisplay.h File for IDE 2.3.2
Date Created: Sept 5 29, 2024
Authors: DaPhisha
Description: Display Action Object 
Version: 1.0.0
*/
#ifndef ACTION_DISPLAY_H
#define ACTION_DISPLAY_H

#include "ActionObject.h"  // Base class
#include "PortManager.h"   // Include PortManager to access Ports structure
#include <Arduino.h>       // Arduino functions and types

// Derived class for handling CTEMP actions
class ActionDisplay : public ActionObject {
private:
    //String msg;  

public:
    // Constructor to initialize with a port, priority, and message
    ActionDisplay(Ports* port, int priority, String msg);

    // Override methods
    void init() override;
    void execute() override;
    void stop() override;
};

#endif // ACTION_DISPLAY_H
