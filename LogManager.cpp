/*
File: LogManager.cpp
Date Created: April 24, 2024
Authors: DaPhisha
Description: Defines LogManager class for handling logs.
Version: 1.0.0
*/

#include "LogManager.h"
#include "PortManager.h"
#include <PortentaEthernet.h>

// Set the serial status to false
bool m_serialConnected = false;
bool m_serialInitialized = false;
bool m_previousSerialStatus = false;

// Initialize the time client
EthernetUDP ntpUDP;
const long utcOffsetInSeconds = -4 * 3600;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

LogManager& LogManager::getInstance() {
    static LogManager instance; // Guaranteed to be destroyed and instantiated on first use
    return instance;
}

void LogManager::updateTime() {
    // Update time client
    timeClient.update();
}

void LogManager::init(bool setDebug) {
    // This could setup necessary log file or prep the system for logging
    m_debug = setDebug;
    timeClient.begin();
    timeClient.update();
    setControllerTime();
    if(PortManager::getInstance().settings.DISABLESERIAL == false){
      Serial.begin(9600); // Start serial communication for logging
      m_serialInitialized = true;
      unsigned long startTime = millis();
      while (!Serial && (millis() - startTime < 5000)) {
          delay(100);
      }
      if (Serial) {
        m_serialConnected = true;
        writeLog("Serial Connected");
      } else {
        m_serialConnected = false;
        writeLog("Serial Not Connected");
      }
    }else{
      writeLog("Serial Disabled");
    }
    writeLog("Log Manager: Init Complete TIME: " + timeToStringNow());
}

bool LogManager::checkSerialConnection() {

    if (Serial) {
        if (!m_serialConnected) {
            m_serialConnected = true;
            LOG("SERIAL CONNECTED");
        }
    } else {
        if (m_serialConnected) {
            m_serialConnected = false;
            LOG("SERIAL NOT CONNECTED");
        }
    }
    return m_serialConnected;
}

void LogManager::writeLog(String message) {

    String fullMessage = timeToStringNow() + ":" + message;
    
    if (PortManager::getInstance().settings.DISABLESERIAL == false && checkSerialConnection()) {
        Serial.println(fullMessage);
    }

    if (fullMessage.length() >= MAX_LOG_STRING_CHAR) {
        fullMessage = fullMessage.substring(0, MAX_LOG_STRING_CHAR);
    }

    // Copy the log message into the flash array
    strcpy(PortManager::getInstance().settings.logEntries[PortManager::getInstance().settings.currentLogIndex], fullMessage.c_str());

    PortManager::getInstance().settings.logEntries[PortManager::getInstance().settings.currentLogIndex][MAX_LOG_STRING_CHAR - 1] = '\0';  // Make sure it is null-terminated

    // Increment the index
    PortManager::getInstance().settings.currentLogIndex = (PortManager::getInstance().settings.currentLogIndex + 1) % MAX_LOG_ENTRIES;
}

String LogManager::timeToString(time_t t) {
    char timestamp[20];  // For YYYY-MM-DD HH:MM:SS format
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", localtime(&t));
    return String(timestamp);
}

String LogManager::timeToStringNow() {
    char timestamp[20];  // For YYYY-MM-DD HH:MM:SS format

    // Get the adjusted time
    unsigned long epochTime = timeClient.getEpochTime();
    time_t now = epochTime;

    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", localtime(&now));
    return String(timestamp);
}

time_t LogManager::getCurrentTime(){
  unsigned long epochTime = timeClient.getEpochTime();
  return (time_t)epochTime;
}

void LogManager::setControllerTime() {
    // Get the adjusted time
    unsigned long epochTime = timeClient.getEpochTime();

    // Set the RTC with NTP time
    // Assuming you have a function to set the RTC, otherwise, this line needs to be adjusted accordingly
    MachineControl_RTCController.begin();
    MachineControl_RTCController.setEpoch(epochTime);
}
