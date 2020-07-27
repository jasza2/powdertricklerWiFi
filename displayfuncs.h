
#ifdef USE_OLED

#include <Wire.h>  // Only needed for Arduino 1.6.5 and earlier
 #include "SSD1306Wire.h" // 

// for external display wired in to sda a scl  
// ----------tput back in for oled ----------
//SSD1306Wire  oled(0x3c, 21, 22); 
//21 SDA (5 down from top right), 22 SCL(2 down from top right) - usb at bottom

SSD1306Wire  oled(0x3c, SDA_PIN, SCL_PIN);
unsigned long iLastoledStatus;
bool bon;
bool bOledIsConnected = true; 

void fn_oledStatus(bool bShowNow) {
	if (!bOledIsConnected) return;
	
	if (!bShowNow) if (millis() - iLastoledStatus < 50) return;
	iLastoledStatus = millis(); 
	///unsigned int istartmillis = millis(); 
	oled.clear();
	 
	oled.setFont(ArialMT_Plain_24);
	if (eepromStore.bShowGrams) {
		oled.drawString(0, 0, String(fgrams, 3) + " Gram ");
	}
	else {
		oled.drawString(0, 0, String(fgrains, 1) + " grains");
	}
	
	oled.setFont(ArialMT_Lato_Regular_13);
	oled.drawString(0, 25, sDispensingStatus);

	oled.setFont(ArialMT_Plain_10);
	if (!eepromStore.bShowGrams) {
	oled.drawString(0, 50, String(fgrams, 3) + " Gram");
	}
	else {
		oled.drawString(0, 50, String(fgrains, 1) + " grains");
	}

	oled.setFont(ArialMT_Plain_10); 

#ifdef USE_EEPROM
	oled.drawString(0, 40, "Target: " + String(eepromStore.iTargetWeight, 2));
	if (bTareHapened) {
		oled.drawString(100, 40, "T");
	} 

#endif	 
	 

#ifdef USE_NETWORK
	oled.setFont(ArialMT_Plain_10);	
	#ifdef USE_ACCESS_POINT
		oled.drawString(0, 30, "svr:" + sServerIP());
		oled.drawString(0, 40, sWifi );
		oled.drawString(0, 49, "MyIp:" + sIP + " Prt:" + String(eepromStore.iUDPSendPort));
	#else
		//oled.drawString(0, 40, sWifi + " Port:" + String(eepromStore.iUDPSendPort));
		#ifdef UDP_SERVER_SEND
			oled.drawString(0, 49, "svr:" + sServerIP());
		#else
		//	oled.drawString(0, 49,"my iP:" +  sIP);
		#endif
	#endif
#endif

	int ix = 100;
	if (bon) {
		oled.drawString(ix, 49, "/");
		bon = false;
	}	else {
		oled.drawString(ix, 49, "-");
		bon = true;
	}
oled.display();
}

void fn_SetupOLED() {
	unsigned int istartmillis = millis();	
	oled.init();

	//oled.flipScreenVertically();
	oled.setFont(ArialMT_Plain_10);
	oled.clear(); 
	unsigned int itimeneeded = millis() - istartmillis;

	// by experimentastion : if the oled is connected, it needs 25 seconds to start, less than 10 seconds if not connected
	if (itimeneeded < 10) bOledIsConnected = false;

//	if (bdebugSerial) 	Serial.println("OLED time needed to start:" + String(itimeneeded));
	//if (bdebugSerial and bOledIsConnected == false)if (bdebugSerial) Serial.println("OLED  NOT Connected!!!!!:" );
	
}

void fn_oledText(String sTooled, int ifont =0 ) {
	oled.clear(); 
	oled.setFont(ArialMT_Plain_16);
	if(sTooled.length()>=20) oled.setFont(ArialMT_Plain_10);
	oled.setFont(ArialMT_Plain_16);

	if (ifont == 1) {
		oled.setFont(ArialMT_Plain_10);
	}
	if (ifont == 2) {
		oled.setFont(ArialMT_Lato_Regular_13);
	}
	if (ifont == 3) {
		oled.setFont(ArialMT_Plain_16);
	}
	if (ifont == 4) {
		oled.setFont(ArialMT_Plain_24);
	}
	oled.drawString(0, 0, sTooled);
	oled.display();
}

void fn_oledot(bool bshow) {
	int x = oled.getWidth() - 6;
	int y = 6;
	if (bshow)	oled.fillCircle(x, y, 3); else	oled.drawCircle(x,y, 3);


	oled.display();
}


#endif