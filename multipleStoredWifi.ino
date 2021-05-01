#include <Arduino.h>
#include <SPI.h>
#include <WiFi101.h>
#include <Networks.h>         // list of stored wifi networks

int status = WL_IDLE_STATUS;  // wifi network status, true if connected
bool wMatch = false;          // wifi networks match flag for breaking comparison sequence
int nMatch = -1;              // if WiFi network match is found, store the index of stored networks array
IPAddress ip;
byte mac[6];

void setup(){

  // For starters initialize Serial for debugging purposes
  Serial.begin(115200);
  while(!Serial);

  // Start of WiFi intialization
  while(status != WL_CONNECTED){
    
    Serial.println("Checking for available WIFI networks ... ");
    int numSsid = WiFi.scanNetworks(); // check number for available networks
    
    if(numSsid == -1)
    {
      Serial.println("No WiFi networks detected, rebooting in 10 seconds");
      delay(10000);
      NVIC_SystemReset();
    }

    int numKnwn = (sizeof(networks)/sizeof(char*))/2; // size of networks/char* returns number of elements in networks array, but since eatch network consists of two elements ssid and password then number of networks is /2
    Serial.print("Number of stored WiFi networks: ");
    Serial.println(numKnwn);


    Serial.print("Number of available WiFi networks: ");
    Serial.println(numSsid);

    while(!wMatch)
    {
      for(int i = 0; i < numSsid; i++) //for every available wifi network SSID
      {
        for(int j = 0; j < numKnwn; j++) // compare a stored network SSID
        {
          const char *a = WiFi.SSID(i);
          const char *b = networks[j][0];
          if(strcmp(a, b) == 0)
          {
            Serial.print("Known WiFi network match found SSID: ");
            Serial.println(WiFi.SSID(i));
            wMatch = true;
            nMatch = j;
            break;  // break out of parent for loop
          }
        }
        if(wMatch)
        {
          break;    // break out of parent for loop if a match is found
        }
        else if(i == numSsid - 1 && !wMatch)
        {
          Serial.println("No known network visible, check available WiFi networks and stored networks (in Networks.h)");
        }
      }
    }
    Serial.print("Logging on to WiFi network SSID: ");
    Serial.println(networks[nMatch][0]);
    status = WiFi.begin(networks[nMatch][0], networks[nMatch][1]);
    delay(10000); //this delay is in every wifi101 library example ... so be it.
  }

  Serial.print("Connected to WiFi network SSID: ");
  Serial.println(WiFi.SSID());
  
  long rssi = WiFi.RSSI();
  Serial.print("Signal strength (RSSI):");
  Serial.println(rssi);

  byte encryption = WiFi.encryptionType();
  Serial.print("Encryption Type:");
  Serial.println(encryption, HEX);

  ip = WiFi.localIP();
  Serial.print("Local IP Address: ");
  Serial.println(ip);

  WiFi.macAddress(mac);
  Serial.print("Local MAC address: ");
  for (int i = 5; i >= 0; i--) {
    if (mac[i] < 16) {
      Serial.print("0");
    }
    Serial.print(mac[i], HEX);
    if (i > 0) {
      Serial.print(":");
    }
  }
  Serial.println();
  // end of wifi initialization
  
}

void loop(){

  while(true);

}