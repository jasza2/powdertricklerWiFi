#pragma once

void fn_dispensing_off() {
    fn_PWMSend(0);
    sDispensingStatus = "Stopped";

}



//dont check too often
unsigned int iLastCheckdispensingON;
unsigned int iLastDispensingPauseStopped; 
bool bDispensingIsPaused; 

void fn_dispensing_on(bool bForceon) {  
    
    if (!bForceon and  millis() - iLastCheckdispensingON <= 100)return;
    iLastCheckdispensingON = millis();

    int idiffpc;
    if (fgrains > 0) {
        //work out the difference from target as a percentage
        idiffpc = abs((eepromStore.iTargetWeight - fgrains) / eepromStore.iTargetWeight)*100;
    }    else {
        idiffpc = 100;
    }

    Serial.println("Target: " + String(eepromStore.iTargetWeight) + " grains: " + String(fgrains) + " dif: " + String(eepromStore.iTargetWeight - fgrains) + " difpc: " + String(idiffpc));
 if (idiffpc >= 10) {
        sDispensingStatus = String(idiffpc) + " Fast";
    }
    else {
        sDispensingStatus = String(idiffpc) + "Slow";
    }
   

    int ipwm = map(idiffpc, 0, 100,  iPWMmin, iPWMmax);

    fn_PWMSend(ipwm);


}

unsigned int iLastDispensingReachedTarget;
bool bDispensingOverOK;
void fn_checkdispensing() {
    if (!bAutoDispensing) {
        fn_dispensing_off();
      //  sDispensingStatus = "Do Empty T.";
        return;
    }

    if (fgrains < -9 and bDispensingOverOK) {// scale goes negative, means waiting for empty the receptacle must weigh more than 9 grains!!!
        fn_dispensing_off();
        sDispensingStatus = "Wait for empty..";
        return;
    }

    //receptacle in place 
    if (fgrains < eepromStore.iTargetWeight and fgrains >= -0.5) {
        int iSettletime = 5000;
        if (millis() - iLastDispensingReachedTarget < iSettletime) {
            //dont restart too early
            float itime = (iSettletime - iLastDispensingReachedTarget)/1000;
            sDispensingStatus = "Settle." + String(iSettletime,1);
        }
        else {
       //can dispense, but see if we must pause
               //check if we are currently paused if we can un-pause            
            int idiff = millis() - iLastDispensingPauseStopped;
            if (bDispensingIsPaused) { //check if we can un-pause     
                if (idiff >= eepromStore.DispensingPauseDuration) { 
                    Serial.println("Unpause");
                    bDispensingIsPaused = false;
                    iLastDispensingPauseStopped = millis();//reset the timer      
                } 
                return; //dont send any pwm if paused
            }  else {     
        //check how long ago we paused. if more than 4 seconds the give a 2 second pause
        if (idiff >= eepromStore.DispensingPauseInterval) { //enough time has passed so pause
            iLastDispensingPauseStopped = millis();
            Serial.println("PAUSE");
            fn_PWMSend(0);
            sDispensingStatus = "Pause";
            bDispensingIsPaused = true;
            return;
        }       
    }       
            fn_dispensing_on(false);
            bDispensingOverOK = false;
        }
    }

    if (fgrains < eepromStore.iTargetWeight and fgrains >= eepromStore.iTargetWeight - eepromStore.DispensingStopNear ) {
        fn_dispensing_off();
        sDispensingStatus = "Done.";
        bDispensingOverOK = true;
        iLastDispensingReachedTarget = millis();
    }

    if (fgrains > eepromStore.iTargetWeight) {
        fn_dispensing_off();
        sDispensingStatus = "OVER!!";
        bDispensingOverOK = true;
        iLastDispensingReachedTarget = millis();
    }
}

void   fn_setTarget() {
    if (bdebugSerial) Serial.println("fn_setTarget");
#ifdef USE_EEPROM
    eepromStore.iTargetWeight = fgrains;
    fn_EEPROMSave();
#endif      
    bAutoDispensing = true;
}
