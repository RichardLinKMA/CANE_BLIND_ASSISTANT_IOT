/******************************************************************************
 * INCLUDES
 ******************************************************************************/
#include "GPS_Feature.h"

/******************************************************************************
 * GLOBAL VARIABLES
 ******************************************************************************/
/* Variable to store the final generated Google Maps URL */
String googleMapUrl = "";

/*================================================================================================*/
/**
* @brief        Parses GPS data and returns different preset Google Maps URLs based on call count
* @details      This function maintains an internal static counter. Each time the function is called,
*               the counter increases by 1 (cycles from 0 → 9), and the function returns a different
*               Google Maps URL according to the counter value. Used to test different URL outputs
*               without real GPS data
*
* @param[in]    None
* @param[out]   None
*
* @return       String      A Google Maps URL selected based on the internal counter
*
* @api
*/
/*================================================================================================*/
String parseGpsToMapLink(void)
{
    /* Static counter to track how many times the function is called (0 → 9) */
    static int count = 0;
    
    /* Base coordinates: Hanoi University of Electric Power */
    /* 235 Hoang Quoc Viet, Co Nhue 1, Bac Tu Liem, Ha Noi */
    const double baseLat = 21.046722;   /* Base latitude */
    const double baseLon = 105.785222;  /* Base longitude */
    
    /* VERY SMALL fluctuation ~1-2 meters */
    /* 0.000009 degrees ≈ 1 meter at Hanoi */
    const double maxOffset = 0.000018;   /* ~2 meters maximum */
    
    /* Create 10 coordinates with VERY small fluctuations around base point */
    static double offsetsLat[10];
    static double offsetsLon[10];
    static bool initialized = false;
    
    /* Initialize random offset array once */
    if (!initialized) {
        randomSeed(analogRead(0) + micros());  /* Use better random seed */
        
        Serial.println("[GPS] Generating 10 coordinates around EPU Hanoi (±2m):");
        
        for (int i = 0; i < 10; i++) {
            /* Generate random offset within ±2m range */
            /* random(0, 37) - 18: generate number from -18 to +18 (equivalent to ±0.000018 degrees) */
            double latOffset = (random(0, 37) - 18) / 1000000.0;  /* ±0.000018 degrees */
            double lonOffset = (random(0, 37) - 18) / 1000000.0;  /* ±0.000018 degrees */
            
            /* Ensure not exceeding 2m */
            if (fabs(latOffset) > maxOffset) {
                latOffset = (latOffset > 0) ? maxOffset : -maxOffset;
            }
            if (fabs(lonOffset) > maxOffset) {
                lonOffset = (lonOffset > 0) ? maxOffset : -maxOffset;
            }
            
            offsetsLat[i] = baseLat + latOffset;
            offsetsLon[i] = baseLon + lonOffset;
            
            /* Calculate distance from base point */
            double distance = sqrt(latOffset * latOffset + lonOffset * lonOffset) * 111319.9;
            
            Serial.printf("[GPS] Point %d: %.6f, %.6f (dist: %.2fm)\n", 
                         i, offsetsLat[i], offsetsLon[i], distance);
        }
        initialized = true;
    }
    
    /* Create Google Maps URL with current coordinates */
    String googleMapUrl = "https://www.google.com/maps?q=";
    googleMapUrl += String(offsetsLat[count], 6);
    googleMapUrl += ",";
    googleMapUrl += String(offsetsLon[count], 6);
    
    /* Calculate distance from base point for debug */
    double latOffset = offsetsLat[count] - baseLat;
    double lonOffset = offsetsLon[count] - baseLon;
    double distance = sqrt(latOffset * latOffset + lonOffset * lonOffset) * 111319.9;
    
    /* Debug output */
    Serial.printf("[GPS] Sending location %d: %.6f, %.6f (%.2fm from center)\n", 
                 count, offsetsLat[count], offsetsLon[count], distance);
    
    /* Increment count and wrap around */
    count = (count + 1) % 10;
    
    /* Return URL */
    return googleMapUrl;
}

/*================================================================================================*/
/**
* @brief        Processes GPS map-link generation and performs AT-command passthrough
* @details      This function invokes the GPS parsing routine to generate a Google Maps URL
*               from the most recently available GNSS data (currently a static or externally
*               updated source). The function does not request new GNSS data nor handle timing
*               intervals
*
*               Additionally, it operates in full AT-command passthrough mode:
*               - Forwards all characters received from USB Serial Monitor to GSM/GNSS module
*               - Forwards all data received from GSM/GNSS module back to USB Serial Monitor
*               This allows real-time debugging and manual AT interaction
*
* @param[in,out] systemCurrentTimeMs   Unused parameter; reserved for future GPS scheduling logic
* @param[in]     intervalMs            Unused parameter; reserved for periodic GNSS request handling
* @param[in]     debug                 If true, may be used in future expansion for debug printing
* @param[out]    None
*
* @return        void
*
* @api
*/
/*================================================================================================*/
void requestGpsLocation(unsigned long &systemCurrentTimeMs, unsigned long intervalMs, bool debug) {
    /* Parse the GPS response into a map link (custom function) */
    parseGpsToMapLink();

  /* Forward data from debug Serial (USB) to GSM serial port */
  while (Serial.available() > 0) {
    gsmSerialPort.write(Serial.read());
    yield(); /* Allow background tasks to run */
  }

  /* Forward data from GSM serial port to debug Serial (USB) */
  while (gsmSerialPort.available() > 0) {
    Serial.write(gsmSerialPort.read());
    yield(); /* Allow background tasks to run */
  }
}