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
#include "NetworkManager.h"
#include "AdminServerManager.h"

#define MAX_EVENTS 10  // Maximum number of scheduled events
#define MASTER_DEBUG true

const char* PROJECT_VERSION = "2.0.0"; // Project version

// Initialize an event scheduler
EventScheduler scheduler(MAX_EVENTS); 

// Initialize network interfaces
EthernetUDP ntpUDP;
AdminServerManager m_webServer;

// Initialize the time client
NTPClient timeClient(ntpUDP, "pool.ntp.org");
time_t systemStartTime;  // Global variable to measure elapsed refresh rates
time_t lastReportTime = 0;

void setup() {
    LogManager& logManager = LogManager::getInstance(); // Obtain the global LogManager instance
    logManager.init(MASTER_DEBUG);  // Initialize LogManager (e.g., setting up Serial communication)
    try {
        // Task Scheduler: Add tasks with timeout and callback
        scheduler.addTask(86400000, checkMemory);  // Check memory every 24 hours
        scheduler.addTask(3600000, checkNetwork);  // Check network every hour
        scheduler.addTask(86400000, writeSettingsToFlash);  // Write settings to flash every 24 hours

        // Initialize PortManager
        PortManager& portManager = PortManager::getInstance(); 
        portManager.init();

        // Initialize NetworkManager
        //DROP this as it is going to be moved to Webserver Init
        //NetworkManager& networkManager = NetworkManager::getInstance();
        //if (!networkManager.initializeNetwork()) {
        //    LOG("Failed to initialize network.");
        //}

        // Initialize the admin web server
        //set to try Wifi FIRST
        m_webServer.init();

    } catch (const std::exception& e) { 
        LogManager::getInstance().writeLog("*********MAJOR STARTUP FAULT************");
        LogManager::getInstance().writeLog("Caught std::exception: " + String(e.what()));
    } catch(...) {
        LogManager::getInstance().writeLog("*********MAJOR STARTUP FAULT************");
        LogManager::getInstance().writeLog("*********UNKNOWN************");
    }
    logManager.writeLog("System Initialized Successfully"); 
}

void loop() {
    try {
        // Main Loop (M7)
        scheduler.run();  // Run scheduled tasks
        m_webServer.handleClient();  // Process any client requests

        // Periodically check WiFi connection
        //if (WiFi.status() != WL_CONNECTED) {
        //m_webServer.attemptWiFiReconnection();
        //}


        // Update ports
        PortManager& portManager = PortManager::getInstance();
        time_t now = time(NULL);

        //update all ports every cycle
        for (int i = 0; i < MAX_PORTS; ++i) {
                Ports& port = portManager.settings.ports[i];
                if (port.isActive) {
                    updatePort(port);
                    port.lastUpdated = now;  // Update the lastUpdated time for each active port
                }
        }

        // Check if the refresh interval has passed
        if (difftime(now, lastReportTime) >= portManager.settings.REFRESH_RATE / 1000) {
            LOG("Updating all active ports and sending report to server.");
            
            //Check network connection
            bool checkWifi = m_webServer.checkWiFiReconnection();
            if(checkWifi == false){
              checkWifi = m_webServer.attemptWiFiReconnection();
            }

            // Check to see if the Arduino is registered
            if (portManager.settings.REGISTRATION_STATUS == true && checkWifi == true) {
                LOG("Sending report to server.");
                String reportPayload = m_webServer.generateReportPayload();
                LOG("Report Payload: " + reportPayload);

                // Here you need to send the payload to the server
                String response = m_webServer.sendJsonToServer(portManager.settings.CALL_HOME_HOST, "/api/report", reportPayload);
                LOG("Server response: " + response);

                // Handle server response (log success or failure)
                if (m_webServer.getJSONValue(response, "status") == "success") {
                    LOG("Report sent successfully.");
                } else {
                    LOG("Failed to send report.");
                }
            }else{
              char timestamp[20];  // For YYYY-MM-DD HH:MM:SS format
              strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", localtime(&now));
              LOG("Failed to connect to wifi.  Report NOT delivered in time frame: " + String(timestamp));
            }

            // Update the last report time
            lastReportTime = now;
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

void updatePort(Ports& port) {
    try {
        switch (port.circuitType) {
            case ONOFF:
                simulateONOFF(port);
                break;
            case MA420:
                simulateMA420(port);
                break;
            case CTEMP:
                simulateCTEMP(port);
                break;
            case VALVE:
                simulateVALVE(port);
                break;
            case FILL:
                simulateFILL(port);
                break;
            case PULSE:
                simulatePULSE(port);
                break;
            default:
                strncpy(port.state, "UNKNOWN", sizeof(port.state));
                break;
        }
    } catch (const std::exception& e) {
        LogManager::getInstance().writeLog("Error updating port: " + String(e.what()));
    } catch (...) {
        LogManager::getInstance().writeLog("Unknown error updating port.");
    }
}

void simulateONOFF(Ports& port) {
    if (port.isSimulated) {
        port.currentReading = port.currentReading == 0 ? 1 : 0;
        if(port.currentReading == 0){

        strcpy(port.state, String("OFF").c_str());
    
        }else{
          strcpy(port.state, String("ON").c_str());
        }
    }
    // Implement the actual read/write logic for ONOFF ports
}

void simulateMA420(Ports& port) {
    if (port.isSimulated) {
        int randomValue = random(0, 3);  // Generate a random number between 0 and 2
        if (randomValue == 0) {
            port.currentReading = 0;
            strcpy(port.state, String("No Signal").c_str());
        } else if (randomValue == 1) {
            port.currentReading = 4;
            strcpy(port.state, String("4 mA").c_str());
        } else {
            port.currentReading = 20;
            strcpy(port.state, String("20 mA").c_str());
        }
    }
    // Implement the actual read/write logic for MA420 ports
}

void simulateCTEMP(Ports& port) {
    if (port.isSimulated) {
        port.currentReading = random(-40, 100);  // Simulate a temperature reading
    }
    if(port.lastReading < port.currentReading){
      strcpy(port.state, String("Warming").c_str());
    }
    if(port.lastReading == port.currentReading){
      strcpy(port.state, String("Constant").c_str());
    }
    if(port.lastReading > port.currentReading){
      strcpy(port.state, String("Cooling").c_str());
    }

    // Implement the actual read/write logic for temperature ports
}

void simulateVALVE(Ports& port) {
    if (port.isSimulated) {
        port.currentReading = port.currentReading == 0 ? 1 : 0;  // Simulate valve open/close
    }
    // Implement the actual read/write logic for valve ports
    if(port.currentReading == 0){
          strcpy(port.state, String("Closed").c_str());
        }else{
          strcpy(port.state, String("Open").c_str());
        }
}

void simulateFILL(Ports& port) {
    if (port.isSimulated) {
        port.currentReading = random(0, 100);  // Simulate a fill level
    }
    String tmpStr =  String(port.currentReading) + "%";
    strcpy(port.state, tmpStr.c_str());
    
    // Implement the actual read/write logic for fill ports
}

void simulatePULSE(Ports& port) {
    if (port.isSimulated) {
        port.currentReading = random(1, 100);  // Simulate a pulse reading
    }
    // Implement the actual read/write logic for pulse ports
    strcpy(port.state, String("Active").c_str());
}
