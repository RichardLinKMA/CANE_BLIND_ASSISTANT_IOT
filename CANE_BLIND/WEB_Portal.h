/* WebPortal.h - Updated Version */

#ifndef WEB_PORTAL_H
#define WEB_PORTAL_H
/******************************************************************************
 * INCLUDES
 ******************************************************************************/
#include <WebServer.h>
#include <DNSServer.h>
#include "WIFI_Manager.h"

/******************************************************************************
 * API
 ******************************************************************************/
/*================================================================================================*/
/**
* @class WebPortal
* @brief Implements a Captive Web Portal for managing and configuring Wi-Fi credentials on an ESP32 device.
* @details This class orchestrates the Access Point (AP) mode, DNS Captive mechanism, and a simple 
* HTTP server to provide a web interface for users to enter their home Wi-Fi details. 
* It relies on the WifiManagerCustom class for persistent storage and connection logic.
*
* @api
*/
/*================================================================================================*/
class WebPortal {
public:
    /*============================================================================================*/
    /**
    * @brief        Constructor for the WebPortal class.
    * @details      Initializes the WebServer instance on port 80 and stores a pointer to the 
    *               WifiManagerCustom instance for credential management.
    *
    * @param[in]    wifiManager    Pointer to the custom Wi-Fi manager instance.
    * @param[out]   N/A
    *
    * @return       N/A
    */
    /*============================================================================================*/
    WebPortal(WifiManagerCustom* wifiManager);

    /*============================================================================================*/
    /**
    * @brief        Starts the Captive Web Portal process.
    * @details      Sets the Wi-Fi mode to AP, configures network settings, starts the SoftAP, 
    *               initializes the DNS Captive server, defines all HTTP routes, and begins the web server.
    *
    * @return       void
    */
    /*============================================================================================*/
    void startPortal();

    /*============================================================================================*/
    /**
    * @brief        Handles all incoming client requests (DNS and HTTP).
    * @details      Must be called repeatedly within the application's main loop (loop()) to ensure 
    *               responsiveness for both DNS redirection and web page/form handling.
    *
    * @return       void
    */
    /*============================================================================================*/
    void handleClient();

private:
    /* Pointer to the external Wi-Fi management class for saving and loading credentials. */
    WifiManagerCustom* wifi;
    /* The HTTP web server instance (runs on port 80). */
    WebServer server;
    /* The DNS server instance used for the Captive Portal mechanism (runs on port 53). */
    DNSServer dns;

    /*============================================================================================*/
    /**
    * @brief        Handler for HTTP GET requests to the root path ('/').
    * @details      Sends the main configuration form HTML page to the client.
    *
    * @return       void
    */
    /*============================================================================================*/
    void handleRoot();

    /*============================================================================================*/
    /**
    * @brief        Handler for HTTP POST requests to '/save'.
    * @details      Processes form data, saves new Wi-Fi credentials, sends a confirmation page, and restarts the device.
    *
    * @return       void
    */
    /*============================================================================================*/
    void handleSave();

    /*============================================================================================*/
    /**
    * @brief        Handler for HTTP GET requests to '/style.css'.
    * @details      Sends the embedded CSS stylesheet content to the client.
    *
    * @return       void
    */
    /*============================================================================================*/
    void handleCSS(); 
};

#endif