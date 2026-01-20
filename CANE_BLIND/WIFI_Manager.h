#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H
/******************************************************************************
 * INCLUDES
 ******************************************************************************/
#include <WiFi.h>
#include <Preferences.h>
#include "Generic_API.h"

/*================================================================================================*/
/**
* @class WifiManagerCustom
* @brief Manages the persistent storage and connection logic for Wi-Fi credentials on the ESP32.
* @details This class provides methods to load, save, and attempt connections using Wi-Fi credentials 
*          stored in the Non-Volatile Storage (NVS) using the Preferences library. It abstracts the 
*          connection process, including retry attempts.
*
* @api
*/
/*================================================================================================*/
class WifiManagerCustom {
public:
    /*============================================================================================*/
    /**
    * @brief        Default constructor for the WifiManagerCustom class.
    * @details      Typically used to initialize the Preferences object (if needed) or other internal 
    *               member variables.
    *
    * @param[in]    None
    * @param[out]   N/A
    *
    * @return       N/A
    */
    /*============================================================================================*/
    WifiManagerCustom();

    /*============================================================================================*/
    /**
    * @brief        Loads saved Wi-Fi credentials from NVS memory.
    * @details      Retrieves the SSID and Password stored under the "wifi" namespace in NVS and 
    *               stores them in the public member variables (savedSSID, savedPASS).
    *
    * @return       bool      Returns true upon successful completion of the loading process.
    */
    /*============================================================================================*/
    bool loadCredentials();

    /*============================================================================================*/
    /**
    * @brief        Saves new Wi-Fi credentials to NVS memory.
    * @details      Writes the provided SSID and Password to the "wifi" namespace in NVS. 
    *               This method is typically called after the WebPortal receives new input.
    *
    * @param[in]    ssid        The new SSID string to be stored.
    * @param[in]    pass        The new Password string to be stored.
    *
    * @return       void
    */
    /*============================================================================================*/
    void saveCredentials(String ssid, String pass);

    /*============================================================================================*/
    /**
    * @brief        Attempts to connect to the saved Wi-Fi network with retries.
    * @details      Calls tryConnect() repeatedly (up to MAX_TRIES) using the credentials stored 
    *               in savedSSID and savedPASS, waiting for CONNECT_DELAY between attempts. 
    *               
    *
    * @return       bool      Returns true if connected successfully, false if all attempts fail.
    */
    /*============================================================================================*/
    bool connectWiFi();

    /*============================================================================================*/
    /**
    * @brief        Performs a single, timed attempt to connect to a specified Wi-Fi network.
    * @details      Sets Wi-Fi mode to Station (WIFI_STA) and checks the connection status 
    *               for a short duration. This is a helper function for connectWiFi().
    *
    * @param[in]    ssid        The target SSID.
    * @param[in]    pass        The target Password.
    *
    * @return       bool      Returns true if connection is established, false otherwise.
    */
    /*============================================================================================*/
    bool tryConnect(String ssid, String pass);

    /* The most recently loaded or saved SSID. Publicly accessible by the WebPortal. */
    String savedSSID;
    /* The most recently loaded or saved Password. Publicly accessible by the WebPortal. */
    String savedPASS;

private:
    /* ESP32 NVS (Non-Volatile Storage) library instance for persistent storage of credentials. */
    Preferences prefs;
};

#endif