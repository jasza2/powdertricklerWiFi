#pragma once


#define  iPWM_OUT_PIN1 13

const int freq = 2000;
const int ledChannel1 = 0;
const int resolution = 300;


void fn_PWMSetup() {
	ledcSetup(ledChannel1, freq, resolution);
	ledcAttachPin(iPWM_OUT_PIN1, ledChannel1);
 }

unsigned long iLastPWM1millis;
bool bStatus;
void fn_PWMSend(int ival) {

	if (millis() - iLastPWM1millis <= 100) return; //dont send too often
	ledcWrite(ledChannel1, ival);
	//Serial.println("PWM:" + String(ival));
	iLastPWM1millis = millis();
}

