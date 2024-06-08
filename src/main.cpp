//Código do receptor da base para o ASR3000 v?
#include <ArduinoJson.h>
#include<LoRaLib.h>
//https://github.com/jgromes/LoRaLib/blob/master/examples/Receive/Receive.ino

//IMPORTANTE!!! 
#define DEBUG //Comente antes de conectar ao Supervisório

#include <main.h>
JsonDocument doc;

//trocar para a versão disponível do LoRa (SX272, SX273, SX276, SX277, SX278, SX279)
//o último parâmetro de LoRa() é uma instância de SPI, use se precisar
SX1272 lora = new LoRa(CS_PIN, DIO0_PIN, DIO1_PIN);

int32_t lora_time;
uint32_t lora_status;
float lora_pressure;
float lora_temperature;
float lora_bmpAltitude;
float lora_maxAltitude;

float lora_AcelX;
float lora_AcelY;
float lora_AcelZ;

float lora_GyroX;
float lora_GyroY;
float lora_GyroZ;

float lora_latitude;
float lora_longitude;
float lora_gpsAltitude;
float lora_HorSpeed;

float loraRSSI;

void setup() {
  Serial.begin(115200);

  int state = lora.begin(
    LORA_FREQ,
    LORA_BW,
    LORA_SF,
    LORA_CR,
    LORA_SYNCWORD,
    LORA_POWER,
    LORA_CURRENTLIMIT,
    LORA_PL,
    LORA_GAIN
  );

#ifdef DEBUG
  if (state == ERR_NONE) {
    Serial.println(F("LoRa iniciado com sucesso!"));
  } else {
    Serial.print(F("ERRO: "));
    Serial.println(state);
    while (true);
  }
#endif
}

void loop() {

  size_t len = 64;
  byte loraByteArray[len];
  int state = lora.receive(loraByteArray, len);

#ifdef DEBUG
  if (state == ERR_NONE) {
    Serial.println(F("Mensagem Recebida!"));
  } else {
    Serial.println("Erro ao receber a mensagem: ");
    Serial.println(state);
  }
#endif

  //copia do byte 0 ao 3 de loraByteArray para a variável lora_time
  memcpy(&lora_time, &loraByteArray[0], sizeof(lora_time));
  memcpy(&lora_status, &loraByteArray[4], sizeof(lora_status));

  memcpy(&lora_pressure, &loraByteArray[8], sizeof(lora_pressure));
  memcpy(&lora_temperature, &loraByteArray[12], sizeof(lora_temperature));
  memcpy(&lora_bmpAltitude, &loraByteArray[16], sizeof(lora_bmpAltitude));
  memcpy(&lora_maxAltitude, &loraByteArray[20], sizeof(lora_maxAltitude));

  memcpy(&lora_AcelX, &loraByteArray[24], sizeof(lora_AcelX));
  memcpy(&lora_AcelY, &loraByteArray[28], sizeof(lora_AcelY));
  memcpy(&lora_AcelZ, &loraByteArray[32], sizeof(lora_AcelZ));

  memcpy(&lora_GyroX, &loraByteArray[36], sizeof(lora_GyroX));
  memcpy(&lora_GyroY, &loraByteArray[40], sizeof(lora_GyroY));
  memcpy(&lora_GyroZ, &loraByteArray[44], sizeof(lora_GyroZ));

  memcpy(&lora_latitude, &loraByteArray[48], sizeof(lora_latitude));
  memcpy(&lora_longitude, &loraByteArray[52], sizeof(lora_longitude));
  memcpy(&lora_gpsAltitude, &loraByteArray[56], sizeof(lora_gpsAltitude));
  memcpy(&lora_HorSpeed, &loraByteArray[60], sizeof(lora_HorSpeed));

  loraRSSI = lora.getRSSI();

  doc['Alt'] = lora_bmpAltitude;
  doc['Temp'] = lora_temperature;
  doc['Lat'] = lora_latitude;
  doc['Long'] = lora_longitude;

  doc['Acel(x)'] = lora_AcelX;
  doc['Acel(y)'] = lora_AcelY;
  doc['Acel(z)'] = lora_AcelZ;

  doc['Gyro(x)'] = lora_GyroX;
  doc['Gyro(y)'] = lora_GyroY;
  doc['Gyro(z)'] = lora_GyroY;

  doc['RSSI'] = loraRSSI;
  doc['timestamp'] = lora_time;

  //envia o json para a porta SERIAL
  serializeJson(doc, Serial);
}