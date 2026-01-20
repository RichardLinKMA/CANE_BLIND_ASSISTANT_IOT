#ifndef CALL_SOS_FEATURE_H
#define CALL_SOS_FEATURE_H

/******************************************************************************
 * INCLUDES
 ******************************************************************************/
#include "Generic_API.h"

/******************************************************************************
 * MACROS
 ******************************************************************************/
/* Debounce delay time in milliseconds to prevent false multiple detections when the button is pressed */
#define DEBOUNCE_DELAY 50    

/* GPIO pin number where the button is connected */
#define BUTTON_PIN 35

/* Define default timeout for reading GSM response when dialing a phone (in milliseconds) */
#define GSM_CALL_RESPONSE_TIMEOUT_MS 5000

/******************************************************************************
 * GLOBAL VARIABLES
 ******************************************************************************/
extern unsigned long pressStart;      /* Timestamp when button was pressed down */
extern bool isPressed;                /* Flag indicating if button is currently held */
extern int lastRawState;              /* Last raw state read from the button pin */
extern int stableState;               /* Stable debounced state of the button */
extern unsigned long lastDebounceTime;/* Last debounce timestamp */

/******************************************************************************
 * FUNCTIONS PROTOTYPES
 ******************************************************************************/

/*================================================================================================*/
/**
* @brief        Reads and debounces the state of a digital input pin.
* @details      Reads the raw state of the specified pin and applies a debouncing algorithm to 
*               filter out noise. The function tracks the stability of the input signal over time 
*               and updates the stable state only if the signal remains unchanged for longer than 
*               the defined DEBOUNCE_DELAY. The stable state is returned.
*
* @param[in]    pin             The digital pin to read.
* @param[out]   None
*
* @return       int             The stable, debounced state of the pin.
*
* @api
*/
/*================================================================================================*/
int debounceRead(int pin);

/*================================================================================================*/
/**
* @brief        Handles SOS button logic and performs AT-command passthrough.
*
* @details      This API performs two main functions:
*
*               1. **SOS Button Handling**
*                  - Uses a debounced digital input to detect button events.
*                  - Detects short-press states based on press duration and logs the state.
*                  - Detects long press (SOS activation):
*                        + Triggers buzzer feedback.
*                        + Initiates a call to the predefined SOS number.
*                        + Attempts to retrieve the GPS position and generate a Google Maps link.
*                        + Sends an SMS containing either the location link or an error message.
*
*               2. **AT Command Passthrough**
*                  - Forwards any text entered into the Serial Monitor (USB) directly to the GSM/GNSS module.
*                  - Forwards all responses from the GSM/GNSS module back to the Serial Monitor.
*                  - Allows real-time manual interaction/debugging using AT commands.
*
* @param[in]    buttonInputPin       Digital input pin connected to the SOS button.
* @param[in]    enableDebugMessages  If true, prints internal debug messages to Serial Monitor.
*
* @return       None
*
* @api
*/
/*================================================================================================*/

void handleCallAndATPassthrough(int buttonInputPin, bool enableDebugMessages, 
                                bool wifiSuccess = false, const char* ssid = "");

/*================================================================================================*/
/**
* @brief        Dials a phone number via the GSM module.
* @details      Sends the AT command to dial the specified phone number using the GSM module.
*               Waits for a defined timeout while reading the module's response. Optionally
*               prints the response to the Serial monitor.
*
* @param[in]    phoneNumber     The phone number to dial, as a String.
* @param[out]   None
*
* @return       None
*
* @note         Uses gsmSerialPort as the GSM UART port.
*/
/*================================================================================================*/
void dialPhoneNumber(String phoneNumber);

#endif /* CALL_SOS_FEATURE_H */