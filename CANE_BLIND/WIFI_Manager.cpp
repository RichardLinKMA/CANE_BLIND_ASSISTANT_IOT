/******************************************************************************
 * INCLUDES
 ******************************************************************************/
#include "WIFI_Manager.h"
#include "Generic_API.h"

/******************************************************************************
 * GLOBAL VARIABLES
 ******************************************************************************/
WifiManagerCustom::WifiManagerCustom() {}

/******************************************************************************
 * api
 ******************************************************************************/
/*================================================================================================*/
/**
* @brief        Loads saved Wi-Fi credentials from the non-volatile memory (NVS/Preferences).
* @details      Opens the "wifi" namespace in read-only mode, retrieves the stored SSID and Password, 
*               falling back to DEFAULT_SSID/DEFAULT_PASS if not found. The retrieved values are 
*               stored in the class member variables (savedSSID, savedPASS).
*
* @param[in]    None
* @param[out]   None (Data loaded into savedSSID, savedPASS members)
*
* @return       bool      Returns true upon completion.
*
* @api
*/
/*================================================================================================*/
bool WifiManagerCustom::loadCredentials() {
  /* Begin the Preferences library session in read-only mode (true). */
  prefs.begin("wifi", true);
  /* Load SSID from NVS; use DEFAULT_SSID if the key "ssid" is not found. */
  savedSSID = prefs.getString("ssid", DEFAULT_SSID);
  /* Load Password from NVS; use DEFAULT_PASS if the key "pass" is not found. */
  savedPASS = prefs.getString("pass", DEFAULT_PASS);
  /* End the Preferences session. */
  prefs.end();
  /* Indicate successful loading (regardless of whether defaults were used). */
  return true;
}

/*================================================================================================*/
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
void WifiManagerCustom::saveCredentials(String ssid, String pass) {
  /* Begin the Preferences library session in read/write mode (false). */
  prefs.begin("wifi", false);
  /* Save SSID to NVS */
  prefs.putString("ssid", ssid);
  /* Save Password to NVS */
  prefs.putString("pass", pass);
  /* End the Preferences session and commit changes. */
  prefs.end();
}

/*================================================================================================*/
/**
* @brief        Attempts to connect to the saved Wi-Fi network with retries.
* @details      Calls tryConnect() repeatedly (up to MAX_TRIES) using the credentials stored 
*               in savedSSID and savedPASS, waiting for CONNECT_DELAY between attempts. 
*               
*
* @return       bool      Returns true if connected successfully, false if all attempts fail.
*/
/*============================================================================================*/
bool WifiManagerCustom::connectWiFi() {
  for (int attempt = 1; attempt <= MAX_TRIES; attempt++) {
    Serial.print("WiFi connection attempt ");
    Serial.print(attempt);
    Serial.print(" of ");
    Serial.println(MAX_TRIES);

    if (tryConnect(savedSSID, savedPASS)) {
      Serial.println("WiFi connected successfully!");
      return true;
    }

    if (attempt < MAX_TRIES) {
      Serial.printf("Try %d failed…\n", attempt);
      delay(CONNECT_DELAY);
    }
  }

  Serial.println("All WiFi connection attempts failed.");
  return false;
}

/*================================================================================================*/
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
bool WifiManagerCustom::tryConnect(String ssid, String pass) {
  /* Set WiFi mode to Station */
  WiFi.mode(WIFI_STA);

  /* Begin connection attempt */
  WiFi.begin(ssid.c_str(), pass.c_str());

  for (int i = 0; i < 10; i++) {
    if (WiFi.status() == WL_CONNECTED) return true;
    delay(500);
  }
  return false;
}