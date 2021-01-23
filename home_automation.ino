
#include <Arduino.h>
#include <WiFi.h>
#include <WiFiMulti.h>
#include <WebSocketsClient.h>
#include <ArduinoJson.h>
#include <StreamString.h>

#include <AceButton.h>
using namespace ace_button;
WiFiMulti WiFiMulti;
WebSocketsClient webSocket;
WiFiClient client;

#define MyApiKey "----------------" // TODO: Change to your sinric API Key. Your API Key is displayed on sinric.com dashboard
#define MySSID "--------" // TODO: Change to your Wifi network SSID
#define MyWifiPassword "------" // TODO: Change to your Wifi network password

#define HEARTBEAT_INTERVAL 300000 // 5 Minutes 

uint64_t heartbeatTimestamp = 0;
bool isConnected = false;

String device_ID_1 = "------------------------";
String device_ID_2 = "------------------------";
String device_ID_3 = "------------------------";
String device_ID_4 = "------------------------";
String device_ID_5 = "------------------------";
String device_ID_6 = "------------------------";
String device_ID_7 = "------------------------";
String device_ID_8 = "------------------------";

// define the GPIO connected with Relays and switches

//Relays
#define RelayPin1 23  //D23
#define RelayPin2 22  //D22
#define RelayPin3 21  //D21
#define RelayPin4 19  //D19
#define RelayPin5 18  //D18
#define RelayPin6 5   //D5
#define RelayPin7 25  //D25
#define RelayPin8 26  //D26

// Switches
#define SwitchPin1 13  //D13
#define SwitchPin2 12  //D12
#define SwitchPin3 14  //D14
#define SwitchPin4 27  //D27
#define SwitchPin5 33  //D33
#define SwitchPin6 32  //D32
#define SwitchPin7 15  //D15
#define SwitchPin8 4   //D4

//WiFi Status LED
#define wifiLed    2   //D2


ButtonConfig config1;
AceButton button1(&config1);
ButtonConfig config2;
AceButton button2(&config2);
ButtonConfig config3;
AceButton button3(&config3);
ButtonConfig config4;
AceButton button4(&config4);
ButtonConfig config5;
AceButton button5(&config5);
ButtonConfig config6;
AceButton button6(&config6);
ButtonConfig config7;
AceButton button7(&config7);
ButtonConfig config8;
AceButton button8(&config8);


void handleEvent1(AceButton*, uint8_t, uint8_t);
void handleEvent2(AceButton*, uint8_t, uint8_t);
void handleEvent3(AceButton*, uint8_t, uint8_t);
void handleEvent4(AceButton*, uint8_t, uint8_t);
void handleEvent5(AceButton*, uint8_t, uint8_t);
void handleEvent6(AceButton*, uint8_t, uint8_t);
void handleEvent7(AceButton*, uint8_t, uint8_t);
void handleEvent8(AceButton*, uint8_t, uint8_t);

void setPowerStateOnServer(String deviceId, String value);

// deviceId is the ID assgined to your smart-home-device in sinric.com dashboard. Copy it from dashboard and paste it here

void turnOn(String deviceId) {
  if (deviceId == device_ID_1) // Device ID of 1st device
  {
    Serial.print("Turn on device id: ");
    Serial.println(deviceId);
    digitalWrite(RelayPin1, LOW);
  }
  if (deviceId == device_ID_2) // Device ID of 2nd device
  {
    Serial.print("Turn on device id: ");
    Serial.println(deviceId);
    digitalWrite(RelayPin2, LOW);
  }
  if (deviceId == device_ID_3) // Device ID of 3rd device
  {
    Serial.print("Turn on device id: ");
    Serial.println(deviceId);
    digitalWrite(RelayPin3, LOW);
  }
  if (deviceId == device_ID_4) // Device ID of 4th device
  {
    Serial.print("Turn on device id: ");
    Serial.println(deviceId);
    digitalWrite(RelayPin4, LOW);
  }
  if (deviceId == device_ID_5) // Device ID of 5th device
  {
    Serial.print("Turn on device id: ");
    Serial.println(deviceId);
    digitalWrite(RelayPin5, LOW);
  }
  if (deviceId == device_ID_6) // Device ID of 6th device
  {
    Serial.print("Turn on device id: ");
    Serial.println(deviceId);
    digitalWrite(RelayPin6, LOW);
  }
  if (deviceId == device_ID_7) // Device ID of 7th device
  {
    Serial.print("Turn on device id: ");
    Serial.println(deviceId);
    digitalWrite(RelayPin7, LOW);
  }
  if (deviceId == device_ID_8) // Device ID of 8th device
  {
    Serial.print("Turn on device id: ");
    Serial.println(deviceId);
    digitalWrite(RelayPin8, LOW);
  }
}

void turnOff(String deviceId) {
  if (deviceId == device_ID_1) // Device ID of 1st device
  {
    Serial.print("Turn off Device ID: ");
    Serial.println(deviceId);
    digitalWrite(RelayPin1, HIGH);
  }
  if (deviceId == device_ID_2) // Device ID of 2nd device
  {
    Serial.print("Turn off Device ID: ");
    Serial.println(deviceId);
    digitalWrite(RelayPin2, HIGH);
  }
  if (deviceId == device_ID_3) // Device ID of 3rd device
  {
    Serial.print("Turn off Device ID: ");
    Serial.println(deviceId);
    digitalWrite(RelayPin3, HIGH);
  }
  if (deviceId == device_ID_4) // Device ID of 4th device
  {
    Serial.print("Turn off Device ID: ");
    Serial.println(deviceId);
    digitalWrite(RelayPin4, HIGH);
  }
  if (deviceId == device_ID_5) // Device ID of 5th device
  {
    Serial.print("Turn off Device ID: ");
    Serial.println(deviceId);
    digitalWrite(RelayPin5, HIGH);
  }
  if (deviceId == device_ID_6) // Device ID of 6th device
  {
    Serial.print("Turn off Device ID: ");
    Serial.println(deviceId);
    digitalWrite(RelayPin6, HIGH);
  }
  if (deviceId == device_ID_7) // Device ID of 7th device
  {
    Serial.print("Turn off Device ID: ");
    Serial.println(deviceId);
    digitalWrite(RelayPin7, HIGH);
  }
  if (deviceId == device_ID_8) // Device ID of 8th device
  {
    Serial.print("Turn off Device ID: ");
    Serial.println(deviceId);
    digitalWrite(RelayPin8, HIGH);
  }
}

void webSocketEvent(WStype_t type, uint8_t * payload, size_t length) {
  switch (type) {
    case WStype_DISCONNECTED:
      isConnected = false;
      WiFiMulti.addAP(MySSID, MyWifiPassword);
      Serial.printf("[WSc] Webservice disconnected from sinric.com!\n");
      break;
    case WStype_CONNECTED: {
        isConnected = true;
        Serial.printf("[WSc] Service connected to sinric.com at url: %s\n", payload);
        Serial.printf("Waiting for commands from sinric.com ...\n");
      }
      break;
      case WStype_TEXT: {
        Serial.printf("[WSc] get text: %s\n", payload);

        #if ARDUINOJSON_VERSION_MAJOR == 5
                DynamicJsonBuffer jsonBuffer;
                JsonObject& json = jsonBuffer.parseObject((char*)payload);
        #endif
        #if ARDUINOJSON_VERSION_MAJOR == 6
                DynamicJsonDocument json(1024);
                deserializeJson(json, (char*) payload);
        #endif
        String deviceId = json ["deviceId"];
        String action = json ["action"];

        if (action == "setPowerState") { // Switch or Light
          String value = json ["value"];
          if (value == "ON") {
            turnOn(deviceId);
          } else {
            turnOff(deviceId);
          }
        }
        else if (action == "test") {
          Serial.println("[WSc] received test command from sinric.com");
        }
      }
      break;
    case WStype_BIN:
      Serial.printf("[WSc] get binary length: %u\n", length);
      break;
  }
}

void setup() {
  Serial.begin(9600);

  WiFiMulti.addAP(MySSID, MyWifiPassword);
  Serial.println();
  Serial.print("Connecting to Wifi: ");
  Serial.println(MySSID);

  // Waiting for Wifi connect
  if (WiFiMulti.run() != WL_CONNECTED) {
    delay(500);
    Serial.print("Connecting...");
  }
  if (WiFiMulti.run() == WL_CONNECTED) {
    Serial.println("");
    Serial.print("WiFi connected. ");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  }

  pinMode(SwitchPin1, INPUT_PULLUP);
  pinMode(SwitchPin2, INPUT_PULLUP);
  pinMode(SwitchPin3, INPUT_PULLUP);
  pinMode(SwitchPin4, INPUT_PULLUP);
  pinMode(SwitchPin5, INPUT_PULLUP);
  pinMode(SwitchPin6, INPUT_PULLUP);
  pinMode(SwitchPin7, INPUT_PULLUP);
  pinMode(SwitchPin8, INPUT_PULLUP);

  pinMode(RelayPin1, OUTPUT);
  pinMode(RelayPin2, OUTPUT);
  pinMode(RelayPin3, OUTPUT);
  pinMode(RelayPin4, OUTPUT);
  pinMode(RelayPin5, OUTPUT);
  pinMode(RelayPin6, OUTPUT);
  pinMode(RelayPin7, OUTPUT);
  pinMode(RelayPin8, OUTPUT);

  pinMode(wifiLed, OUTPUT);

  //During Starting all Relays should TURN OFF
  digitalWrite(RelayPin1, HIGH);
  digitalWrite(RelayPin2, HIGH);
  digitalWrite(RelayPin3, HIGH);
  digitalWrite(RelayPin4, HIGH);
  digitalWrite(RelayPin5, HIGH);
  digitalWrite(RelayPin6, HIGH);
  digitalWrite(RelayPin7, HIGH);
  digitalWrite(RelayPin8, HIGH);
  
  //During Starting WiFi LED should TURN OFF
  digitalWrite(wifiLed, LOW);

  config1.setEventHandler(button1Handler);
  config2.setEventHandler(button2Handler);
  config3.setEventHandler(button3Handler);
  config4.setEventHandler(button4Handler);
  config5.setEventHandler(button5Handler);
  config6.setEventHandler(button6Handler);
  config7.setEventHandler(button7Handler);
  config8.setEventHandler(button8Handler);

  button1.init(SwitchPin1);
  button2.init(SwitchPin2);
  button3.init(SwitchPin3);
  button4.init(SwitchPin4);
  button5.init(SwitchPin5);
  button6.init(SwitchPin6);
  button7.init(SwitchPin7);
  button8.init(SwitchPin8);

  // server address, port and URL
  webSocket.begin("iot.sinric.com", 80, "/");

  // event handler
  webSocket.onEvent(webSocketEvent);
  webSocket.setAuthorization("apikey", MyApiKey);

  // try again every 5000ms if connection has failed
  webSocket.setReconnectInterval(5000);   // If you see 'class WebSocketsClient' has no member named 'setReconnectInterval' error update arduinoWebSockets
}

void loop() {

  if (WiFiMulti.run() != WL_CONNECTED)
  {
    Serial.println("Not Connected");
    digitalWrite(wifiLed, LOW);
  }
  else
  {
    Serial.println(" Connected");
    digitalWrite(wifiLed, HIGH);
    webSocket.loop();
  }

  button1.check();
  button2.check();
  button3.check();
  button4.check();
  button5.check();
  button6.check();
  button7.check();
  button8.check();

  if (isConnected) {
    uint64_t now = millis();

    // Send heartbeat in order to avoid disconnections during ISP resetting IPs over night. Thanks @MacSass
    if ((now - heartbeatTimestamp) > HEARTBEAT_INTERVAL) {
      heartbeatTimestamp = now;
      webSocket.sendTXT("H");
    }
  }
}

void setPowerStateOnServer(String deviceId, String value) {
  #if ARDUINOJSON_VERSION_MAJOR == 5
    DynamicJsonBuffer jsonBuffer;
    JsonObject& root = jsonBuffer.createObject();
  #endif
  #if ARDUINOJSON_VERSION_MAJOR == 6
    DynamicJsonDocument root(1024);
  #endif
  
    root["deviceId"] = deviceId;
    root["action"] = "setPowerState";
    root["value"] = value;
    StreamString databuf;
  #if ARDUINOJSON_VERSION_MAJOR == 5
    root.printTo(databuf);
  #endif
  #if ARDUINOJSON_VERSION_MAJOR == 6
    serializeJson(root, databuf);
  #endif
  webSocket.sendTXT(databuf);
}

void button1Handler(AceButton* button, uint8_t eventType, uint8_t buttonState) {
  Serial.println("EVENT1");
  switch (eventType) {
    case AceButton::kEventPressed:
      Serial.println("kEventPressed");
      setPowerStateOnServer(device_ID_1, "ON");
      digitalWrite(RelayPin1, LOW);
      break;
    case AceButton::kEventReleased:
      Serial.println("kEventReleased");
      setPowerStateOnServer(device_ID_1, "OFF");
      digitalWrite(RelayPin1, HIGH);
      break;
  }
}

void button2Handler(AceButton* button, uint8_t eventType, uint8_t buttonState) {
  Serial.println("EVENT2");
  switch (eventType) {
    case AceButton::kEventPressed:
      Serial.println("kEventPressed");
      setPowerStateOnServer(device_ID_2, "ON");
      digitalWrite(RelayPin2, LOW);
      break;
    case AceButton::kEventReleased:
      Serial.println("kEventReleased");
      setPowerStateOnServer(device_ID_2, "OFF");
      digitalWrite(RelayPin2, HIGH);
      break;
  }
}

void button3Handler(AceButton* button, uint8_t eventType, uint8_t buttonState) {
  Serial.println("EVENT3");
  switch (eventType) {
    case AceButton::kEventPressed:
      Serial.println("kEventPressed");
      setPowerStateOnServer(device_ID_3, "ON");
      digitalWrite(RelayPin3, LOW);
      break;
    case AceButton::kEventReleased:
      Serial.println("kEventReleased");
      setPowerStateOnServer(device_ID_3, "OFF");
      digitalWrite(RelayPin3, HIGH);
      break;
  }
}

void button4Handler(AceButton* button, uint8_t eventType, uint8_t buttonState) {
  Serial.println("EVENT4");
  switch (eventType) {
    case AceButton::kEventPressed:
      Serial.println("kEventPressed");
      setPowerStateOnServer(device_ID_4, "ON");
      digitalWrite(RelayPin4, LOW);
      break;
    case AceButton::kEventReleased:
      Serial.println("kEventReleased");
      setPowerStateOnServer(device_ID_4, "OFF");
      digitalWrite(RelayPin4, HIGH);
      break;
  }
}

void button5Handler(AceButton* button, uint8_t eventType, uint8_t buttonState) {
  Serial.println("EVENT5");
  switch (eventType) {
    case AceButton::kEventPressed:
      Serial.println("kEventPressed");
      setPowerStateOnServer(device_ID_5, "ON");
      digitalWrite(RelayPin5, LOW);
      break;
    case AceButton::kEventReleased:
      Serial.println("kEventReleased");
      setPowerStateOnServer(device_ID_5, "OFF");
      digitalWrite(RelayPin5, HIGH);
      break;
  }
}

void button6Handler(AceButton* button, uint8_t eventType, uint8_t buttonState) {
  Serial.println("EVENT6");
  switch (eventType) {
    case AceButton::kEventPressed:
      Serial.println("kEventPressed");
      setPowerStateOnServer(device_ID_6, "ON");
      digitalWrite(RelayPin6, LOW);
      break;
    case AceButton::kEventReleased:
      Serial.println("kEventReleased");
      setPowerStateOnServer(device_ID_6, "OFF");
      digitalWrite(RelayPin6, HIGH);
      break;
  }
}

void button7Handler(AceButton* button, uint8_t eventType, uint8_t buttonState) {
  Serial.println("EVENT7");
  switch (eventType) {
    case AceButton::kEventPressed:
      Serial.println("kEventPressed");
      setPowerStateOnServer(device_ID_7, "ON");
      digitalWrite(RelayPin7, LOW);
      break;
    case AceButton::kEventReleased:
      Serial.println("kEventReleased");
      setPowerStateOnServer(device_ID_7, "OFF");
      digitalWrite(RelayPin7, HIGH);
      break;
  }
}

void button8Handler(AceButton* button, uint8_t eventType, uint8_t buttonState) {
  Serial.println("EVENT8");
  switch (eventType) {
    case AceButton::kEventPressed:
      Serial.println("kEventPressed");
      setPowerStateOnServer(device_ID_8, "ON");
      digitalWrite(RelayPin8, LOW);
      break;
    case AceButton::kEventReleased:
      Serial.println("kEventReleased");
      setPowerStateOnServer(device_ID_8, "OFF");
      digitalWrite(RelayPin8, HIGH);
      break;
  }
}
