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
static String m_activeToken = "";
static unsigned long m_tokenGenerationTime = 0;

//default refresh rate for the monitor page
//DASHBOARD_REFRESH = 3000

//Main constructor for ADMIN_WEB_SERVER
AdminServerManager::AdminServerManager() {
/*
static void handleRoot(EthernetClient& client, const String& request,int contentLength, const String &authToken);
    static void handleHome(EthernetClient& client, const String& request,int contentLength, const String &authToken);
    static void handleAdmin(EthernetClient& client, const String& request,int contentLength, const String &authToken);
    static void handleCSS(EthernetClient& client, const String& request,int contentLength, const String &authToken);
    static void handleJS(EthernetClient& client, const String& request,int contentLength, const String &authToken);
    static void handleLogin(EthernetClient& client, const String& request,int contentLength, const String &authToken); 
*/
  static RouteHandler localHandlers[] = {
        {"GET", "/", handleRoot},
        {"GET", "/login", handleRoot},
        {"GET", "/logout", handleLogOut},
        {"POST", "/login", handleLogin},
        {"GET", "/home", handleHome},
        {"GET", "/admin", handleAdmin},
        {"POST", "/admin", processAdmin},

        {"GET", "/js/script.js", handleJS},
        {"GET", "/css/style.css", handleCSS}
    };

    handlers = localHandlers;
    m_handlersCount = sizeof(localHandlers) / sizeof(localHandlers[0]);

}
String AdminServerManager::generateRandomToken() {
    String token = "";
    for (int i = 0; i < 10; i++) { 
        char c = (char) random(33, 126); // Generate a random character from '!'(33) to '~'(126)
        token += c;
    }
    token += String(millis());
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
        String requestBody = ""; // Stores the body of a POST request
        String authToken = "";   // Stores the value of the authToken cookie
        int contentLength = 0;   // Stores the length of the POST request body

        while (client.connected()) {
            if (client.available()) {
                char c = client.read(); // Read one character from the client
                if (c == '\n') { // Check if the character is a newline
                    if (currentLine.length() == 0) { // A newline on an empty line indicates the end of the HTTP header
                        isHeaderComplete = true; // Set the header completion flag

                        // If the method is POST, read the request body
                        if (method == "POST" && contentLength > 0) {
                            while (requestBody.length() < contentLength) {
                                if (client.available()) {
                                    char c = client.read();
                                    requestBody += c;
                                }
                            }
                        }

                        // Process the request using the appropriate handler
                        for (int i = 0; i < m_handlersCount; i++) {
                            if (method == handlers[i].method && url == handlers[i].path) {
                                LOG("Handling route: " + url);
                                LOG("Method: " + method);
                                LOG("Auth Token: " + authToken); // Log the authToken
                                handlers[i].handler(client, requestBody, contentLength, authToken); // Call the handler function with the request body and content length
                                break;
                            }
                        }
                        break;
                    } else {
                        // Check if it's the first line with HTTP method and URL
                        if (method == "" && url == "") {
                            int firstSpace = currentLine.indexOf(' ');
                            int secondSpace = currentLine.lastIndexOf(' ');
                            if (firstSpace != -1 && secondSpace != -1 && firstSpace != secondSpace) {
                                method = currentLine.substring(0, firstSpace);
                                url = currentLine.substring(firstSpace + 1, secondSpace);
                            }
                        }

                        // Check for Content-Length header to determine the size of the POST request body
                        if (currentLine.startsWith("Content-Length: ")) {
                            contentLength = currentLine.substring(16).toInt();
                        }

                        // Check for Cookie header to find the authToken
                        if (currentLine.startsWith("Cookie: ")) {
                            int authTokenIndex = currentLine.indexOf("authToken=");
                            if (authTokenIndex != -1) {
                                int authTokenEnd = currentLine.indexOf(';', authTokenIndex);
                                if (authTokenEnd == -1) {
                                    authTokenEnd = currentLine.length();
                                }
                                authToken = currentLine.substring(authTokenIndex + 10, authTokenEnd);
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

void AdminServerManager::handleHome(EthernetClient& client, const String& request,int contentLength, const String &authToken) {

  //check for authentication token if pass redirect
  if(authToken == m_activeToken){
    handleHomeLandingPage(client, request,"Authentication Token found.");
  }else{
    handleError(client, 401, "User not authorized! Please re-login.");
  }
}

void AdminServerManager::handleRoot(EthernetClient& client, const String& request,int contentLength, const String &authToken) {
     LOG("GET /login");
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



void AdminServerManager::handleLogOut(EthernetClient& client, const String& request,int contentLength, const String &authToken) {
      //reset the active token and push the user to login page
      m_activeToken = generateRandomToken();
      handleRoot(client, request, 0,"");
}

void AdminServerManager::handleLogin(EthernetClient& client, const String& request,int contentLength, const String &authToken) {

      //check auth Token
      if(authToken == m_activeToken){
          LOG("POST /login already authenticated");
          handleHomeLandingPage(client,request, "Resuming session with found Token.");
          return;
      }

      LOG("POST /login");
      DEBUG("REQUEST: " + request);
      String m_userMessage = "";
      /*
      int m_contentLength = getContentLength(request);

      if(m_contentLength < 0){
        m_userMessage = "Missing Content Length on POST login.";
        LOG(m_userMessage);
        handleError(client, 411, m_userMessage);
        return;
      }
      */

      //String postData = getPostData(m_contentLength, request);
      String m_userName = extractValueFromPostData(request,"username");
      String m_password = extractValueFromPostData(request,"password");

      //LOG("Post Params: Content-Length: " + String(m_contentLength));
      LOG("Username: " + m_userName + " Password: " + m_password);


      if (m_userName == PortManager::getInstance().settings.Admin_USERNAME && m_password == PortManager::getInstance().settings.Admin_PASSWORD) {
        m_activeToken = generateRandomToken(); // Sets the active token to this one
        m_tokenGenerationTime = millis();//sets the token gen time to this one (note another authurized login will wipe the previos one authorized

        //serveMainPage(client, "Successfully logged in.");
        m_userMessage = "PASSED CREDENTIAL CHECK.";
        LOG(m_userMessage);
        handleHomeLandingPage(client,request, "Welcome back " + m_userName);
    } else {
        //serveLoginPage(client, "Invalid credentials. Try again.");
        m_userMessage = "LOGIN FAILED";
        LOG(m_userMessage);
        handleError(client, 404, m_userMessage);
    }
}
/*
Helper function to get  the content length from a POST to calculate the body size
*/
int AdminServerManager::getContentLength(String requestString){
    int contentLengthIndex = requestString.indexOf("Content-Length: ");
    if (contentLengthIndex != -1) {
        int contentLengthStart = contentLengthIndex + 15;
        int contentLengthEnd = requestString.indexOf("\r\n", contentLengthStart);
        String contentLengthStr = requestString.substring(contentLengthStart, contentLengthEnd);
        return contentLengthStr.toInt();
    }
    return -1;
}
/*
Helper function for URL Decoding a string
*/

String AdminServerManager::urlDecode(const String &str) {
    String decoded = "";
    char charBuffer[3];
    charBuffer[2] = '\0'; // Null-terminate the buffer for the conversion.

    for (unsigned int i = 0; i < str.length(); i++) {
        char c = str.charAt(i);
        if (c == '+') { 
            decoded += ' ';
        } else if (c == '%' && (i + 2 < str.length())) {
            // Grab the next two characters
            charBuffer[0] = str.charAt(++i);
            charBuffer[1] = str.charAt(++i);
            decoded += (char) strtol(charBuffer, NULL, 16);
        } else {
            decoded += c;
        }
    }
    return decoded;
}

/*
Helper function for extracting post Data payload from the body of the request
*/
//static String getPostData(int contentLength, const String &request);
String AdminServerManager::getPostData(int contentLength, const String &request) {
    if (contentLength <= 0) {
        return String();  // return empty string if content length is invalid
    }
    int headerEnd = request.indexOf("\r\n\r\n");
    return request.substring(headerEnd + 4, headerEnd + 4 + contentLength);
}

/*
Helper function for extracting values from post data
*/
String AdminServerManager::extractValueFromPostData(String postData, String variableName) {
    int variableIndex = postData.indexOf(variableName + "=");
    if (variableIndex != -1) {
        int valueIndex = variableIndex + variableName.length() + 1; // Skip '='
        int endIndex = postData.indexOf("&", valueIndex);
        if (endIndex == -1) {
            endIndex = postData.length(); // Reached end of string
        }
        return postData.substring(valueIndex, endIndex);
    }
    return ""; // Variable not found
}

/*
Helper function for extracting values from post data

String AdminServerManager::getParameterValue(const String &parameterName, const String &data) {
    String searchFor = parameterName + "=";
    int startIdx = data.indexOf(searchFor);
    
    // If the parameter isn't found, return an empty string
    if (startIdx == -1) {
        return "";
    }

    startIdx += searchFor.length();  // Move the index to the start of the value
    int endIdx = data.indexOf("&", startIdx);
    if (endIdx == -1) endIdx = data.length();  // If there's no &, it's the last parameter

    return data.substring(startIdx, endIdx);
}
*/

void AdminServerManager::handleJS(EthernetClient& client, const String& request,int contentLength, const String &authToken) {
    client.print(ResourceHandler::getJavascript());
}

void AdminServerManager::handleCSS(EthernetClient& client, const String& request,int contentLength, const String &authToken) {
    client.print(ResourceHandler::getCSS());
}

void AdminServerManager::handleError(EthernetClient& client, int errorCode, String errorMessage) {

  client.println("HTTP/1.1 " + String(errorCode) + " " + errorMessage);
  client.println("Content-Type: text/html");
  client.println("Connection: close");
  client.println();
  client.println("<!DOCTYPE HTML>");
  client.println("<html lang='en'>");
  client.println("<head>");
  client.println("<meta charset='UTF-8'>");
  client.println("<meta name='viewport' content='width=device-width, initial-scale=1.0'>");
  client.println("<title>Protonify Agent Handler</title>");
  client.println("<link rel='stylesheet' href='/css/style.css'>");
  client.println("</head>");
  client.println("<body>");
  client.println("<div class='error-container'>");
  client.println("<div class='card large'>");
  client.println("<div class='card-header'> Opps we found an error!</div>");
  client.println("<div class='card-body center'>");
  client.println("<div class='error-title'>" + String(errorCode) + "</div>");
  client.println("<div class='error-message'>" + errorMessage + "</div>");
  client.println("<a href='/home/' class='error-button'>Home</a>");
  client.println("<a href='/' class='error-button'>Re-Login</a>");
  client.println(" </div></div></div>");
  client.print(ResourceHandler::getFooter());
  client.println("</body>");
  client.println("</html>");
}

void AdminServerManager::processAdmin(EthernetClient& client, const String& request,int contentLength, const String &authToken) {
    //check to see if allowed
    if(authToken != m_activeToken){
         handleError(client, 404, "Un-Authorized Access Request to /admin");
         return;
    }
    //check all of the form vars then update if complete
    handleError(client, 404, "PROCESS ADMIN NOT IMPLEMENTED");

}

void AdminServerManager::handleAdmin(EthernetClient& client, const String& request,int contentLength, const String &authToken) {
    //check to see if allowed
    if(authToken != m_activeToken){
         handleError(client, 404, "Un-Authorized Access Request to /admin");
         return;
    }
    //set the return headers
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println("Connection: close");
    //keep the the cookie
    client.println("Set-Cookie: authToken=" + m_activeToken + "; Path=/; Max-Age=86400"); // This sets the token as a cookie valid for 24 hours
    client.println("<!DOCTYPE HTML>");
    client.println("<html lang='en'>");
    client.print(ResourceHandler::getHeader("Admin Settings"));
    client.println("<body>");
    client.print(ResourceHandler::getHeaderMenu());
    client.println("<main class='main-content'>");
    String user_message = "Welcome please make sure to save your changes.";
    client.println("<br><p class='centered-message'>" + user_message + "</p><br>");
    String page =R"(
            <p class='centered-message'>Admin Settings</p>
            <div class='admin-container'>
                <form action='/admin' method='POST' class='admin-form'>
                    <div class='admin-card'>
                        <div class='admin-card-header'>Admin Credentials</div>
                        <div class='admin-card-content'>
                            <label for='Admin_USERNAME'>Username</label>
                            <input type='text' id='Admin_USERNAME' name='Admin_USERNAME' value='{{Admin_USERNAME}}' required>
                            <label for='Admin_PASSWORD'>Password</label>
                            <input type='password' id='Admin_PASSWORD' name='Admin_PASSWORD' value='{{Admin_PASSWORD}}' required>
                        </div>
                    </div>

                    <div class='admin-card'>
                        <div class='admin-card-header'>WiFi Settings</div>
                        <div class='admin-card-content'>
                            <label for='WIFI_SSID'>SSID</label>
                            <input type='text' id='WIFI_SSID' name='WIFI_SSID' value='{{WIFI_SSID}}' required>
                            <label for='WIFI_PASSWORD'>Password</label>
                            <input type='password' id='WIFI_PASSWORD' name='WIFI_PASSWORD' value='{{WIFI_PASSWORD}}' required>
                        </div>
                    </div>

                    <div class='admin-card'>
                        <div class='admin-card-header'>Network Settings</div>
                        <div class='admin-card-content'>
                            <label for='IP_ADDRESS'>IP Address</label>
                            <input type='text' id='IP_ADDRESS' name='IP_ADDRESS' value='{{IP_ADDRESS}}' required>
                            <label for='GATEWAY'>Gateway</label>
                            <input type='text' id='GATEWAY' name='GATEWAY' value='{{GATEWAY}}' required>
                            <label for='DNS'>DNS</label>
                            <input type='text' id='DNS' name='DNS' value='{{DNS}}' required>
                            <label for='SUBNET'>Subnet</label>
                            <input type='text' id='SUBNET' name='SUBNET' value='{{SUBNET}}' required>
                            <label for='MAC'>MAC Address</label>
                            <input type='text' id='MAC' name='MAC' value='{{MAC}}' required>
                        </div>
                    </div>

                    <div class='admin-card'>
                        <div class='admin-card-header'>Device Information</div>
                        <div class='admin-card-content'>
                            <label for='SERIAL_NUMBER'>Serial Number</label>
                            <input type='text' id='SERIAL_NUMBER' name='SERIAL_NUMBER' value='{{SERIAL_NUMBER}}' required>
                            <label for='MODEL'>Model</label>
                            <input type='text' id='MODEL' name='MODEL' value='{{MODEL}}' required>
                            <label for='REFRESH_RATE'>Refresh Rate</label>
                            <input type='number' id='REFRESH_RATE' name='REFRESH_RATE' value='{{REFRESH_RATE}}' required>
                            <label for='SHARED_SECRET'>Shared Secret</label>
                            <input type='text' id='SHARED_SECRET' name='SHARED_SECRET' value='{{SHARED_SECRET}}' required>
                            <label for='CALL_HOME_URL'>Call Home URL</label>
                            <input type='text' id='CALL_HOME_URL' name='CALL_HOME_URL' value='{{CALL_HOME_URL}}' required>
                            <label for='REGISTRATION_STATUS'>Registration Status</label>
                            <input type='checkbox' id='REGISTRATION_STATUS' name='REGISTRATION_STATUS' {{REGISTRATION_STATUS}} disabled>
                        </div>
                    </div>
                    <button type='submit' class='admin-submit-button'>Update Settings</button>
                </form>
            </div>
    )";

    // Replace placeholders with actual settings values
    page.replace("{{Admin_USERNAME}}", String(PortManager::getInstance().settings.Admin_USERNAME));
    
    page.replace("{{Admin_PASSWORD}}", String(PortManager::getInstance().settings.Admin_PASSWORD));
    page.replace("{{WIFI_SSID}}", String(PortManager::getInstance().settings.WIFI_SSID));
    page.replace("{{WIFI_PASSWORD}}", String(PortManager::getInstance().settings.WIFI_PASSWORD));
    page.replace("{{IP_ADDRESS}}", PortManager::getInstance().ipToString(PortManager::getInstance().settings.IP_ADDRESS));
    page.replace("{{GATEWAY}}", PortManager::getInstance().ipToString(PortManager::getInstance().settings.GATEWAY));
    page.replace("{{DNS}}", PortManager::getInstance().ipToString(PortManager::getInstance().settings.DNS));
    page.replace("{{SUBNET}}", PortManager::getInstance().ipToString(PortManager::getInstance().settings.SUBNET));
    page.replace("{{MAC}}", PortManager::getInstance().macToString(PortManager::getInstance().settings.MAC));
    page.replace("{{SERIAL_NUMBER}}", String(PortManager::getInstance().settings.SERIAL_NUMBER));
    page.replace("{{MODEL}}", String(PortManager::getInstance().settings.MODEL));
    page.replace("{{REFRESH_RATE}}", String(PortManager::getInstance().settings.REFRESH_RATE));
    page.replace("{{SHARED_SECRET}}", String(PortManager::getInstance().settings.SHARED_SECRET));
    page.replace("{{CALL_HOME_URL}}", String(PortManager::getInstance().settings.CALL_HOME_URL));
    page.replace("{{REGISTRATION_STATUS}}", PortManager::getInstance().settings.REGISTRATION_STATUS ? "checked" : "");
    
    client.println(page);
    client.println("</main>");
     //close page with footer
    client.print(ResourceHandler::getFooter());
    //close out body and page
    client.println("</body>");
    client.println("</html>");
}
void AdminServerManager::handleHomeLandingPage(EthernetClient& client, const String& request, String user_message){

    //would not be here unless authenticated
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println("Connection: close");
    client.println("Set-Cookie: authToken=" + m_activeToken + "; Path=/; Max-Age=86400"); // This sets the token as a cookie valid for 24 hours
    client.println();
    client.println("<!DOCTYPE HTML>");
    client.println("<html lang='en'>");
    client.print(ResourceHandler::getHeader("Agent Dashboard"));
    client.println("<body>");
    client.print(ResourceHandler::getHeaderMenu());
    
    client.println("<main class='main-content'>");
    client.println("<br><p class='centered-message'>" + user_message + "</p><br>");
     // Open stats container
    client.println("<div class='stats-container'>");
    client.print(ResourceHandler::getStatCard("Uptime", getUptimeString()));
    //memory stats
    FlashIAPLimits limits = getFlashIAPLimits();  // Get the current flash memory limits
    client.print(ResourceHandler::getStatCard("Flash Size", String(limits.flash_size)));
    client.print(ResourceHandler::getStatCard("Flash Available", String(limits.available_size)));
    client.print(ResourceHandler::getStatCard("Flash %", String((limits.available_size/limits.flash_size))));
    
    //Heap Stats
    //Heap Used: " + String(heap_stats.current_size) + " bytes (" + String(usagePercentage, 2) + "% of total reserved memory");
    mbed_stats_heap_t heap_stats;
    mbed_stats_heap_get(&heap_stats);
    
    float usagePercentage = (float)heap_stats.current_size / heap_stats.reserved_size * 100.0;
    client.print(ResourceHandler::getStatCard("Heap Used", String(heap_stats.current_size)));
    client.print(ResourceHandler::getStatCard("Heap %", String(usagePercentage)));

    //ports
    client.print(ResourceHandler::getStatCard("Ports Enabled", String(PortManager::getInstance().getActivePortCount())));
    // Close stats container
    client.println("</div>");

    client.println("  </main>");//close off main content
    //close page with footer
    client.print(ResourceHandler::getFooter());
    //close out body and page
    client.println("</body>");
    client.println("</html>");
}
