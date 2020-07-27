

#define  USE_OLED
#define USE_EEPROM
#define USE_WEBSERVER
#define USE_NETWORK

#define TARE_PIN 5
#define DISPENSE_ONOFF_PIN 17 
#define TARGET_PIN 16

//for external OLED:
#define SDA_PIN 21 // (white/blue (middle) on tf mini) - outside pin on our boards
#define SCL_PIN 22 // (green/yellow (outer) on tfmini)

//for wemos built in oled
//#define SDA_PIN 5 // (white/blue (middle) on tf mini) - outside pin on our boards
//#define SCL_PIN 4 // (green/yellow (outer) on tfmini)
 
//------------------------------- forward declare --------------------------//
void scale_tare();
void     fn_oledot(bool);
void fn_oledText(String , int );
void fn_PWMSend(int);
void fn_dispensing_off();
//-------------------------------END OF forward declare --------------------------//

#ifdef USE_EEPROM
    #include "eepromfuncs.h" 
#endif

bool bdebugSerial = true;

int iPWMmin = 500;
int iPWMmax = 3900;

float fgrains;
float fgrams;

bool bAutoDispensing;
bool bSlowMode = true; 
bool bTareHapened = false; 
String sDispensingStatus = "";

  #include "scalefuncs.h"
    #include "dispensingfuncs.h"
    #include "pwmfuncs.h"

#ifdef USE_OLED
    #include "displayfuncs.h"
#endif  

#ifdef USE_NETWORK
    String DEFAULT_SSID = "SCALE_01";
    String DEFAULT_PWD = "12345678";
    String sIP = ""; 
    #include "networkfuncs.h"
#endif

#if defined (USE_WEBSERVER)
    #include "websitefuncs.h"
#endif

  
void setup() {
    Serial.begin(115200);
    if (bdebugSerial) Serial.println("HX711 calibration sketch");

#ifdef USE_EEPROM
   EEPROM.begin(512);
   fn_EEPROMLoad();
   bSlowMode = eepromStore.bSlowMode; 
#endif 

#ifdef USE_OLED 
    if (bdebugSerial) Serial.println("setup oled");
    fn_SetupOLED();
    fn_oledText("Start..");
#endif 

    if (bdebugSerial) Serial.println("setup pins");
    pinMode(TARE_PIN, INPUT_PULLUP);
    pinMode(DISPENSE_ONOFF_PIN, INPUT_PULLUP);
    pinMode(TARGET_PIN, INPUT_PULLUP);

    scale_setup();
 
#ifdef USE_NETWORK 
        #ifdef USE_OLED	
            fn_oledText(DEFAULT_SSID + " " + DEFAULT_PWD );
        #endif
            setupWifi();
#endif 

#ifdef USE_WEBSERVER 
    fn_SetupWebsite();
#endif 

    fn_PWMSetup();

    if (bdebugSerial) Serial.println("setup done"); 
}


unsigned int ilastpinread;
void fn_ReadPins() {
    if (digitalRead(TARE_PIN) == LOW) {
        if (millis() - ilastpinread > 50) {
            scale_tare();
            bTareHapened = true;
       }
     ilastpinread = millis();
    } 

    if (digitalRead(DISPENSE_ONOFF_PIN) == LOW) {
       if (millis() - ilastpinread > 50) {
          bAutoDispensing =!bAutoDispensing ;
          if (bAutoDispensing) {
                  fn_dispensing_on(true);//this is the tare to start dispensing
          } else {
              fn_dispensing_on(true);//this is the tare to start dispensing
          }         
       }
     ilastpinread = millis(); 
    
    } 
    if (digitalRead(TARGET_PIN) == LOW) {
        fn_setTarget();
    }  
} 


void loop() {


#ifdef USE_WEBSERVER	
    server.handleClient();
#endif
     fn_ReadPins();
      scale_read();//puts the current weight into  currentresult
#ifdef USE_OLED
    fn_oledStatus(false);
#endif 
    fn_checkdispensing(); 
#ifdef USE_WEBSERVER	
    server.handleClient();
#endif	
}
