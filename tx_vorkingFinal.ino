#include "SPI.h"
#include "nRF24L01.h"
#include "RF24.h"

const byte pipe[6] = "00007";           // адрес передатчика
uint8_t ch = 120;   
// выбор радиоканала (0 - 127)
RF24 radio(9,10);                       // (CE, CS)

// Структура для хранения RC-команды (4 байта)
struct ControlData {
  uint8_t joyXL;         // ось X вниз 0...255
  uint8_t joyXH;         // ось X вверх 0...255
  uint8_t joyYL;         // ось Y влево 0...255
  uint8_t joyYH;         // ось Y вправо 0...255
  uint8_t button123;     // кнопки 3 шт побитно
} control_data;

void setup() {
  control_data.joyXL = 1;
  control_data.joyXH = 2;
  control_data.joyYL = 3;
  control_data.joyYH = 4;
  control_data.button123 = 5;

  Serial.begin(115200);
  
  radio.begin();
  radio.setChannel(ch); // выбор радиоканала (0 - 127)
  radio.setDataRate(RF24_1MBPS); // скорость RF24_250KBPS, RF24_1MBPS, RF24_2MBPS
  radio.setPALevel(RF24_PA_HIGH); // мощность _MIN=-18dBm, _LOW=-12dBm, _MED=-6dBM
  radio.openWritingPipe(pipe);
  radio.stopListening();
}





void loop() {
  delay(40);
  control_data.joyXL = map(analogRead(A0),0,1023,0,180);
    control_data.joyXH = map(analogRead(A1),0,1023,0,255);
    
  // передача радиокоманды
  radio.write(&control_data, sizeof(control_data));
  radio.write(&control_data, sizeof(control_data));
 Serial.print(control_data.joyXL);
    Serial.print('\t');
    Serial.print(control_data.joyXH);
    Serial.print('\t');
    Serial.print(control_data.joyYL);
    Serial.print('\t');
    Serial.print(control_data.joyYH);
    Serial.print('\t');
  Serial.println();
}

