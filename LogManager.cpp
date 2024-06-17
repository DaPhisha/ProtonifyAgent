/*
File: LogManager.cpp
Date Created: April 24, 2024
Authors: DaPhisha
Description: Defines LogManager class for handling logs.
Version: 1.0.0
*/
#include "LogManager.h"
#include "PortManager.h"


LogManager& LogManager::getInstance() {
    static LogManager instance; // Guaranteed to be destroyed and instantiated on first use
    return instance;
}

void LogManager::init(bool setDebug) {
    // This could setup necessary log file or prep the system for logging
    m_debug = setDebug;
    //ADD TIME OUT
    Serial.begin(9600); // Start serial communication for logging
    unsigned long startTime = millis();
    while (!Serial && (millis() - startTime < 5000)) {
        // Wait for serial or 5 seconds timeout
        //writeLog("Initializing Serial Bus");
    }
    writeLog("LogManager: INIT COMPLETE");
}

void LogManager::writeLog(String message) {
    char timestamp[20];  // For YYYY-MM-DD HH:MM:SS format
    time_t now = time(NULL);
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", localtime(&now));
    
    String fullMessage = String(timestamp) + ":" + message;

    if (Serial) {
        Serial.println(fullMessage);
    }

  if (fullMessage.length() >= MAX_LOG_STRING_CHAR) {
        fullMessage = fullMessage.substring(0, MAX_LOG_STRING_CHAR);
    }

  //copy the log message into the flash array  
  strcpy(PortManager::getInstance().settings.logEntries[PortManager::getInstance().settings.currentLogIndex], fullMessage.c_str());

  PortManager::getInstance().settings.logEntries[PortManager::getInstance().settings.currentLogIndex][MAX_LOG_STRING_CHAR -1] = '\0';  //Make sure it null terminated

  //increment the index
  PortManager::getInstance().settings.currentLogIndex = (PortManager::getInstance().settings.currentLogIndex + 1) % MAX_LOG_ENTRIES;

}
void LogManager::writeDebugLog(String message) {
    if(m_debug == false){
      //don't write this 
      return;
    }
    char timestamp[20];  // For YYYY-MM-DD HH:MM:SS format
    time_t now = time(NULL);
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", localtime(&now));
    
    String fullMessage = String(timestamp) + ":" + message;

    if (Serial) {
        Serial.println(fullMessage);
    }
}
String LogManager::timeToString(time_t t) {
    char timestamp[20];  // For YYYY-MM-DD HH:MM:SS format
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", localtime(&t));
    return String(timestamp);
}