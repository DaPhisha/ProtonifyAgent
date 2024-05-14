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

class ResourceHandler {
public:
    static const char* getCSS();
    static const char* getJavascript();
    static const char* getHeader();
    static const char* getFooter();
    static const char* getLoginCard();
};

#endif // RESOURCE_HANDLER_H
