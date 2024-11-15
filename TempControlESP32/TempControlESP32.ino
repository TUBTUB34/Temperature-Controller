#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <U8g2lib.h>
#include <Wire.h>
#include <Servo.h>

Servo myServo; // Create a Servo object
int servoPosition = 0;

#ifndef STASSID
#define STASSID //add wifi name here 
#define STAPSK // wifi password here
#endif

const char *ssid = STASSID;
const char *password = STAPSK;

int warmPosition = 15;
int hotPosition = 10;
int coldPosition = 20;

ESP8266WebServer server(80);

U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, /* reset=*/12, /* clock=*/14, U8X8_PIN_NONE);  // Initialize u8g2 display object
int val = HIGH;

uint32_t warnCount = 0;    // Counter for Warn state
uint32_t hotCount = 0;     // Counter for Hot state
uint32_t coldCount = 0;    // Counter for Cold state

enum LedState { WARN, HOT, COLD };
LedState currentLedState = WARN;

void handleRoot() {
  int sec = millis() / 1000;
  int min = sec / 60;
  int hr = min / 60;

  StreamString temp;
  temp.reserve(1500);  // Preallocate a larger chunk for content
  temp.printf("<html>\
  <head>\
    <title>Temperature State Controller</title>\
    <style>\
      body { font-family: Arial, sans-serif; background: #ff6666; color: #333; padding: 0; }\
      .container { max-width: 800px; margin: 20px auto; background-color: #fff; border-radius: 10px; padding: 20px; }\
      h1 { color: #FF5733; font-size: 48px; text-align: center; margin-bottom: 20px; }\
      p { font-size: 24px; line-height: 1.5; margin-bottom: 10px; }\
      button { font-size: 24px; padding: 15px 30px; background-color: #FFC300; color: #fff; border-radius: 8px; cursor: pointer; }\
      button:hover { background-color: #FF5733; transform: scale(1.1); }\
      ul { list-style-type: none; padding: 0; margin-top: 20px; text-align: center; }\
      li { font-size: 24px; margin-bottom: 10px; }\
    </style>\
    <script>\
      function sendRequest(endpoint) {\
        var xhr = new XMLHttpRequest();\
        xhr.open('GET', endpoint, true);\
        xhr.send();\
      }\
    </script>\
  </head>\
<body>\
  <div class='container'>\
    <h1>Temperature State Controller</h1>\
    <p>Uptime: %02d:%02d:%02d</p>\
    <p>Select a state:</p>\
    <div>\
      <button onclick='sendRequest(\"/warn\")'>Warn</button>\
      <button onclick='sendRequest(\"/hot\")'>Hot</button>\
      <button onclick='sendRequest(\"/cold\")'>Cold</button>\
    </div>\
    <ul>\
      <li>Warn Count: <span id='warnCount'>%u</span></li>\
      <li>Hot Count: <span id='hotCount'>%u</span></li>\
      <li>Cold Count: <span id='coldCount'>%u</span></li>\
    </ul>\
  </div>\
</body>\
</html>",
              hr, min % 60, sec % 60, warnCount, hotCount, coldCount);

  server.send(200, "text/html", temp.c_str());
}

void dispMessage() {
  u8g2.clearBuffer();  // Clear the display buffer

  // Display the count for each state on the OLED
  u8g2.setFont(u8g2_font_6x10_tf);
  u8g2.setCursor(0, 10);
  u8g2.print("Warn: ");
  u8g2.print(warnCount);
  u8g2.setCursor(64, 10);
  u8g2.print("Hot: ");
  u8g2.print(hotCount);
  u8g2.setCursor(0, 30);
  u8g2.print("Cold: ");
  u8g2.print(coldCount);

  // Draw different visual patterns for each state
  switch (currentLedState) {
    case WARN:
      u8g2.drawBox(10, 40, 20, 20);  // Display a square for "Warn"
      break;
    case HOT:
      u8g2.drawDisc(30, 50, 10);  // Display a filled circle for "Hot"
      break;
    case COLD:
      u8g2.drawFrame(60, 40, 20, 20);  // Display an outlined square for "Cold"
      break;
  }

  u8g2.sendBuffer();  // Send the buffer to the display
}

void handleWarn() {
  currentLedState = WARN;
  warnCount++;
  myServo.write(warmPosition);
  server.send(200, "text/plain", "Warn selected");
}

void handleHot() {
  currentLedState = HOT;
  hotCount++;
  myServo.write(hotPosition);
  server.send(200, "text/plain", "Hot selected");
}

void handleCold() {
  currentLedState = COLD;
  coldCount++;
  myServo.write(coldPosition);
  server.send(200, "text/plain", "Cold selected");
}

void setup(void) {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, val);
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");
  u8g2.begin();

  // Wait for connection
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
  server.on("/warn", handleWarn);
  server.on("/hot", handleHot);
  server.on("/cold", handleCold);
  server.onNotFound([]() {
    String message = "File Not Found\n\n";
    message += "URI: ";
    message += server.uri();
    message += "\n";
    server.send(404, "text/plain", message);
  });
  server.begin();
  Serial.println("HTTP server started");
}

void loop(void) {
  server.handleClient();
  MDNS.update();
  dispMessage();
}
