
#include <SPI.h>
#include <Arduino.h>
#include <ArduinoJson.h> 

byte mac[] = { 0xAB, 0xCD, 0x12, 0x34, 0xFF, 0xCA };

//Defines the shield configuration, addresses, mac...

IPAddress ip(10,0,11,104); 
IPAddress gateway(10,0,11,167); 
IPAddress subnet(255, 255, 255, 0);
IPAddress server(10, 0, 11, 167); 
EthernetClient client;

String receivedData;  // String to store received data from server
boolean startFlag = false;  // Flag to indicate start of JSON data


void setup() {
  //Sets pins 8 and 9 as outputs, they are connected to the relay.
  Serial.begin(9600);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  digitalWrite(8, HIGH);
  digitalWrite(9, HIGH);
Ethernet.begin(mac, ip); 
}

void loop() {

  //Attempt to connect to the server. Then, makes a GET request for the JSON file. If it is complete, it will send it to the processJson_go() 
  if (client.connect(server, 5000)) {
    client.println("=======================================");
    Serial.println("Connected to server");
    client.println("GET /get-coffee-data HTTP/1.1");;
    client.println("=======================================");

    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        if (c == '{') {
          startFlag = true;
          receivedData = "";
        }
        else if (startFlag == true){
          receivedData += c;
        }
        if (c == '}') {
          startFlag = false;
          processJson_go(receivedData);        
        }
      }
    }
    client.println("=======================================");
    client.stop();
    Serial.println("Connection closed");
    Serial.println("=======================================");
    
  } else {
    Serial.println("Unable to connect to server");
  }

  delay(15000);  // Adjust the delay as needed
}

void processJson_go(String jsonStr) {
  // Parse the JSON string
  StaticJsonDocument<1024> doc;
  DeserializationError error = deserializeJson(doc, jsonStr);

  if (error) {
    Serial.print("JSON parsing failed: ");
    Serial.println(error.c_str());
    return;
  }

  String CoffeeTime = doc["coffeeTime"];
  String ShutdownTime = doc["shutdownTime"];
  String Realtime = doc["realTime"];
  boolean manualControlFlag = doc["manualControl"];

  int CT[2], RT[2], ST[2];

  RT = parseTimeString(Realtime);
  CT = parseTimeString(CoffeeTime);
  ST = parseTimeString(ShutdownTime);

  if (manualControlFlag){
    Serial.println("Manual control activated");
    digitalWrite(8, LOW);
    digitalWrite(9, LOW);
    return;
  }

  boolean checkShutdownTime = checkTime(RT , ST);
  boolean checkCoffeTime = checkTime(RT, CT);

  if (checkShutdownTime){
     Serial.println("Shutdown Time!");
      digitalWrite(8, HIGH);
      digitalWrite(9, HIGH);
      return;
    }
  else if(checkCoffeTime){
      Serial.println("Coffeetime!");
      digitalWrite(8, LOW);
      digitalWrite(9, LOW);
      return;
    }
  else{
      Serial.println("Waiting for coffee time");
      digitalWrite(8, HIGH);
      digitalWrite(9, HIGH);
      return;
    }
    
  }

int* parseTimeString(String timeString) {
//Parse the string formated time info to a array with int values. 

  int colonPos = timeString.indexOf(':');
  int* timeArray = new int[2]; 

  timeArray[0] = timeString.substring(0, colonPos).toInt();
  timeArray[1] = timeString.substring(colonPos + 1).toInt();

  return timeArray;  
}

bool checkTime(const int* RT, const int* TC) {
    // Validate input pointers to ensure they are not null pointers
    if (!RT || !TC ) {
        Serial.println("Error: Invalid input pointers. Cannot check time.");
        return false; // Indicate error
    }

    // Check if current time (RT) is after or equal to target time (T)
    if (RT[0] > TH[0] || (RT[0] == TH[0] && RT[1] >= TH[1])) {
        return true;
    } else {
        return false;
    }
}



