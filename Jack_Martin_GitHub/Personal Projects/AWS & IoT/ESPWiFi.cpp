// Function to Setup and Connect to Wifi Netowork defined by homeSSID

#include "ESPWiFi.h"
#include "secret.h"


char homeSSID[] = myssid;        // your network SSID (name)
char homePASS[] = mypass;
int wifiStatus = WL_IDLE_STATUS;

void printWifiStatus() {

  // print the SSID of the network you're attached to:
  Serial.println("");
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your board's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");

  digitalWrite(LED_BUILTIN, HIGH);

}

void connectToWiFi() {
  
  WiFi.mode(WIFI_STA);
  WiFi.disconnect(true);

  Serial.print("Attempting to connect to SSID: ");
  Serial.print(homeSSID);

  // attempt to connect to Wifi network:
   while(wifiStatus != WL_CONNECTED){
    wifiStatus = WiFi.begin(homeSSID, homePASS);
    Serial.print("...");
    delay(1000);
   }
  // you're connected now, so print out the status:
  printWifiStatus();
}

void setupWiFi(){
  connectToWiFi();
}
