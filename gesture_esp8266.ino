#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

WiFiServer server(80);

const char* ssid = "****"; /* Your SSID */
const char* password = "****"; /* Your Password */

String direction = "";
int robospeed = 800; /* Defining speed limit min 400 to max 1023 */

int RMotor_1 = D3;  /* Right Motor GP0 */
int RMotor_2 = D4;  /* Right Motor GP2 */
int LMotor_1 = D1; /* Left Motor GP13 */
int LMotor_2 = D2; /* Left Motor GP15 */

int REnable = D5; /* Right Motor Enable Pin GP14 */
int LEnable = D6; /* Right Motor Enable Pin GP12 */

int IRSensor = A0; // Connect the IR sensor to Analog pin A0

void setup() {
  Serial.begin(115200);
  Serial.println("GPIO test!");

  pinMode(RMotor_1, OUTPUT);
  pinMode(RMotor_2, OUTPUT);
  pinMode(LMotor_1, OUTPUT);
  pinMode(LMotor_2, OUTPUT);

  pinMode(REnable, OUTPUT);
  pinMode(LEnable, OUTPUT);

  pinMode(IRSensor, INPUT);

  connectingToWiFi();

  server.begin();
  Serial.println("Server started");

  analogWrite(REnable, robospeed);
  analogWrite(LEnable, robospeed);
}

void loop() {
  WiFiClient client = server.available();
  
  if (client) {
    String currentLine = "";
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        if (c == '\n') {
          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
            String direction = server.arg("direction");
            Serial.println(direction);

            if (digitalRead(IRSensor) == LOW && direction == "F") {
              move_stop();
            } else {
              if (direction == "F") {
                move_forward();
              } else if (direction == "B") {
                move_backward();
              } else if (direction == "R") {
                turn_right();
              } else if (direction == "L") {
                turn_left();
              } else if (direction == "S") {
                move_stop();
              }
            }
            
            break;
          } else {
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;
        }
      }
    }
    client.stop();
    Serial.println("Client disconnected");
  }
}

void connectingToWiFi() {
  delay(3000);
  WiFi.disconnect(true);
  delay(1000);
  Serial.println("Connecting to WiFi");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  Serial.println("Connected");
  Serial.println("Local IP is : ");
  Serial.print(WiFi.localIP());
  delay(5000);
}

void move_forward() {
  digitalWrite(RMotor_1, LOW);
  digitalWrite(RMotor_2, HIGH);
  digitalWrite(LMotor_1, HIGH);
  digitalWrite(LMotor_2, LOW);
}

void move_backward() {
  digitalWrite(RMotor_1, HIGH);
  digitalWrite(RMotor_2, LOW);
  digitalWrite(LMotor_1, LOW);
  digitalWrite(LMotor_2, HIGH);
}

void turn_right() {
  digitalWrite(RMotor_1, LOW);
  digitalWrite(RMotor_2, HIGH);
  digitalWrite(LMotor_1, LOW);
  digitalWrite(LMotor_2, HIGH);
}

void turn_left() {
  digitalWrite(RMotor_1, HIGH);
  digitalWrite(RMotor_2, LOW);
  digitalWrite(LMotor_1, HIGH);
  digitalWrite(LMotor_2, LOW);
}

void move_stop() {
  digitalWrite(RMotor_1, LOW);
  digitalWrite(RMotor_2, LOW);
  digitalWrite(LMotor_1, LOW);
  digitalWrite(LMotor_2, LOW);
}
