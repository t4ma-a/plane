#include "SPI.h"
#include "nRF24L01.h"
#include "RF24.h"
#include <Servo.h>

Servo servo_mech;

// Настройки радиомодуля
const byte pipe[6] = "00007";     // адрес
uint8_t ch = 120;                 // выбор радиоканала (0 - 127)
RF24 radio(9,10);                 // (CE, CS)

// Структура для радиокоманды (5 байта)
struct ControlData {
  uint8_t joyXL;         // джойстик ось X
  uint8_t joyXH;         //
  uint8_t joyYL;         // джойстик ось Y
  uint8_t joyYH;         //
  uint8_t button123;     // кнопки меча, щита, джойстика
} rControl;

void setup() {
  servo_mech.attach(6);
  servo_mech.write(90);
  
  Serial.begin(115200);
   
  radio.begin();
  radio.setChannel(ch);
  radio.setDataRate(RF24_1MBPS);
  radio.setPALevel(RF24_PA_HIGH);
  radio.openReadingPipe(1, pipe);
  radio.startListening();

  //while(1);
}




void loop() {
  if (radio.available()) {
    rc_comm();
  }
}

void rc_comm() {
    radio.read(&rControl, sizeof(rControl));

    Serial.print(rControl.joyXL);
    Serial.print('\t');
    Serial.print(rControl.joyXH);
    Serial.print('\t');
    Serial.print(rControl.joyYL);
    Serial.print('\t');
    Serial.print(rControl.joyYH);
    Serial.print('\t');
    Serial.println(rControl.button123, BIN);
    servo_mech.write(rControl.joyXL);
    digitalWrite(7,rControl.joyXH);
}

