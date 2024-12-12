/* include library */
#include <Arduino.h>
#include "WiFi.h"
#include "ESP8266WebServer.h"

WiFiClient client;

/* defining server port */
ESP8266WebServer server(80);

/* Define WiFi Credentials */
const char* ssid = "****"; /* Your SSID */
const char* password = "****"; /* Your Password */

String direction = "";
int robospeed = 800; /* Defining speed limit min 400 to max 1023 */

/* Defining right and left motor pins */
int RMotor_1 = 3;  /* Right Motor GP0 */
int RMotor_2 = 4;  /* Right Motor GP2 */
int LMotor_1 = 1; /* Left Motor GP13 */
int LMotor_2 = 2; /* Left Motor GP15 */

/* Defining L298N enable pins */
int REnable = 10; /* Right Motor Enable Pin GP14 */
int LEnable = 11; /* Right Motor Enable Pin GP12 */

/* Infrared Sensor Pin */
int IRSensor = 0; // Connect the IR sensor to GPIO pin 4

void setup() {
  Serial.begin(115200);
  Serial.println("GPIO test!");

  /* Initialize Motor Control Pins as Output */
  pinMode(RMotor_1, OUTPUT);
  pinMode(RMotor_2, OUTPUT);
  pinMode(LMotor_1, OUTPUT);
  pinMode(LMotor_2, OUTPUT);

  /* Initialize Motor Enable Pins as Output */
  pinMode(REnable, OUTPUT);
  pinMode(LEnable, OUTPUT);

  /* Initialize IR Sensor Pin as Input */
  pinMode(IRSensor, INPUT);

  /* Connecting to WiFi */
  connectingToWiFi();

  server.on("/", HTTP_GET, handleRequest);
  server.onNotFound(handleRequest);

  /* Starting Server */
  server.begin();
  Serial.println("Server started");

  /* Starting Server */
  analogWrite(REnable, robospeed);
  analogWrite(LEnable, robospeed);
}

void loop() {
  /* Handling Request */
  server.handleClient();
  String direction = server.arg("direction");
  Serial.println(direction);

  // Check if the IR sensor detects an obstacle
  if (digitalRead(IRSensor) == LOW && direction == "F") {
    // Obstacle detected and the direction is forward, prevent the car from moving forward
    move_stop();
  } else {
    // No obstacle or other directions, allow the car to move based on the received direction
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
}


void handleRequest() {
  if (server.hasArg("direction")) {
    Serial.println(server.arg("direction"));
  }
  server.send(200, "text/html", "");
  delay(1);
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

/* Move Forward */
void move_forward() {
  digitalWrite(RMotor_1, LOW);
  digitalWrite(RMotor_2, HIGH);
  digitalWrite(LMotor_1, HIGH);
  digitalWrite(LMotor_2, LOW);
}

/* Move Backward */
void move_backward() {
  digitalWrite(RMotor_1, HIGH);
  digitalWrite(RMotor_2, LOW);
  digitalWrite(LMotor_1, LOW);
  digitalWrite(LMotor_2, HIGH);
}

/* Turn Right */
void turn_right() {
  digitalWrite(RMotor_1, LOW);
  digitalWrite(RMotor_2, HIGH);
  digitalWrite(LMotor_1, LOW);
  digitalWrite(LMotor_2, HIGH);
}

/* Turn Left */
void turn_left() {
  digitalWrite(RMotor_1, HIGH);
  digitalWrite(RMotor_2, LOW);
  digitalWrite(LMotor_1, HIGH);
  digitalWrite(LMotor_2, LOW);
}

/* Stop Move */
void move_stop() {
  digitalWrite(RMotor_1, LOW);
  digitalWrite(RMotor_2, LOW);
  digitalWrite(LMotor_1, LOW);
  digitalWrite(LMotor_2, LOW);
}

