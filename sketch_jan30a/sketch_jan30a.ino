#include <CAN.h>

#define ENCODER_PIN 34

int encoderValue = 0;
uint8_t *encoderPtr = (uint8_t*)&encoderValue;

void setup() {
  Serial.begin(115200);
  delay(1000);

  //Inicializando o CANBUS em 500k de baud
  if (!CAN.begin(500E3)) 
  {
    Serial.println("Transceiver CAN ausente");
    while (1000);
  }
}

void loop() {
  encoderValue = analogRead(ENCODER_PIN);
  Serial.println(String(encoderValue));

  //Começa pacote CAN
  CAN.beginPacket(0x12);


  //CAN.write(encoderPtr, sizeof(encoderValue));
  CAN.endPacket();

  delay(250);
}
