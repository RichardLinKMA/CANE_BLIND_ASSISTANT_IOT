#ifndef GPS_FEATURE_H
#define GPS_FEATURE_H

/******************************************************************************
 * INCLUDES
 ******************************************************************************/
#include "Generic_API.h"

/******************************************************************************
 * GLOBAL VARIABLES
 ******************************************************************************/
/* Global system timestamp (milliseconds) */
extern unsigned long systemCurrentTimeMs;

/* Variable to store the final generated Google Maps URL */
extern String googleMapUrl;

/******************************************************************************
 * FUNCTIONS PROTOTYPES
 ******************************************************************************/

/*================================================================================================*/
/**
* @brief        Parses GPS data and returns different preset Google Maps URLs based on call count.
* @details      This function maintains an internal static counter. Each time the function is called,
*               the counter increases by 1 (cycles from 0 → 9), and the function returns a different
*               Google Maps URL according to the counter value. Used to test different URL outputs
*               without real GPS data.
*
* @param[in]    None
* @param[out]   None
*
* @return       String      A Google Maps URL selected based on the internal counter.
*
* @api
*/
/*================================================================================================*/
String parseGpsToMapLink();

/*================================================================================================*/
/**
* @brief        Processes GPS map-link generation and performs AT-command passthrough.
* @details      This function invokes the GPS parsing routine to generate a Google Maps URL
*               from the most recently available GNSS data (currently a static or externally
*               updated source). The function does not request new GNSS data nor handle timing
*               intervals.  
*
*               Additionally, it operates in full AT-command passthrough mode:
*               - Forwards all characters received from the USB Serial Monitor to the GSM/GNSS module.
*               - Forwards all data received from the GSM/GNSS module back to the USB Serial Monitor.
*               This allows real-time debugging and manual AT interaction.
*
* @param[in,out] systemCurrentTimeMs   Unused parameter; reserved for future GPS scheduling logic.
* @param[in]     intervalMs            Unused parameter; reserved for periodic GNSS request handling.
* @param[in]     debug                 If true, may be used in future expansion for debug printing.
* @param[out]    None
*
* @return        void
*
* @api
*/
/*================================================================================================*/

void requestGpsLocation(unsigned long &systemCurrentTimeMs, unsigned long intervalMs, bool debug);
#endif