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
#include <PortentaEthernet.h>
#include <cstdlib>
#include "PortManager.h"
#include "LogManager.h"
#include "FlashAPILimits.h"
#define TOKEN_LENGTH 32 
#define HTTP_SERVER_PORT 80
#define DASHBOARD_REFRESH 3000 //miliseconds to refresh dashboard

//Struct for server response
struct CallHomeResponse {
    int code;
    String message;
};
struct RouteHandler {
    String method;
    String path;
    void (*handler)(EthernetClient &client, const String &request, int contentLength, const String &authToken);
};
class AdminServerManager {
private:
    RouteHandler* handlers;
    int m_handlersCount;

    //ANY function called from within a static function must also be static
    static void handleRoot(EthernetClient& client, const String& request,int contentLength, const String &authToken);
    static void handleHome(EthernetClient& client, const String& request,int contentLength, const String &authToken);
    static void handleAdmin(EthernetClient& client, const String& request,int contentLength, const String &authToken);
    static void handleConsole(EthernetClient& client, const String& request,int contentLength, const String &authToken);
    static void handleReset(EthernetClient& client, const String& request,int contentLength, const String &authToken);
    static void processAdmin(EthernetClient& client, const String& request,int contentLength, const String &authToken);
    static void handleCSS(EthernetClient& client, const String& request,int contentLength, const String &authToken);
    static void handleHelp(EthernetClient& client, const String& request,int contentLength, const String &authToken);
    static void handleJS(EthernetClient& client, const String& request,int contentLength, const String &authToken);
    static void handleLogin(EthernetClient& client, const String& request,int contentLength, const String &authToken); 
    static void handleLogOut(EthernetClient& client, const String& request,int contentLength, const String &authToken);
    
   
    //PORT HANDLERS
    static void handleGetAnalogINPorts(EthernetClient& client, const String& request,int contentLength, const String &authToken);
    static void handleAnalogINPorts(EthernetClient& client, const String& request,int contentLength, const String &authToken);
    static void handleGetAnalogOUTPorts(EthernetClient& client, const String& request,int contentLength, const String &authToken);
    static void handleAnalogOUTPorts(EthernetClient& client, const String& request,int contentLength, const String &authToken);
    static void handleDigitalINPorts(EthernetClient& client, const String& request, int contentLength, const String &authToken);
    static void handleGetDigitalINPorts(EthernetClient& client, const String& request, int contentLength, const String &authToken);
    static void handleDigitalOUTPorts(EthernetClient& client, const String& request, int contentLength, const String &authToken);
    static void handleGetDigitalOUTPorts(EthernetClient& client, const String& request, int contentLength, const String &authToken);
    static void handleProgrammableIOPorts(EthernetClient& client, const String& request, int contentLength, const String &authToken);
    static void handleGetProgrammableIOPorts(EthernetClient& client, const String& request, int contentLength, const String &authToken);
    static void handleHMIPorts(EthernetClient& client, const String& request, int contentLength, const String &authToken);
    static void handleGetHMIPorts(EthernetClient& client, const String& request, int contentLength, const String &authToken);
    static void handlePTEMPPorts(EthernetClient& client, const String& request, int contentLength, const String &authToken);
    static void handleGetPTEMPPorts(EthernetClient& client, const String& request, int contentLength, const String &authToken);
    
    static void handleENCODERPorts(EthernetClient& client, const String& request, int contentLength, const String &authToken);
    static void handleGetENCODERPorts(EthernetClient& client, const String& request, int contentLength, const String &authToken);

    static void handleGetAllActivePorts(EthernetClient& client, const String& request, int contentLength, const String &authToken);
    static void handleAllActivePorts(EthernetClient& client, const String& request,int contentLength, const String &authToken); 
   //Generic Port Handler
    //loads the page for admin ports by type
    static void handlePortsPage(EthernetClient& client, const String& request, int contentLength, const String &authToken, const String& pageTitle);
    static void handleGetPorts(EthernetClient& client, const String& request, int contentLength, const String &authToken, PIN_TYPE pinType);
    //Port update from AJAX client
    static void handlePortUpdate(EthernetClient& client, const String& request,int contentLength, const String &authToken);
  

    static String getUptimeString();
    static String generateRandomToken();
    static String urlDecode(const String &str);
    static int getContentLength(String requestString);
    static String getPostData(int contentLength, const String &request);
    static String extractValueFromPostData(String postData, String variableName);
    static void handleError(EthernetClient& client, int errorCode, String errorMessage);
    static void handleHomeLandingPage(EthernetClient& client, const String& request, String user_message);
    static bool isValidMAC(const String& mac);
    static bool isValidIP(const String& ip);
    static bool isValidURL(const String& url);
    static void sendSuccessResponse(EthernetClient& client, const String& message);
    static void sendSuccessResponseData(EthernetClient& client, const String& message, const String& data);
    static void sendErrorResponse(EthernetClient& client, const String& message);
    /*
    void serveMainPage(EthernetClient& client, const String &message);
    void serveLoginPage(EthernetClient& client, const String &message);
    void serveAdminPage(EthernetClient& client, const String &message);
    void serveLoggerPage(EthernetClient& client, const String &message);
    void processLogin(EthernetClient& client, const String& username, const String& password);
    void serve404Page(EthernetClient& client);
    void serve400Page(EthernetClient& client, const String &message);
    void serveMonitorPage(EthernetClient& client, const String &message);
    void serveHelpPage(EthernetClient& client, const String &message);
    void serveResetPage(EthernetClient& client);
    
    String urlDecode(const String &str);
    String generateToken();
    
    CallHomeResponse sendJsonToRemoteServer(const String &jsonPayload, const String routeToCallHome);
    String parseAndValidatePostData(const String &data); 
    String createRegisterJsonPayload();
    CallHomeResponse decodeJSONResponse(const String& jsonResponse);
    void serveCard(EthernetClient& client, const String &cardClass,const String &cardTitle, const String &formAction, const String &buttonText, const String &content);
    int getContentLength(String &request);
    String getPostData(int contentLength, String &request);
    String getParameterValue(const String &parameterName, const String &data); 
    String generatePortOptions();
    int findPinByReference(const String &reference); 
    void printPinDetailsAtReference(int referenceNum);
    void servePortCard(EthernetClient& client,const String &cardTitle, const String &content, const String &color); 
    String getNextShadeOfGreen(int step, int iteration); 
    String dropDownCircuitSelected(int refNum);
    String createStatusJsonPayload();
    String connectionURLTest(const String &testURL);
    String formatMacAddress(byte mac[]); 
    String extractValueFromPostData(String postData, String variableName);
    byte convertHexStrToByte(String hexStr); 
    */

public:
    AdminServerManager();
    void init();  // Initialize server
    void handleClient();  // Handle incoming client requests
    //void checkConnection();
};