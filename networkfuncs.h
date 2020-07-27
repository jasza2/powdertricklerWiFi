#pragma once 
#ifdef USE_NETWORK 
#include <WiFi.h>

//these are needed for the ESP32 acting as an access point
	bool bAPHasBeenSetup = false;
	// Put IP Address details 
	IPAddress local_ip(10, 0, 0, 1);
	IPAddress gateway(10, 0, 0, 1);
	IPAddress subnet(255, 255, 255, 0);

	int iWifiConnectAttempts ;

String ipToString(IPAddress ip) {
	String s = "";
	for (int i = 0; i < 4; i++)
		s += i ? "." + String(ip[i]) : String(ip[i]);
	return s;
}


void fn_WiFiConnect() {

	if (bdebugSerial) Serial.println("Setting up access point: " + DEFAULT_SSID);
	if (bdebugSerial) Serial.println("pwd: " + DEFAULT_PWD);

		//int iresult = WiFi.softAP(s_ssid.c_str(), s_password.c_str(), 1, 0, 4);//only one connection allowed at a time
		int iresult = WiFi.softAP(DEFAULT_SSID.c_str(), DEFAULT_PWD.c_str(), 4, 0, 4);//4 connection allowed at a time
		if (bdebugSerial) Serial.println("done: " + String(iresult));
		if (bdebugSerial) Serial.println("setting up gateway: " );
		
		WiFi.softAPConfig(local_ip, gateway, subnet);
		//IPOfAccessPoint = WiFi.softAPIP();
		Serial.print("IP Of Access Point: ");
		Serial.println(local_ip);
		sIP = local_ip.toString();
		bAPHasBeenSetup = true;
	//}	
}


bool WifiIsConnected() {
	return bAPHasBeenSetup;   //TODO: need to check if ap has been setup  (WiFi.status() == WL_CONNECTED);

		}

unsigned long iLastCheckWifi;
void setupWifi() {
	fn_WiFiConnect();	
	WiFi.setSleep (false); 
}

#endif


		



