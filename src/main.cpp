#include "HardwareSerial.h"
#include "esp32-hal-gpio.h"
#include "esp32-hal.h"
#include "pins_arduino.h"
#include <Arduino.h>
#include <Print.h>
#include <cstdlib>
#include "BluetoothSerial.h"

const int IN1 = 4;
const int IN2 = 16;

const int IN3 = 12;
const int IN4 = 13;

BluetoothSerial SerialBT;

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run make menuconfig to and enable it
#endif
#if !defined(CONFIG_BT_SPP_ENABLED)
#error Serial Bluetooth not available or not enabled. It is only available for the ESP32 chip.
#endif

void btAuthCallback(uint32_t pin) {
  Serial.printf("PIN request from device, displaying: %06dn", pin);
  SerialBT.confirmReply(true);  // Confirm the PIN
}

// 28:05:A5:33:81:AA
void setup() {
  Serial.begin(115200);
  SerialBT.begin("clanker");
  SerialBT.enableSSP();
  SerialBT.onConfirmRequest(btAuthCallback);
  //SerialBT.setPin("1234");

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  Serial.println(".................");
}

// frente: F
// tras: B
// direita: R
// esq: L
// f: X, x
// para: S

void loop() {
  digitalWrite(IN1, 0);
  digitalWrite(IN2, 0);
  digitalWrite(IN3, 0);
  digitalWrite(IN4, 0);

  static int i = 0, bmsg_count = 0;

  digitalWrite(LED_BUILTIN, i++ & 1);

  static int a = 0, b = 0;
  static int c = 0, d = 0;
  // d = "direcao da rotacao da roda"

  int cmd = 0;
  if (Serial.available())
    cmd = Serial.read();

  if (SerialBT.available())
    cmd = SerialBT.read();

  Serial.printf("%c\n", cmd);

  switch (cmd) {
    case 'S':
      a = 0;
      b = 0;
      c = 0;
      d = 0;
      break;

    case 'B':
      a = 1;
      b = 0;
      c = 1;
      d = 0;
      break;
  
    case 'F':
      a = 0;
      b = 1;
      c = 0;
      d = 1;
      break;

    case 'R':
      a = 1;
      b = 0;
      c = 0;
      d = 1;
      break;

    case 'L':
      a = 0;
      b = 1;
      c = 1;
      d = 0;
      break;
  }

  digitalWrite(IN1, a);
  digitalWrite(IN2, b);
  digitalWrite(IN3, c);
  digitalWrite(IN4, d);

  delay(50);
}