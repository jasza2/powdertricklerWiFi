#pragma once
#ifdef USE_WEBSERVER
#include <WebServer.h> 
WebServer server;

bool bDebugWeb = true;
//DNSServer         dnsServer;              // only needed for captive portal
//const byte        DNS_PORT = 53;          // Capture DNS requests on port 53
int iwebclientscount = 0;

String fn_buildHTML() {//String smsg

	//fn_WifiGetRSSI();
	if (bDebugWeb) Serial.println("Getting html..");
	String sHTML = "<!DOCTYPE html> <html>\n";
	sHTML += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=yes\">\n";
	
	String sType = " Scale 101 ";

	sHTML += "<title>" + sType + " </title>\n";
	sHTML += "<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
	sHTML += "body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}\n";
	sHTML += ".button {display: block;width: 80px;background-color: #3498db;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n";
	sHTML += ".button-on {background-color: #3498db;}\n";
	sHTML += ".button-on:active {background-color: #2980b9;}\n";
	sHTML += ".button-off {background-color: #34495e;}\n";
	sHTML += ".button-off:active {background-color: #2c3e50;}\n";
	sHTML += ".blank {background-color: white;}\n";
	sHTML += ".submitbut {background-color: gray;}\n";	
	sHTML += ".boxrounded {border-radius:25px; border:2px solid #73AD21; padding: 5px;}\n";
//width: 200px;  height: 150px;
	sHTML += "p {font-size: 14px;color: #888;margin-bottom: 10px;}\n";
	sHTML += "</style>\n";
	sHTML += "</head>\n";
	sHTML += "<body>\n";

	if (bDebugWeb) Serial.println("1 html..");
	sHTML += "<h1><a href=http://" + sIP + "> Home." + sType + "</a></h1>\n";
    sHTML += "<p>Connected clients: " + String(iwebclientscount) + "\n</p>";



	//---------------------------------------------------------- target wight and dispensing -------------------------
	sHTML += "<form action='/submit' method='POST'>";
	sHTML += "<div class='boxrounded'>";
	sHTML += "<h3>Dispensing:</h3>";

	//showslow mode button
	if (bAutoDispensing)
	{
		sHTML += "<p><H2>Currently Dispensing.</H2>";
		sHTML += "<a class=\"button button-on\" href=\"/submit?DISPENSING=0\">STOP</a>\n";
	}
	else {
		sHTML += "<p><H2>Currently NOT dispensing:</H2>";
		sHTML += "<a class=\"button button-off\" href=\"/submit?DISPENSING=1\">START</a>\n";
	}


	sHTML += "<p>Target:<INPUT  name='TARGETWEIGHT' id='TARGETWEIGHT' value='" + String(eepromStore.iTargetWeight,1) + "' ></p>";

	sHTML += "</br><INPUT type='submit' value='Set target..'>";
	sHTML += "</form>";
	sHTML += "</div>";

	//----------------------------------------end of dispensings-------------------------------------

	 


	//show a tare button
	sHTML += "<p><a class=\"button button-on\" href=\"/submit?TARE=1\">Tare</a>\n</p>"; 
		
	//----------------------------------------general settings-------------------------------------
	sHTML += "<form action='/submit' method='POST'>";
	sHTML += "<div class='boxrounded'>";
	sHTML += "<h3>General settings:</h3>";	
	
			sHTML += "<p>Scale Gain (128 or 64):<INPUT type='number' name='GAIN' id='GAIN' value='" + String(eepromStore.igain) + "' min='64' max='128'></p>";
				sHTML += "<p>Convert raw to grain. Divide by:<INPUT  name='CONVERTTOGRAIN' id='CONVERTTOGRAIN' value='" + String(eepromStore.iConvertToGrain) + "'> to get to grain)</p>";
			sHTML += "<p>Convert grains to Grams:(divide the grains by this amount, normaly 15.432)<INPUT   name='GRAINTOGRAM' id='GRAINTOGRAM' value='" + String(eepromStore.iConvertGrainToGrams,4) + "'  ></p>";
			sHTML += "<p>Tare: Number of Readings:<INPUT  name='TAREREADINGS' id='TAREREADINGS' value='" + String(eepromStore.iTareNumberOfReadings) + "' type='number'  min='1' max='20000' ></p>";
			sHTML += "<p>Tare: Number of loops:<INPUT  name='TARELOOPS' id='TARELOOPS' value='" + String(eepromStore.iTareNumberOfLoops) + "' type='number'  min='1' max='20000' ></p>";
			sHTML += "<p>Dispensing: Pause Duration<INPUT  name='DISPENSINGPAUSEDURATION' id='DISPENSINGPAUSEDURATION' value='" + String(eepromStore.DispensingPauseDuration) + "' type='number'  min='0' max='200000' ></p>";
			sHTML += "<p>Dispensing: Pause Interval<INPUT  name='DISPENSINGPAUSEINTERVAL' id='DISPENSINGPAUSEINTERVAL' value='" + String(eepromStore.DispensingPauseInterval) + "' type='number'  min='0' max='200000' ></p>";
			sHTML += "<p>Dispensing: Stop before target<INPUT  name='DISPENSINGSTOPNEAR' id='DISPENSINGSTOPNEAR' value='" + String(eepromStore.DispensingStopNear) + "'   ></p>";

			sHTML += "</br><INPUT type='submit' value='Submit'>";
			sHTML += "</form>";
			sHTML += "</div>";

	


//----------------------------------------------------------slow/fast mode -------------------------
		sHTML += "<form action='/submit' method='POST'>";
		sHTML += "<div class='boxrounded'>";	
		sHTML += "<h3>Slow/Fast mode:</h3>";

		sHTML += "<p>In Slow Mode: No of Readings to take:<INPUT type='number' name='NOREADINGSSLOW' id='NOREADINGSSLOW' value='" + String(eepromStore.iNumberofReadingsSlow) + "' min='1' max='10000'></p>";
		sHTML += "<p>In Fast Mode: No of Readings to take:<INPUT type='number' name='NOREADINGSFAST' id='NOREADINGSFAST' value='" + String(eepromStore.iNumberofReadingsQuick) + "' min='1' max='10000'></p>";
		//showslow mode button
		if (bSlowMode)
		{
			sHTML += "<p><H2>Currently Slow:</H2>";
			sHTML += "<a class=\"button button-on\" href=\"/submit?SLOWMODE=0\">Go Fast</a>\n";
		}
		else {
			sHTML += "<p><H2>Currently Fast:</H2>";
			sHTML += "<a class=\"button button-off\" href=\"/submit?SLOWMODE=1\">Go Slow</a>\n";
		}

		sHTML += "</br><INPUT type='submit' value='Submit'>";
		sHTML += "</form>";
		sHTML += "</div>";

//----------------------------------------end of generasl settings-------------------------------------

//----------------------------------------------------------calibrate now -------------------------
		sHTML += "<form action='/submit' method='POST'>";
		sHTML += "<div class='boxrounded'>";
		sHTML += "<h3>Calibrate:</h3>";

		String sUOM = "grain";
		if (eepromStore.bShowGrams) sUOM = "Grams";
		
		sHTML += "</br>(place a known " + sUOM + " weight on the scale)";

		sHTML += "<p>Known Weight (" + sUOM + "):<INPUT  name='CALIBWEIGHT' id='CALIBWEIGHT' value='" + String(eepromStore.iCalibweight) + "' ></p>";
	
		sHTML += "</br><INPUT type='submit' value='Calibrate now..'>";
		sHTML += "</form>";
		sHTML += "</div>";

		//----------------------------------------end of  calib now-------------------------------------

	//----------------------------------------other settings-------------------------------------
	
		sHTML += "<div class='boxrounded'>";
		sHTML += "<h3>UOM:</h3>";
		if (eepromStore.bShowGrams)
		{
			sHTML += "<p><H2>Currently showing Grams</H2>";
			sHTML += "<a class=\"button button-on\" href=\"/submit?SHOWGRAMS=0\">Show grains</a>\n";
		}
		else {
			sHTML += "<p>Currently showing grains:";
			sHTML += "<a class=\"button button-off\" href=\"/submit?SHOWGRAMS=1\">Show Grams</a>\n";
		} 
	sHTML += "</div>";
//---------------------------------------------- end if settings ----------------------

			//----------------------------------------show all settings-------------------------------------
		sHTML += "<div class='boxrounded'>";
		sHTML += "<h3>Current settings:</h3>";

		sHTML += "<p>fgrains: " + String(fgrains, 3) + "</p>";
		sHTML += "<p>fgrams: " + String(fgrams, 3) + "</p>";
	
		sHTML += "<p>sDispensingStatus: " + sDispensingStatus + "</p>";


		sHTML += "</div>";
		//---------------------------------------------- end of settings ----------------------



	sHTML += "</body>\n";
	sHTML += "</html>\n";

	if (bDebugWeb) Serial.println("end html..");
	return sHTML;
}

String sToSend = "";
void handle_OnConnect() {
	iwebclientscount += 1;
	if (bDebugWeb) Serial.println("Client " + String(iwebclientscount) + " Connected to server.");
	server.send(200, "text/html", fn_buildHTML());
}

void handle_download() {
	Serial.println("download.");
	String sTosend = "nothing to download"; // fn_TagList();
	server.sendHeader("Content-Length", String(sTosend.length()));
 	server.sendHeader("Content-Disposition", "attachment; filename=\"DroneScanned.csv\"");
 	server.send(200, "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet", sTosend);
}

void handle_NotFound() {
	server.send(404, "text/plain", "Web page Not found");
}
bool NZBool(String str) {
	str.trim();
	str.toLowerCase();

	if (str == "on") return true;
	if (str == "off") return false;
	if (str == "") return false;
	if (str == "true") return true;
	if (str == "false") return false;
	if (str == "0") return false;
	if (str == "flase") return false;
	if (str == "fals") return false;
	if (str == "f") return false;
	if (str == "t") return true;
	if (str == "1") return true;
	if (str == "-1") return true;
	return false;


}

bool isNumeric(String str) {
	for (byte i = 0; i < str.length(); i++)
	{
		if (str.charAt(i) == '-' or isdigit(str.charAt(i))) return true;
	}
	return false;
}

//pass it two parameters from the serial port and it will save them, returns true if ok
String fn_CheckMessageSave(String sVal0, String sVal1) {
	bool bNumeric = isNumeric(sVal1);
	bool bok = false;
	sVal0.trim();
	sVal1.trim();
	if (bDebugWeb) 	Serial.println("sVal0:" + sVal0);
	if (bDebugWeb) 	Serial.println("sVal1:" + sVal1);
	if (bDebugWeb) Serial.println("bNumeric:" + String(bNumeric));

	if (sVal0 == "GAIN") {
		if (bNumeric) eepromStore.igain = sVal1.toInt();// constrain(, 0, MAX_LEN);
		bok = true;
	}

	if (sVal0 == "TARE") {
		scale_tare();
	}

	if (sVal0 == "NOREADINGSFAST") {
		if (bNumeric) eepromStore.iNumberofReadingsQuick = sVal1.toInt();//  
		bok = true;
	}
	if (sVal0 == "NOREADINGSSLOW") {
		if (bNumeric) eepromStore.iNumberofReadingsSlow = sVal1.toInt();//  
		bok = true;
	}
	if (sVal0 == "CONVERTTOGRAIN") {
		if (bNumeric) {
			eepromStore.iConvertToGrain = sVal1.toFloat();// 
			scale_setGrainConv(eepromStore.iConvertToGrain);
		}
		bok = true;
	}
	if (sVal0 == "GRAINTOGRAM") {
		if (bNumeric) eepromStore.iConvertGrainToGrams = sVal1.toFloat();// 
		bok = true;
	}

	if (sVal0 == "SLOWMODE") {
	  eepromStore.bSlowMode = NZBool(sVal1);// 
	  bSlowMode = eepromStore.bSlowMode;
	  scale_slow(eepromStore.bSlowMode);
  	bok = true;
	}

	if (sVal0 == "DISPENSING") {
		bAutoDispensing= NZBool(sVal1);// 
		if (bAutoDispensing) {
			fn_dispensing_on(true);
		}else{
			fn_dispensing_off;
		}
	}
	if (sVal0 == "TARGETWEIGHT") {
		if (bNumeric) eepromStore.iTargetWeight = sVal1.toInt();// 
		bok = true;
	}
	

	if (sVal0 == "SLOWMODETEMP") {
		bSlowMode = NZBool (sVal1);// 
	}


	if (sVal0 == "TAREREADINGS") {
		if (bNumeric) eepromStore.iTareNumberOfReadings = sVal1.toInt();// 
		bok = true;
	}

	if (sVal0 == "TARELOOPS") {
		if (bNumeric) eepromStore.iTareNumberOfLoops = sVal1.toInt();// 
		bok = true;
	}

	if (sVal0 == "DISPENSINGPAUSEDURATION") {
		if (bNumeric) eepromStore.DispensingPauseDuration = sVal1.toInt();// 
		bok = true;
	}

	if (sVal0 == "DISPENSINGPAUSEINTERVAL") {
		if (bNumeric) eepromStore.DispensingPauseInterval = sVal1.toInt();// 
		bok = true;
	}

	if (sVal0 == "DISPENSINGSTOPNEAR") {
			if (bNumeric) eepromStore.DispensingStopNear = sVal1.toFloat();// 
			bok = true;		
	}


	if (sVal0 == "CALIBWEIGHT") {
		if (bNumeric) eepromStore.iCalibweight  = sVal1.toInt();// 
		//perform the calibration
		if (eepromStore.iCalibweight > 0) {
		    
			scale_calib(eepromStore.iCalibweight);
			
		}
		bok = true;
	}

	if (sVal0 == "SHOWGRAMS") {
		eepromStore.bShowGrams = NZBool(sVal1);
		bok = true;
	}

	if (bok) {
		fn_EEPROMSave();
	//	fn_BuzzOnOff(true, 100);	
		if (bDebugWeb) 	Serial.println(sVal0 + " changed to: " + sVal1);	
		return sVal0 + " changed to: " + sVal1;
	}
	return "";
}



void handleSubmit() {

	Serial.println("handleSubmit");
	String s;
	for (uint8_t i = 0; i < server.args(); i++) {
		String sargname = server.argName(i);
		String sargval = server.arg(i);
		Serial.println("Web Recevied: " + sargname + ": " + sargval + "\n");
		fn_CheckMessageSave( sargname, sargval);
	}
	 
	server.send(200, "text/html", fn_buildHTML()); 
}



void fn_SetupWebsite() {
if (bDebugWeb) Serial.println("fn_SetupWebsite..");
     server.on("/", handle_OnConnect);
	server.on("/submit", handleSubmit);
	server.on("/post", handleSubmit);
 	server.onNotFound(handle_NotFound);
	if( bDebugWeb)	Serial.println("Starting web server....");
	server.begin();
	if (bDebugWeb)Serial.println("web server started");

}

#endif




