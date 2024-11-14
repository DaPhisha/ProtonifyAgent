/*
File: Protonify Agent 2.0.0 ActionCTEMP.h File for IDE 2.3.2
Date Created: Sept 5 29, 2024
Authors: DaPhisha
Description: CTEMP Action Object 
Version: 1.0.0
*/
// ActionCTEMP.h
#ifndef ACTION_CTEMP_H
#define ACTION_CTEMP_H

#include "ActionObject.h"  // Base class
#include "PortManager.h"   // Include PortManager to access Ports structure
#include <Arduino.h>       // Arduino functions and types

// Derived class for handling CTEMP actions
class ActionCTEMP : public ActionObject {
private:
   // String msg;  // Custom message to be used in logging

public:
    // Constructor to initialize with a port, priority, and message
    ActionCTEMP(Ports* port, int priority, String msg);

    // Override methods
    void init() override;
    void execute() override;
    void stop() override;
};

#endif // ACTION_CTEMP_H
