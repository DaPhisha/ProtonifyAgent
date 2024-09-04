/*
  File: ResourceHandler.h
  Date Created: May 11, 2024
  Authors: DaPhisha, ChatGPT, and Assistant
  Description: Header file for the resource handler class.
  Last Modified: May 11, 2024
  Version 2.0.0
*/
#ifndef RESOURCE_HANDLER_H
#define RESOURCE_HANDLER_H

#include <Arduino.h>
#include "PortManager.h"
#include "LogManager.h"

class ResourceHandler {
public:
    static const char* getCSS();
    static const char* getJavascript();
    static const char* getHeader(String title);
    static const char* getHeaderRefresh(String title);
    static const char* getFooter();
    static const char* getLoginCard();
    static const char* getHeaderMenu();
    static const char* getStatCard(String title, String statValue);
    static const char* getStatCardWifi(String title, String statValue);
    static const char* getStatCardUSB(String title, String statValue);
    static const char* getPortMainContent();
    static const char* getMessageDiv(String msg);
    static const char* getActivePortMainContent();
    static const char* getHelpContent(); 
    static const char* getDisplayCard(String displayTxt, bool displayStatus);
    static const char* getRegisterCard(bool isRegistered);
    static const char* getClearButton();
    static const char* getRefreshIntervalDropdown();
};

#endif // RESOURCE_HANDLER_H
