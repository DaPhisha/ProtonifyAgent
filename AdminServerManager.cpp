/*
  File: AdminServerManager.cpp
  Date Created: May 11, 2024
  Authors: DaPhisha, ChatGPT, and Assistant
  Description: Code file for the administrative web server.
  Last Modified: May 11, 2024
  Version 2.0.0

  struct AdminSettings {
        char Admin_USERNAME[MAX_USERNAME+1];//plus end of line
        char Admin_PASSWORD[MAX_PASSWORD+1];//plus end of line
        char WIFI_SSID[MAX_SSID+1];//plus end of line
        char WIFI_PASSWORD[MAX_PASSWORD+1];//plus end of line
        uint8_t IP_ADDRESS[4];//always four no end of line
        uint8_t GATEWAY[4];//always four no end of line
        uint8_t DNS[4];//always four no end of line
        uint8_t SUBNET[4];//always four no end of line
        byte MAC[6];//always six no end of line
        char SERIAL_NUMBER[MAX_SERIAL+1]; //plus end of line
        char MODEL[MAX_MODEL+1];
        uint32_t REFRESH_RATE;
        char SHARED_SECRET[MAX_SHARED_SECRET + 1];//32 plus end of line
        char CALL_HOME_URL[MAX_CALL_HOME_URL +1];//128 plus end of line
        bool REGISTRATION_STATUS;
        char logEntries[MAX_LOG_ENTRIES][MAX_LOG_STRING_CHAR];
        size_t currentLogIndex;
        time_t DATE_LAST_UPDATED;
        time_t DATE_LAST_REBOOT;
        Ports ports[MAX_PORTS];
    };
*/
//not sure I need these
//#include "stm32h747xx.h"
//#include "WiFiClient.h"
#include "AdminServerManager.h"
#include "ResourceHandler.h"

//Webserver port NOTE hard coded to port 80
EthernetServer server(HTTP_SERVER_PORT); //has to be a port set on constructor
bool m_serverStatusConnected = false;
//Token for authentication
String m_activeToken = "";
unsigned long m_tokenGenerationTime = 0;

//default refresh rate for the monitor page
//DASHBOARD_REFRESH = 3000

//Main constructor for ADMIN_WEB_SERVER
AdminServerManager::AdminServerManager() {

  static RouteHandler localHandlers[] = {
        {"GET", "/", handleRoot},
        {"GET", "/admin", handleAdmin},
        {"GET", "/js/script.js", handleJS},
        {"GET", "/css/style.css", handleCSS}
    };

    handlers = localHandlers;
    m_handlersCount = sizeof(localHandlers) / sizeof(localHandlers[0]);

}
String AdminServerManager::generateRandomToken() {
    String token = "";
    for (int i = 0; i < TOKEN_LENGTH; i++) { // Assuming TOKEN_LENGTH is the length of your token
        char randomChar = (char)(random(32, 127)); // Generate a random ASCII character
        token += randomChar;
    }
    return token;
}
void AdminServerManager::init() {

  //reset the access token
  m_activeToken = generateRandomToken();
  //start the ethernet listener using the IP address stored in PErsistent data
  


  LOG("CONFIG ADMIN SERVER");
  //IP Address for webserver
  IPAddress m_IPAddress(PortManager::getInstance().settings.IP_ADDRESS[0],
                        PortManager::getInstance().settings.IP_ADDRESS[1],
                        PortManager::getInstance().settings.IP_ADDRESS[2],
                        PortManager::getInstance().settings.IP_ADDRESS[3]);
  
  IPAddress m_DNS(PortManager::getInstance().settings.DNS[0],
                  PortManager::getInstance().settings.DNS[1],
                  PortManager::getInstance().settings.DNS[2],
                  PortManager::getInstance().settings.DNS[3]);
  
  IPAddress m_Gateway(PortManager::getInstance().settings.GATEWAY[0],
                  PortManager::getInstance().settings.GATEWAY[1],
                  PortManager::getInstance().settings.GATEWAY[2],
                  PortManager::getInstance().settings.GATEWAY[3]);

  IPAddress m_Subnet(PortManager::getInstance().settings.SUBNET[0],
                  PortManager::getInstance().settings.SUBNET[1],
                  PortManager::getInstance().settings.SUBNET[2],
                  PortManager::getInstance().settings.SUBNET[3]);

  
  int retValue = Ethernet.begin(
  PortManager::getInstance().settings.MAC,
  m_IPAddress,
  m_DNS,
  m_Gateway,
  m_Subnet);
  
  LOG("Admin Server Ethernet Begin Return Value: " + String(retValue));
  server.begin();
}

void AdminServerManager::handleClient() {
    EthernetClient client = server.available();
    if (client) {
        LOG("Client connected");
        String currentLine = ""; // Stores the current line of the request
        String method = "";      // Stores the HTTP method (GET, POST, etc.)
        String url = "";         // Stores the URL requested
        bool isHeaderComplete = false; // Flag to check if the HTTP header has been fully received

        while (client.connected()) {
            if (client.available()) {
                char c = client.read(); // Read one character from the client
                if (c == '\n') { // Check if the character is a newline
                    if (currentLine.length() == 0) { // A newline on an empty line indicates the end of the HTTP header
                        isHeaderComplete = true; // Set the header completion flag
                        for (int i = 0; i < m_handlersCount; i++) {
                            if (method == handlers[i].method && url == handlers[i].path) {
                                LOG("Handling route: " + url);
                                handlers[i].handler(client, "");  // Call the handler function
                                break;
                            }
                        }
                        if (!isHeaderComplete) {
                            LOG("HTTP Header incomplete, waiting for more data...");
                        }
                        break;
                    } else {
                        // At the end of each line, check if it's the first line with HTTP method and URL
                        if (method == "" && url == "") {
                            int firstSpace = currentLine.indexOf(' ');
                            int secondSpace = currentLine.lastIndexOf(' ');
                            if (firstSpace != -1 && secondSpace != -1 && firstSpace != secondSpace) {
                                method = currentLine.substring(0, firstSpace);
                                url = currentLine.substring(firstSpace + 1, secondSpace);
                            }
                        }
                        currentLine = ""; // Reset currentLine for the next line
                    }
                } else if (c != '\r') {
                    currentLine += c; // Add character to currentLine if it's not a carriage return
                }
            }
        }
        client.stop(); // Close the client connection
        LOG("Client disconnected");
    }
}

String AdminServerManager::getUptimeString() {
  unsigned long totalSeconds = millis() / 1000;
  unsigned long days = totalSeconds / 86400;
  totalSeconds %= 86400;
  unsigned long hours = totalSeconds / 3600;
  totalSeconds %= 3600;
  unsigned long minutes = totalSeconds / 60;
  unsigned long seconds = totalSeconds % 60;

  String uptime = "Uptime: " + String(days) + "d " + String(hours) + "h " + String(minutes) + "m " + String(seconds) + "s";
  return uptime;
}


void AdminServerManager::handleRoot(EthernetClient& client, const String& request) {

    String message = "Welcome to the Protonify Agent Management System!" + AdminServerManager::getUptimeString();
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println("Connection: close");
    client.println();
    client.println("<!DOCTYPE HTML>");
    client.println("<html>");
    client.println("<head>");
    // Additional CSS for centering and styling
    client.println("<link rel='stylesheet' href='/css/style.css'>");
    client.println("<style>");
    client.println("");
    client.println("</style>");
    client.println("</head>");
    client.println("<body>");
    client.println("<br><div class='center' style='color: blue;'>" + message + "</div><br>");
    client.println("<div class='container center' style='background-color: gray;'>");
    
    client.print(ResourceHandler::getLoginCard());
    client.println("</div>");
    client.print(ResourceHandler::getFooter());
    client.println("</body>");
    client.println("</html>");
}

void AdminServerManager::handleAdmin(EthernetClient& client, const String& request) {
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println();
    client.println("<html><body><h1>Admin Page</h1></body></html>");
}

void AdminServerManager::handleJS(EthernetClient& client, const String& request) {
    client.print(ResourceHandler::getJavascript());
}

void AdminServerManager::handleCSS(EthernetClient& client, const String& request) {
    client.print(ResourceHandler::getCSS());
}