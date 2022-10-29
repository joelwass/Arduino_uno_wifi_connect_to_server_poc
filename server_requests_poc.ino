
/*
  Example of reading values from a Bolsen Hall Effect water flow sensor
  and printing the values to a Grove-LCD RGB Backlight V4.0
  using an Arduino Uno Wifi.
  
  This example code is in the public domain.
*/

#include <Wire.h>
#include <WiFiNINA.h>
#include "rgb_lcd.h"

rgb_lcd lcd;

const int colorR = 255;
const int colorG = 0;
const int colorB = 0;

char ssid[] = "network_id";    //  your network SSID (name)
char pass[] = "network_password";   // your network password

char SERVER_ADDRESS[] = "google.com"; // hostname of web server
const int PORT = 80; // port of the server

int status = WL_IDLE_STATUS;

// Initialize the client library
WiFiClient client;

// the setup function runs once when you press reset or power the board
void setup() {
  Serial.begin(9600);
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);

  lcd.setRGB(colorR, colorG, colorB);
  
  lcd.clear();
  lcd.setCursor(0,0);

  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true);
  }

  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
    Serial.println("Please upgrade the firmware");
  }

  // print your MAC address:
  byte mac[6];
  WiFi.macAddress(mac);
  Serial.print("MAC: ");
  printMacAddress(mac);

  lcd.print("SSID: " + String(ssid));

  status = WiFi.begin(ssid, pass);
  if ( status != WL_CONNECTED) {
    lcd.clear();
    lcd.print("Couldn't get a wifi connection");
    // don't do anything else:
    while(true);
  }
  lcd.clear();
  lcd.print("Connected!");

  Serial.println("Connected to wifi");
  Serial.println("\nStarting server request...");
  // if you get a connection, report back via serial:
  if (client.connect(SERVER_ADDRESS, PORT)) {
    Serial.println("connected to server");
    // Make a HTTP request:
    makeGetRequest("/");
  }
}

// the loop function runs over and over again forever
void loop() {
  while (client.available()) {
    // read an incoming byte from the server and print them to serial monitor:
    char c = client.read();
    Serial.print(c);
    lcd.clear();
    lcd.print(c);
  }
  
  if (!client.connected()) {
    // if the server's disconnected, stop the client:
    client.stop();
  }
}

void printMacAddress(byte mac[]) {
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
}

// Path should start with a slash, i.e. /hello/world
void makeGetRequest(String path) {
  client.println("GET " + path + " HTTP/1.1");
  client.println("Host: " + String(SERVER_ADDRESS));
  client.println("Connection: close");
  client.println(); // end HTTP request header
}

// Path should start with a slash, i.e. /hello/world
// Body should be stringified json.
void makePostRequest(String body, String path) {
  // send HTTP request header
  client.println("POST " + path + " HTTP/1.1");
  client.println("Host: " + String(SERVER_ADDRESS));
  client.println("Content-Type: application/json");
  client.println("Accept: */*");
  client.println("Cache-Control: no-cache");
  client.println("Accept-Encoding: gzip, deflate");
  client.println("Accept-Language: en-us");
  client.println("Content-Length: " + String(body.length()));
  client.println("Connection: close");
  client.println(); // end HTTP request header
  client.println(body);
}
