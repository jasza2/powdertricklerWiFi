#pragma once
#define USE_ADS
//#define USE_HX711 ---- could get stable readings

#ifdef USE_ADS
#include "ADS123X.h"

//pins
#define SCALE_DOUT   27
#define SCALE_SCLK   26
#define SCALE_PDWN   25
#define SCALE_GAIN0  33
#define SCALE_GAIN1  32
#define SCALE_SPEED  35
#define SCALE_A0     34
#define SCALE_A1     14 //clkin???

ADS123X scale;
Channel iChan = AIN1;


unsigned int ilastscalereadmillis;
int iScaleErrCount;


void scale_setup() {

	Serial.println("ADS123X setup");
	scale.begin(SCALE_DOUT, SCALE_SCLK, SCALE_PDWN, SCALE_GAIN0, SCALE_GAIN1, SCALE_SPEED, SCALE_A0, SCALE_A1);
	scale.setGain(GAIN64);
	scale.power_up();
  scale.setSpeed(FAST);
	scale.set_scale(iChan, eepromStore.iConvertToGrain);        // this value is obtained by calibrating the scale with known weights; see the README for details
  
    scale_tare();
}

void scale_tare() {
#ifdef USE_OLED
    fn_oledText("Tare...", 4);
#endif
    fn_dispensing_off();
    Serial.println("Tare."); 
    if (eepromStore.iTareNumberOfReadings <= 0)eepromStore.iTareNumberOfReadings = 1;
    scale.tare(iChan,eepromStore.iTareNumberOfReadings,false);				        // reset the scale to 0
   
}

void scale_slow(bool bslow) {
 if (bslow)  scale.setSpeed(SLOW); else   scale.setSpeed(FAST);
}

float fn_convert_Grain_to_Gram(float ival) {
    if (eepromStore.iConvertGrainToGrams == 0)eepromStore.iConvertGrainToGrams = 1;
    return ival / eepromStore.iConvertGrainToGrams;
}

// reads the scale, returns true if an error
bool scale_read() {
     ilastscalereadmillis = millis();
    fn_oledot(true); //show a dot
    int inoreadings = eepromStore.iNumberofReadingsQuick;
    if (bSlowMode)inoreadings = eepromStore.iNumberofReadingsSlow;
    if (inoreadings <= 0)inoreadings = 1;
    Serial.print("getting readings " + String(inoreadings) + " ... ");
    scale.get_units(iChan,fgrains , inoreadings);
   fgrams = fn_convert_Grain_to_Gram(fgrains); 
  fn_oledot(false);
    return false;
}


void scale_setGrainConv(float ival) { 
    scale.set_scale(iChan,ival);
    Serial.println("Set scale conv. to grains to :" + String(ival));
}

void scale_calib(float ivaltocal) {
    //get the raw readings
    float iraw;
    scale.get_value(iChan, iraw, eepromStore.iTareNumberOfReadings);

    eepromStore.iConvertToGrain = iraw / ivaltocal;
    scale_setGrainConv(eepromStore.iConvertToGrain); 
}
#endif
