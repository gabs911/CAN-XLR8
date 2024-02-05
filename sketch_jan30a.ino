#include <CAN.h>


const int encoderPin = 34;
int encoderValue = 0;
int *encoderValue_ptr = &encoderValue;

void setup() {
  Serial.begin(115200);
  delay(1000);

  //Inicializando o CANBUS em 500k de baud
  if (!CAN.begin(500E3)) {
    Serial.println("Transceiver CAN ausente");
    while (1);
  }
}

void loop() {
  encoderValue = analogRead(encoderPin);
  Serial.println(String(*encoderValue_ptr));

  //Começa pacote CAN
  CAN.beginPacket(0x12);

  //Mandar valor de 10 bits do ADC
  CAN.write((const uint8_t*)encoderValue_ptr, sizeof(encoderValue));
  CAN.endPacket();

  delay(5);
}