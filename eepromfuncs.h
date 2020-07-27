#pragma once
#ifdef USE_EEPROM

int iEEPromAddr = 20;

#include <EEPROM.h>
//-----------hard coded settings- must change!!------------------------
#define EEPROM_SETTINGS_VER 3 // checks for this version and if it doesnt find it, initialises all values to default
struct StoreStruct {
	int version;   // This is to detect if they are your settings or settings are corrupt, if so, re-initilise them to defaults
	float iTargetWeight;
	float izzzzzz;
	//scale control
	int iNumberofReadingsQuick ;// 50 readings takes 1 second but is more accurate
	float DispensingStopNear;// stop just before the target 
	//conversion to other units
	float iConvertToGrain ;
	float iConvertGrainToGrams;
	int igain; // gain: 128 or 64 for channel A; channel B works with 32 gain factor only
	 int iNumberofReadingsSlow  ;// 50 readings takes 1 second but is more accurate
	bool bSlowMode;
	
	int iTareNumberOfReadings = 30;
	int iTareNumberOfLoops = 2;
	int DispensingPauseDuration; /// the duration of the pause to give the scale time to rest

	float iCalibweight = 180; //grains. typical weight used to calibrate
	bool bzzzzSmoothReadings;
	int DispensingPauseInterval; //  pause every few seconds while dispensing 
	bool  bShowGrams;


	 //put new values here at the end to avoid corrupting old settings
	//------------------->>>>>>>>
}  eepromStore;


void fn_EEPROMSave() {

//	if (bdebugSerial) Serial.println("saving to EEPROM...");
	EEPROM.put(iEEPromAddr, eepromStore);

//	if (bdebugSerial) Serial.println("...saved to EEPROM, committing");
	//fn_eepromCopyValsAcross();
	EEPROM.commit();

	//if (bdebugSerial) Serial.println("done saving, exiting.");
	// if (bdebugSerial) Serial.println("Saved to EEPROM: Server:" + sServerIP());
}

void fn_EEPROM_check() {

	eepromStore.iTareNumberOfReadings = constrain(eepromStore.iTareNumberOfReadings, 1, 300);
	eepromStore.iTareNumberOfLoops = constrain(eepromStore.iTareNumberOfLoops, 1, 30);
	//eepromStore.iDelayBetweenReadings = constrain(eepromStore.iDelayBetweenReadings, 0, 800);
	eepromStore.iCalibweight = constrain(eepromStore.iCalibweight, 1, 1000);
	//eepromStore.SmootheOvermS = constrain(eepromStore.SmootheOvermS, 1, 30000);


	if (eepromStore.version != EEPROM_SETTINGS_VER) {
		//initialise all settings to default
		eepromStore.version = EEPROM_SETTINGS_VER;
		eepromStore.iNumberofReadingsQuick = 57;// 50 readings takes 1 second but is more accurate
		eepromStore.iNumberofReadingsSlow = 120;// 50 readings takes 1 second but is more accurate
//		eepromStore.iRoundingVal = 69;// higher value means less drift, but then you cant detect small items
		//conversion to other units
		eepromStore.iConvertToGrain = -4.02315;
		eepromStore.iConvertGrainToGrams = 15.43236073;
		eepromStore.igain = 128; // gain: 128 or 64 for channel A; channel B works with 32 gain factor only
		bool bSlowMode;
	//now save
		fn_EEPROMSave();
	}


}

void fn_EEPROMLoad() {	
	EEPROM.get(iEEPromAddr, eepromStore);

	fn_EEPROM_check();
} 

#endif
