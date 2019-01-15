/*
   This program can receive data from a Raspberry configured with Node-Red, and send some codes with 433 Mhz sender, and
   he can also send data to Raspberry (also with Node-Red), data is coming from a CO2 sensor, that use a library that
   need to be configured with value here :
   https://www.esrl.noaa.gov/gmd/ccgg/trends/gl_trend.html
*/

#include <RCSwitch.h>
#include "MQ135.h"
#include <ArduinoJson.h>

RCSwitch transmitter = RCSwitch();

//==========================================
// Constant fields
//==========================================

//Constants for 433 MHz module
#define NEGATIVE_433_PIN 6
#define POSITIVE_433_PIN 5
#define DATA_433_PIN 4

#define LED_CONFIRM_PIN 13 // Confirmation led

#define MOV_SENSOR 3    // Movement sensor to automatically power on light

#define mq135Pin A7     // Pin connected to MQ135

// Used to control remote outlet with 433 MHz, this is the 3 firsts outlets with the same code for On and Off
#define REMOTE_OUTLET_1 5330371
#define REMOTE_OUTLET_2 5330224
#define REMOTE_OUTLET_3 5330188

// Used to control remote outlet with 433 MHz, this is the outlets with one code for On and another code for Off
#define REMOTE_OUTLET_4_ON 283955
#define REMOTE_OUTLET_4_OFF 283964

#define REMOTE_OUTLET_5_ON 284099
#define REMOTE_OUTLET_5_OFF 284108

#define REMOTE_OUTLET_6_ON 284419
#define REMOTE_OUTLET_6_OFF 284428

#define REMOTE_OUTLET_7_ON 285955
#define REMOTE_OUTLET_7_OFF 285964

#define REMOTE_OUTLET_8_ON 292099
#define REMOTE_OUTLET_8_OFF 292108

//==========================================
// fields
//==========================================

MQ135 gasSensor = MQ135(mq135Pin);  // Initialize object MQ135 on the specified pin
long currentTime;

//==========================================
// Setup Method
//==========================================

void setup()
{
  Serial.begin(9600);

  pinMode(NEGATIVE_433_PIN, OUTPUT);
  digitalWrite(NEGATIVE_433_PIN, LOW);
  pinMode(POSITIVE_433_PIN, OUTPUT);
  digitalWrite(POSITIVE_433_PIN, HIGH);
  pinMode(LED_CONFIRM_PIN, OUTPUT);
  digitalWrite(LED_CONFIRM_PIN, LOW);

  // 433 MHz initialisation
  transmitter.enableTransmit(DATA_433_PIN);
  transmitter.setPulseLength(166);

  // MQ-135 initialisation
  currentTime = millis();

  // Movement sensor initialisation, with interrupt
  pinMode(MOV_SENSOR, INPUT);
  attachInterrupt(digitalPinToInterrupt(MOV_SENSOR), somethingDetected, CHANGE);
}

//==========================================
// Loop Method
//==========================================

void loop()
{
  if (Serial.available())
    sendData433(Serial.readString());

  // MQ-135 measure every 30 seconds
  if (millis() - currentTime > 30000)
  {
    sendPPMSensorValue();
    sendPPMSensorResistance();
    currentTime = millis();
  }
}

//==========================================
// Send 433 MHZ Data Method
//==========================================

void sendData433(String comSerie)
{
  // 3 outlets with one code for on and off
  if (comSerie == "prise-sans-terre-1") {
    transmitter.send(REMOTE_OUTLET_1, 24);
  }
  else if (comSerie == "prise-sans-terre-2") {
    transmitter.send(REMOTE_OUTLET_2, 24);
  }
  else if (comSerie == "prise-sans-terre-3") {
    transmitter.send(REMOTE_OUTLET_3, 24);
  }

  // 5 outlets with one code for on and another code for off
  else if (comSerie == "prise1on") {
    transmitter.send(REMOTE_OUTLET_4_ON, 24);
  }
  else if (comSerie == "prise1off") {
    transmitter.send(REMOTE_OUTLET_4_OFF, 24);
  }
  else if (comSerie == "prise2on") {
    transmitter.send(REMOTE_OUTLET_5_ON, 24);
  }
  else if (comSerie == "prise2off") {
    transmitter.send(REMOTE_OUTLET_5_OFF, 24);
  }
  else if (comSerie == "prise3on") {
    transmitter.send(REMOTE_OUTLET_6_ON, 24);
  }
  else if (comSerie == "prise3off") {
    transmitter.send(REMOTE_OUTLET_6_OFF, 24);
  }
  else if (comSerie == "prise4on") {
    transmitter.send(REMOTE_OUTLET_7_ON, 24);
  }
  else if (comSerie == "prise4off") {
    transmitter.send(REMOTE_OUTLET_7_OFF, 24);
  }
  else if (comSerie == "prise5on") {
    transmitter.send(REMOTE_OUTLET_8_ON, 24);
  }
  else if (comSerie == "prise5off") {
    transmitter.send(REMOTE_OUTLET_8_OFF, 24);
  }
  else {
    return;
  }

  confirmWithLed();
}

//==========================================
// Led Confirmation Method
//==========================================

void confirmWithLed()
{
  digitalWrite(LED_CONFIRM_PIN, HIGH);
  delay(500);
  digitalWrite(LED_CONFIRM_PIN, LOW);
}

//==========================================
// Get CO2 PPM Method
//==========================================

void sendPPMSensorValue()
{
  sendJsonToSerial("A", String(gasSensor.getPPM()));
}

void sendPPMSensorResistance()
{
  sendJsonToSerial("B", String(gasSensor.getRZero()));
}

//==========================================
// Movement sensor Method
//==========================================

void somethingDetected() // Need to change this, now is for debug only
{
  if (digitalRead(MOV_SENSOR))
    sendJsonToSerial("C", "movement-true");
  if (!digitalRead(MOV_SENSOR))
    sendJsonToSerial("D", "movement-false");
}

//==========================================
// Json Method
//==========================================

void sendJsonToSerial(String source, String data)
{
  const size_t bufferSize = JSON_OBJECT_SIZE(2);
  DynamicJsonBuffer jsonBuffer(bufferSize);
  
  JsonObject& root = jsonBuffer.createObject();
  root["source"] = source;
  root["data"] = data;
  root.printTo(Serial);
  Serial.println();
}
