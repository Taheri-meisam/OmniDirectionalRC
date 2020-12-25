#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

const char* ssid = "VirusNetwork2.4";
const char* password = "4076174574";

ESP8266WebServer server(80);
int x;
const int LFF = 5; //D1
const int LFR = 4; //D2
const int RFF = 0; //D3
const int RFR = 2; //D4
const int BLF = 14; //D5
const int BLR = 12; //D6
const int BRF = 16; //D0
const int BRR = 13; //D7
const int SpeedPin = 10; //sD3
int motorSpeed = 100;
bool flag = true;
String webPage = "<html> <head> <style> button {   background-color: #0080FF;  padding: 12px 28px;   width: 100%;  height: 10%;  border-radius: 40px;   border: 0px;  font-size: 30px;  color: black;   } html {font-family: Arial; display: inline-block; text-align: center;}h2 {font-size: 2.3rem;}p {font-size: 1.9rem;}body {max-width: 400px; margin:0px auto; padding-bottom: 25px;}.slider { -webkit-appearance: none; margin: 14px; width: 360px; height: 25px; background: #FFD65C;outline: none; -webkit-transition: .2s; transition: opacity .2s;}.slider::-webkit-slider-thumb {-webkit-appearance: none; appearance: none; width: 35px; height: 35px; background: #003249; cursor: pointer;}.slider::-moz-range-thumb { width: 35px; height: 35px; background: #003249; cursor: pointer; }</style> </head> <body> <h1> Omnidirectional RC car  </h1> <p><button onmousedown=location.href=\"/Forward\"> Forward </button></p> <p><button onmousedown=location.href=\"/Backward\"> Backward </button></p> <p><button onmousedown=location.href=\"/Right\"> Right </button></p><p><button onmousedown=location.href=\"/Left\"> Left </button></p> <p><button onmousedown=location.href=\"/SpeedUp\"> SpeedUp </button></p><p><button onmousedown=location.href=\"/SpeedDown\"> SpeedDown </button></p><p><button onmousedown=location.href=\"/stop\"> STOP </button></p> </body> </html>";
String speedtext = "0";
void handleRoot() {
  server.send(200, "text/html", webPage + "<h3> Speed is : " + String(motorSpeed) + "</h3>");
}
void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

void setSpeed(int speed) {
  motorSpeed = speed;
  server.send(200, "text/html", webPage + "<h3> Speed is : " + String(motorSpeed) + "</h3>");
}
void setup(void) {
  pinMode(LFF, OUTPUT);
  pinMode(LFR, OUTPUT);
  pinMode(RFF, OUTPUT);
  pinMode(RFR, OUTPUT);
  pinMode(BLF, OUTPUT);
  pinMode(BLR, OUTPUT);
  pinMode(BRF, OUTPUT);
  pinMode(BRR, OUTPUT);

  digitalWrite(LFF, 0);
  digitalWrite(LFR, 0);
  digitalWrite(RFF, 0);
  digitalWrite(RFR, 0);
  digitalWrite(BLF, 0);
  digitalWrite(BLR, 0);
  digitalWrite(BRF, 0);
  digitalWrite(BRR, 0);

  Serial.begin(115200);

  WiFi.softAP(ssid, password);

  IPAddress apip = WiFi.softAPIP();
  Serial.println(apip);
  //   Wait for connection
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);
  server.on("/Forward", []() {
    server.send(200, "text/html", webPage+ speedtext);
    delay(0.5);
    digitalWrite(LFF, 25);
    //digitalWrite(LFF, 1);
    digitalWrite(LFR, 0);
    digitalWrite(RFF, 1);
    digitalWrite(RFR, 0);
    digitalWrite(BLF, 1);
    digitalWrite(BLR, 0);
    digitalWrite(BRF, 1);
    digitalWrite(BRR, 0);

  }
           );
  server.on("/Backward", []() {
    server.send(200, "text/html", webPage + speedtext);
    delay(0.5);
    digitalWrite(LFF, 0);
    digitalWrite(LFR, 1);
    digitalWrite(RFF, 0);
    digitalWrite(RFR, 1);
    digitalWrite(BLF, 0);
    digitalWrite(BLR, 1);
    digitalWrite(BRF, 0);
    digitalWrite(BRR, 1);


  });
  server.on("/Right", []() {
    server.send(200, "text/html", webPage + speedtext);
    delay(0.5);
    digitalWrite(LFF, 1);
    digitalWrite(LFR, 0);
    digitalWrite(RFF, 0);
    digitalWrite(RFR, 1);
    digitalWrite(BLF, 0);
    digitalWrite(BLR, 1);
    digitalWrite(BRF, 1);
    digitalWrite(BRR, 0);

  });
  server.on("/Left", []() {
    server.send(200, "text/html", webPage + speedtext);
    delay(0.5);
    digitalWrite(LFF, 1);
    digitalWrite(LFR, 0);
    digitalWrite(RFF, 1);
    digitalWrite(RFR, 0);
    digitalWrite(BLF, 1);
    digitalWrite(BLR, 0);
    digitalWrite(BRF, 1);
    digitalWrite(BRR, 0);

  });

  server.on("/stop", []() {
    server.send(200, "text/html", webPage + speedtext);
    digitalWrite(LFF, 0);
    digitalWrite(LFR, 0);
    digitalWrite(RFF, 0);
    digitalWrite(RFR, 0);
    digitalWrite(BLF, 0);
    digitalWrite(BLR, 0);
    digitalWrite(BRF, 0);
    digitalWrite(BRR, 0);
    delay(1);
  });

  server.on("/SpeedUp", []() {
    motorSpeed = motorSpeed + 20;
    setSpeed(motorSpeed);
  });

  server.on("/SpeedDown", []() {
    motorSpeed = motorSpeed - 20;
    setSpeed(motorSpeed);
  });

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server has started");
}

void loop(void) {
  speedtext = "<h3> Speed is : " + String(motorSpeed) + "</h3>";
  server.handleClient();
}
