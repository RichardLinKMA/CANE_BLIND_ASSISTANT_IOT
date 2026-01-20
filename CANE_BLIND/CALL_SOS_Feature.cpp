/******************************************************************************
 * INCLUDES
 ******************************************************************************/
#include "CALL_SOS_Feature.h"
#include "SMS_Feature.h"
#include "GPS_Feature.h"


/******************************************************************************
 * GLOBAL VARIABLES
 ******************************************************************************/
/* Variables for button press tracking */
unsigned long pressStart = 0; /* Timestamp when button was pressed down */
bool isPressed = false;       /* Flag indicating if button is currently held */

/* Variables for software debouncing */
int lastRawState = HIGH;            /* Last raw (unfiltered) state read from the pin */
int stableState = HIGH;             /* Stable (debounced) state of the button */
unsigned long lastDebounceTime = 0; /* Last time the raw state was toggled */

/* SOS activation flag to prevent multiple triggers */
bool sosActive = false;

/******************************************************************************
 * PRIVATE FUNCTION PROTOTYPES
 ******************************************************************************/
void handleATPassthrough();

/*================================================================================================*/
/**
 * @brief       Handles AT command passthrough between Serial and GSM.
 * @details     Separated from SOS handling for cleaner code.
 *
 * @param[in]   None
 *
 * @return      void
 *
 * @api
 */
/*================================================================================================*/
void handleATPassthrough() {
    /* Forward AT commands from Serial Monitor (USB) to GSM/GNSS module */
    while (Serial.available() > 0) {
        gsmSerialPort.write(Serial.read());
        yield(); /* Allow background tasks to run */
    }

    /* Forward responses from GSM/GNSS module back to Serial Monitor (USB) */
    while (gsmSerialPort.available() > 0) {
        Serial.write(gsmSerialPort.read());
        yield(); /* Allow background tasks to run */
    }
}

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

int debounceRead(int pin) {
  int currentRawState = digitalRead(pin); /* Read the raw (unfiltered) state from the pin */

  /* If the raw state changes compared to the last raw read, 
     reset the debounce timer to start measuring stability time */
  if (currentRawState != lastRawState) {
    lastDebounceTime = millis();
  }

  /* If the signal has remained unchanged for longer than DEBOUNCE_DELAY,
     update the stable state to reflect the new value */
  if ((millis() - lastDebounceTime) > DEBOUNCE_DELAY) {
    if (currentRawState != stableState) {
      stableState = currentRawState;
    }
  }

  /* Store the raw state for comparison in the next function call */
  lastRawState = currentRawState;

  /* Return the stable, debounced state of the button */
  return stableState;
}

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
void dialPhoneNumber(String phoneNumber) {
  /* Send ATD command to start dialing */
  gsmSerialPort.print("ATD");
  gsmSerialPort.print(phoneNumber);
  gsmSerialPort.println(";");  // End AT command for dialing

  /* Read the response from GSM module with specified timeout */
  readGsmResponse(GSM_CALL_RESPONSE_TIMEOUT_MS);
}

/*================================================================================================*/
/**
 * @brief       Handles SOS button logic and performs AT-command passthrough.
 *
 * @details     This API performs two main functions:
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
                                bool wifiSuccess, const char* ssid) {
    // Check if SOS is already active (prevent re-triggering)
    if (sosActive) {
        // Still allow AT passthrough even during SOS
        handleATPassthrough();
        return;
    }
    
    int stableState = debounceRead(buttonInputPin);

    /* Detect button press (falling edge) */
    if (stableState == LOW && !isPressed) {
        pressStart = millis();
        isPressed = true;
    }
    /* Detect button release (rising edge) */
    else if (stableState == HIGH && isPressed) {
        unsigned long pressDuration = millis() - pressStart;

        /* Handle short press states */
        if (pressDuration < 300) {
            Serial.println("State 1 detected");
        } else if (pressDuration <= 650) {
            Serial.println("State 2 detected");
        } else {
            /* ------------------- SOS Activation (Highest Priority) ------------------- */
            Serial.println("SOS button long press detected!");
            sosActive = true; // Set flag to prevent re-triggering
            
            // Immediately stop any heart rate emergency handling
            // Note: We'll handle this after HEART_Feature.h is included
            
            beepBuzzer(2);

            // /* Step 1: Show SOS call screen */
            // TFT_ShowSOSCallScreen(SOS_PHONE_NUMBER);
            
            /* Step 2: Dial SOS number */
            dialPhoneNumber(SOS_PHONE_NUMBER);

            // /* Step 3: Show SMS sending screen */
            // TFT_ShowSOSSMSScreen();
            
            String mapLink = parseGpsToMapLink();

            if (mapLink.length() > 0) {
                String message = "SOS! My location: " + mapLink;
                sendTextMessage(SOS_PHONE_NUMBER, message.c_str());
            } else {
                sendTextMessage(SOS_PHONE_NUMBER, "SOS! Unable to get GPS location.");
            }
            
            // /* Step 4: Show completion screen */
            // TFT_ShowSOSCompleteScreen();
            
            /* Step 5: Delay then return to main screen */
            delay(3000); // Show completion screen for 3 seconds
            
            /* Return to ready screen with current WiFi status */
            // Note: We need access to wifiConnectedStatus and connectedSSID
            // This will be handled by the main program

    //         /* Return to main heart rate screen */
    // TFT_ReturnToMainScreen(wifiSuccess, ssid);
            
            // Reset SOS active flag after completion
            sosActive = false;
        }
        isPressed = false;
    }

    /* Always handle AT passthrough */
    handleATPassthrough();
}