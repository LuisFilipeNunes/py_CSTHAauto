// Projeto 21 – Configurando o shield ethernet com o Arduino

// Protocolo utilizado para fazer a comunicação com o Arduino e...
//Shield Ethernet
#include <SPI.h>
#include <Arduino.h>
#include <Ethernet.h> // Biblioteca utilizada para funções internas do shield
#include <ArduinoJson.h> 
// A linha abaixo permite definir o endereço físico (MAC ADDRESS) da placa...
//de rede.

byte mac[] = { 0xAB, 0xCD, 0x12, 0x34, 0xFF, 0xCA };

//Os valores abaixo definem o endereço IP, gateway e máscara.

IPAddress ip(10,0,11,104); // Define o endereço IP.
IPAddress gateway(10,0,11,167); // Define o gateway.
IPAddress subnet(255, 255, 255, 0); // Define a máscara de rede.
IPAddress server(10, 0, 11, 167);  // Replace with your server's IP address
EthernetClient client;

String receivedData;
boolean startFlag;


void setup() {
  Serial.begin(9600);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  digitalWrite(8, HIGH);
  digitalWrite(9, HIGH);
Ethernet.begin(mac, ip); // Inicializa a placa com os dados fornecidos
 // Make sure to include the appropriate Ethernet library for your Arduino

}
void loop() {
  if (client.connect(server, 5000)) {
    Serial.println("Connected to server");
    client.println("GET /get-coffee-data HTTP/1.1");
    client.println("Host: 32.0.1.20");
    client.println("Connection: close");
    client.println();

    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        // Accumulate the received data
        if (c == '{') {
          startFlag = true;
        }
        if (startFlag == true){
          receivedData += c;
        }
        if (c == '}') {
          processJson_go(receivedData);
          // Reset the received data for the next iteration
          receivedData = "";
          startFlag = false;
        }
      }
    }
    client.stop();
    Serial.println("Connection closed");
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

  boolean shutdownFlag = doc["shutdownFlag"];
  boolean manualControlFlag = doc["manualControl"];
  boolean timeCheck;

  int CTHour, CTMin, STHour, STMin, RTHour, RTMin;

  parseTimeString(Realtime, RTHour, RTMin);
  parseTimeString(CoffeeTime, CTHour, CTMin);
  parseTimeString(ShutdownTime, STHour, STMin);

  if (manualControlFlag){
    Serial.println("Manual control activated");
    digitalWrite(8, LOW);
    digitalWrite(9, LOW);
    return;
  }

  boolean checkShutdownTime;
  boolean checkCoffeTime = checkTime(RTHour, RTMin, CTHour, CTMin);
  
  if(shutdownFlag){
    checkShutdownTime = checkTime(RTHour, RTMin, STHour, STMin);
    if (checkShutdownTime){
     Serial.println("Shutdown Time!");
      digitalWrite(8, HIGH);
      digitalWrite(9, HIGH);
      return;
    }
    else if(checkCoffeTime){
      Serial.println("Coffe time boy, but has a shutdown time!");
      digitalWrite(8, LOW);
      digitalWrite(9, LOW);
      return;
    }
    else{
      Serial.println("Waiting for coffe time");
      digitalWrite(8, HIGH);
      digitalWrite(9, HIGH);
      return;
    }
    
  }
  if(checkCoffeTime){
    Serial.println("Coffe time boy!");
    digitalWrite(8, LOW);
    digitalWrite(9, LOW);
  }
  else {
    Serial.println("Waiting for coffe time");
    digitalWrite(8, HIGH);
    digitalWrite(9, HIGH); 
  }

}
void parseTimeString(String timeString, int &hour, int &minute){
  int colonPos = timeString.indexOf(':');

  hour = timeString.substring(0, colonPos).toInt();
  minute = timeString.substring(colonPos + 1).toInt(); 
}

bool checkTime(int RTHour, int RTMin, int THour, int TMin) {
    if (RTHour > THour || (RTHour == THour && RTMin >= TMin)) {
        return true; 
    } else {
        return false;
    }
}


