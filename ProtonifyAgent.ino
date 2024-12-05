/*
File: Protonify Agent 2.0.0 Main .ino File for IDE 2.3.2
Date Created: April 29, 2024
Authors: DaPhisha
Description: Agent for reporting port status as JSON to Mongo Server
Version: 2.0.0

Road Map -
Add Cards for import and export a confifuration.

Fix Wifi

Improve UX for Ports

Stick Ports into Meters or Vessels or Processes...

Define port logic rules on what ports can be assigned which circuit types.  CRITICAL FOR SAFETY

*/


#include <Arduino_PortentaMachineControl.h>
#include <Arduino.h>
//#include <Ethernet.h>
//#include <EthernetUdp.h>
#include <NTPClient.h>
#include <time.h>
#include "LogManager.h"
#include "EventScheduler.h"
#include "TaskCallbacks.h"
#include "PortManager.h"
//#include "NetworkManager.h"
#include "AdminServerManager.h"
#include "ActionQueue.h"
#include "ActionTest.h"
#include "ActionONOFF.h"
#include "ActionMA420.h"
#include "ActionCTEMP.h"
#include "ActionON24V.h"
#include "ActionOFF24V.h"
#include "ActionDisplay.h"
#define MAX_EVENTS 10  // Maximum number of scheduled events
#define MASTER_DEBUG true

const char* PROJECT_VERSION = "2.0.1"; // Project version
uint32_t m_loopCounter = 0;
// Initialize an event scheduler
EventScheduler scheduler(MAX_EVENTS); 
ActionQueue actionQueue;
// Initialize network interfaces
AdminServerManager m_webServer;

void setup() {
    LogManager& logManager = LogManager::getInstance(); // Obtain the global LogManager instance
    logManager.init(MASTER_DEBUG);  // Initialize LogManager (e.g., setting up Serial communication)
    try {
        // Task Scheduler: Add tasks with timeout and callback
        //scheduler.addTask(86400000, checkMemory);  // Check memory every 24 hours
        //scheduler.addTask(3600000, checkNetwork);  // Check network every hour
        scheduler.addTask(86400000, writeSettingsToFlash);  // Write settings to flash every 24 hours

        // Initialize PortManager
        PortManager& portManager = PortManager::getInstance(); 
        portManager.init();
        m_webServer.init();

        // Initialize Wire transmission -- MOVE to M4?
        Wire.begin();
        if (!MachineControl_DigitalInputs.begin()) {
          LOG("Failed to initialize the digital input GPIO expander!");
        }
    } catch (const std::exception& e) { 
        LogManager::getInstance().writeLog("*********MAJOR STARTUP FAULT************");
        LogManager::getInstance().writeLog("Caught std::exception: " + String(e.what()));
    } catch(...) {
        LogManager::getInstance().writeLog("*********MAJOR STARTUP FAULT************");
        LogManager::getInstance().writeLog("*********UNKNOWN************");
    }
    logManager.writeLog("System Initialization COMPLETE");
}


void loop() {
    try {
        m_loopCounter ++;
        LogManager::getInstance().updateTime();
        //check serial connection
        LogManager::getInstance().checkSerialConnection();
        // Main Loop (M7)
        scheduler.run();  // Run scheduled tasks
        m_webServer.handleClient();  // Process any client requests

        // Update ports
        PortManager& portManager = PortManager::getInstance();
        //update all ports every REFRESH cycle
        //Populate Queue
        populateActionQueue();
        //Prioritize queue by highest priority value (highest to lowest
        actionQueue.sortActions(); 
        //Execute Action Queue
        actionQueue.executeActions();

        // Check if the refresh interval has passed
        if (m_loopCounter >= portManager.settings.REFRESH_RATE ) {

            // Check to see if the Arduino is registered
            if (portManager.settings.REGISTRATION_STATUS == true) {
                //LOG("Sending report to server.");
                String reportPayload = m_webServer.generateReportPayload();
                LOG("Report Payload: " + reportPayload);

                // Here you need to send the payload to the server
                String response = m_webServer.sendJsonToServer(portManager.settings.CALL_HOME_HOST, "/api/report", reportPayload);
                LOG("Server response: " + response);

                // Handle server response (log success or failure)
                if (m_webServer.getJSONValue(response, "status") == "success") {
                    //LOG("Report sent successfully.");
                    m_webServer.updatedReportStats();
                } else {
                    LOG("Failed to send report.");
                }
            }else{
              LOG("Failed to connect to server check Registration Status.  Report NOT delivered in time frame: " + LogManager::getInstance().timeToStringNow());
            }

            //UPDATE DISPLAY HERE
            //CODE TO UPDATE DISPLAY EVERY LOOP
            //RESET ACTION QUEUE
            actionQueue.clearActions();  
            //reset the Loop counter
            m_loopCounter = 0;
        }
     
    } catch (const std::exception& e) { 
        LogManager::getInstance().writeLog("*********MAJOR SYSTEM FAULT************");
        LogManager::getInstance().writeLog("Caught std::exception: " + String(e.what()));
    } catch(...) {
        LogManager::getInstance().writeLog("*********MAJOR SYSTEM FAULT************");
        LogManager::getInstance().writeLog("*********UNKNOWN************");
    }
}

void shutdown() {
    // Proper cleanup if necessary before shutdown
    LogManager::getInstance().writeLog("*********SYSTEM SHUTDOWN************");
}

//function that iterates through the current active port types and assigns and action to it.
void populateActionQueue() {
  PortManager& portManager = PortManager::getInstance();
    for (int i = 0; i < MAX_PORTS; ++i) {
        Ports& port = portManager.settings.ports[i];
        if (port.isActive) {
            ActionObject* action = nullptr;
            // Create action objects based on port type
            switch (port.circuitType) {
                case ON24V:
                    action = new ActionON24V(&port,9,"ON24V");  
                    break;
                case OFF24V:
                    action = new ActionOFF24V(&port,9,"OFF24V");  
                    break;
                case ONOFF:
                    action = new ActionONOFF(&port,9,"ONOFF");  
                    break;
                case MA420:
                    action = new ActionMA420(&port,7,"MA420");
                    break;
                case CTEMP:
                    action = new ActionCTEMP(&port,9,"CTEMP");
                    break;
                case VALVE:
                    action = new ActionTest(&port,1,"VALVE");
                    break;
                case FILL:
                    action = new ActionTest(&port,1,"FILL");
                    break;
                case PULSE:
                    action = new ActionTest(&port,1,"PULSE");
                    break;
                case FLOW:
                    action = new ActionTest(&port,1,"FLOW");  // Add read digital pin action
                    break;
                default:
                    break;
            }

            if (action) {
                actionQueue.addAction(action);  // Add action to queue
            }
        }
    }
    //Add last action for Display

    if(portManager.settings.DISPLAY_STATUS == true){
       ActionObject* action2 = new ActionDisplay(NULL,1, String(portManager.settings.DISPLAY_TEXT));
       actionQueue.addAction(action2); 
    }
}
//https://docs.arduino.cc/tutorials/portenta-machine-control/user-manual/

