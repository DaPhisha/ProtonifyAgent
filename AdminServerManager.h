/*
  File: AdminServerManager.h
  Date Created: May 11, 2024
  Authors: DaPhisha, ChatGPT, and Assistant
  Description: Header file for the administrative web server.
  Last Modified: May 11, 2024
  Version 2.0.0
*/

#pragma once
#include <Arduino.h>
#include <SPI.h>
//#include <Ethernet.h>
#include <EthernetSSLClient.h>
#include <PortentaEthernet.h>
#include <WiFi.h>
#include <cstdlib>
#include "PortManager.h"
#include "LogManager.h"
#include "FlashAPILimits.h"
#include "ResourceHandler.h"
#define TOKEN_LENGTH 32 
#define HTTP_SERVER_PORT 80
#define DASHBOARD_REFRESH 3000 //miliseconds to refresh dashboard

class AdminServerManager {
private:
    //ANY function called from within a static function must also be static
    static void handleRoot(Client& client, const String& request, int contentLength, const String &authToken);
    static void handleHome(Client& client, const String& request, int contentLength, const String &authToken);
    static void handleToggle(Client& client, const String& request, int contentLength, const String &authToken);
    static void handleAdmin(Client& client, const String& request, int contentLength, const String &authToken);
    static void handleConsole(Client& client, const String& request, int contentLength, const String &authToken);
    static void handleClearLog(Client& client, const String& request, int contentLength, const String &authToken);
    static void handleReset(Client& client, const String& request, int contentLength, const String &authToken);
    static void processAdmin(Client& client, const String& request, int contentLength, const String &authToken);
    static void handleCSS(Client& client, const String& request, int contentLength, const String &authToken);
    static void handleHelp(Client& client, const String& request, int contentLength, const String &authToken);
    static void handleJS(Client& client, const String& request, int contentLength, const String &authToken);
    static void handleLogin(Client& client, const String& request, int contentLength, const String &authToken); 
    static void handleLogOut(Client& client, const String& request, int contentLength, const String &authToken);
    static void handleTestConnection(Client& client, const String& request, int contentLength, const String& authToken);
    static void handleDisplay(Client& client, const String& request, int contentLength, const String& authToken);
    static void processDislay(Client& client, const String& request, int contentLength, const String &authToken);
    //PORT HANDLERS
    static void handleGetAnalogINPorts(Client& client, const String& request, int contentLength, const String &authToken);
    static void handleAnalogINPorts(Client& client, const String& request, int contentLength, const String &authToken);
    static void handleGetAnalogOUTPorts(Client& client, const String& request, int contentLength, const String &authToken);
    static void handleAnalogOUTPorts(Client& client, const String& request, int contentLength, const String &authToken);
    static void handleDigitalINPorts(Client& client, const String& request, int contentLength, const String &authToken);
    static void handleGetDigitalINPorts(Client& client, const String& request, int contentLength, const String &authToken);
    static void handleDigitalOUTPorts(Client& client, const String& request, int contentLength, const String &authToken);
    static void handleGetDigitalOUTPorts(Client& client, const String& request, int contentLength, const String &authToken);
    static void handleProgrammableIOPorts(Client& client, const String& request, int contentLength, const String &authToken);
    static void handleGetProgrammableIOPorts(Client& client, const String& request, int contentLength, const String &authToken);
    static void handleHMIPorts(Client& client, const String& request, int contentLength, const String &authToken);
    static void handleGetHMIPorts(Client& client, const String& request, int contentLength, const String &authToken);
    static void handlePTEMPPorts(Client& client, const String& request, int contentLength, const String &authToken);
    static void handleGetPTEMPPorts(Client& client, const String& request, int contentLength, const String &authToken);
    static void handleENCODERPorts(Client& client, const String& request, int contentLength, const String &authToken);
    static void handleGetENCODERPorts(Client& client, const String& request, int contentLength, const String &authToken);
    static void handleGetAllActivePorts(Client& client, const String& request, int contentLength, const String &authToken);
    static void handleAllActivePorts(Client& client, const String& request, int contentLength, const String &authToken); 
    static void handlePortsPage(Client& client, const String& request, int contentLength, const String &authToken, const String& pageTitle);
    static void handleGetPorts(Client& client, const String& request, int contentLength, const String &authToken, PIN_TYPE pinType);
    static void handlePortUpdate(Client& client, const String& request, int contentLength, const String &authToken);
    static String getUptimeString();
    static String generateRandomToken();
    static String urlDecode(const String &str);
    static int getContentLength(String requestString);
    static String getPostData(int contentLength, const String &request);
    static String extractValueFromPostData(String postData, String variableName);
    static void handleError(Client& client, int errorCode, String errorMessage);
    static void handleHomeLandingPage(Client& client, const String& request, String user_message);
    static bool isValidMAC(const String& mac);
    static bool isValidIP(const String& ip);
    static bool isValidURL(const String& url);
    static void sendSuccessResponse(Client& client, const String& message);
    static void sendSuccessResponseData(Client& client, const String& message, const String& data);
    static void sendErrorResponse(Client& client, const String& message);
    static void handleRegister(Client& client, const String& request, int contentLength, const String& authToken);
    static void handleUnregister(Client& client, const String& request, int contentLength, const String& authToken);
    static String generateUnregistrationPayload();
    static String generateRegistrationPayload();
    static void handleClientConnection(Client& client);

public:
    AdminServerManager();
    void init();  // Initialize server
    void handleClient();  // Handle incoming client requests
    static String generateReportPayload();
    static String sendJsonToServer(const String& host, const String& path, const String& payload);
    static String getJSONValue(const String& json, const String& key);
    static bool attemptWiFiReconnection();
    static bool checkWiFiReconnection(); 
    static void updatedReportStats();
    
};
