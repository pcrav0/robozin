#include "esp32-hal-gpio.h"
#include "esp32-hal.h"
#include "pins_arduino.h"
#include <Arduino.h>
#include <Print.h>
#include <cstdlib>
#include "BluetoothSerial.h"

const int IN1 = 4; 
const int IN2 = 5;

const int IN3 = 12;
const int IN4 = 13;

BluetoothSerial SerialBT;

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run make menuconfig to and enable it
#endif
#if !defined(CONFIG_BT_SPP_ENABLED)
#error Serial Bluetooth not available or not enabled. It is only available for the ESP32 chip.
#endif

void setup() {
  Serial.begin(115200);
  SerialBT.begin("clanker");

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  String MAC_Address = SerialBT.getBtAddressString();
  Serial.println(MAC_Address.c_str());
}

void loop() {
  static int i = 0;
  delay(0);

  digitalWrite(LED_BUILTIN, i++ & 1);

  int c1 = Serial.available();
  int c2 = SerialBT.available();
  if (c1) {
    SerialBT.write(Serial.read());
  }
  if (c2) {
    Serial.write(SerialBT.read());
  }

  if (c1 || c2) {
    Serial.println("!!!");
  }

  delay(200);
  return;

  static int a = 0, b = 0;
  static int as = 1, bs = 1;

  digitalWrite(IN1, 0);
  digitalWrite(IN2, 1);
  digitalWrite(IN3, 0);
  digitalWrite(IN4, 1);

  //delay(20);

  //digitalWrite(IN1, 0);
  //digitalWrite(IN2, 0);
  //digitalWrite(IN3, 0);
  //digitalWrite(IN4, 0);

}