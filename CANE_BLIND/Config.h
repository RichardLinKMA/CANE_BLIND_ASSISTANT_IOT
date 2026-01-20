#ifndef CONFIG_H
#define CONFIG_H

/******************************************************************************
 * WIFI CREDENTIALS
 ******************************************************************************/
/* Default WiFi network credentials */
#define DEFAULT_SSID   "VIETTEL"
#define DEFAULT_PASS   "26011990"

/* Access Point credentials for configuration portal */
#define AP_SSID        "ESP32-Setup"
#define AP_PASS        "12345678"

/******************************************************************************
 * CONNECTION SETTINGS
 ******************************************************************************/
/* Maximum number of connection attempts */
#define MAX_TRIES      5

/* Delay between connection attempts in milliseconds */
#define CONNECT_DELAY  2000

#endif /* CONFIG_H */