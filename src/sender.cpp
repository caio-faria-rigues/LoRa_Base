//Código do receptor da base para o ASR3000 v?
#include <ArduinoJson.h>
#include<LoRaLib.h>
//https://github.com/jgromes/LoRaLib/blob/master/examples/Receive/Receive.ino

//IMPORTANTE!!! 
#define DEBUG //Comente antes de conectar ao Supervisório

#include <main.h>

//trocar para a versão disponível do LoRa (SX272, SX273, SX276, SX277, SX278, SX279)
//o último parâmetro de LoRa() é uma instância de SPI, use se precisar
SX1272 lora = new LoRa(CS_PIN, DIO0_PIN, DIO1_PIN);

// Data structure to store sensor data
typedef struct // size = 64 bytes
{
    int32_t time;
    uint32_t status;

    float pressure;
    float temperature;
    float bmp_altitude;
    float max_altitude;

    float AcelX;
    float AcelY;
    float AcelZ;

    float GyroX;
    float GyroY;
    float GyroZ;

    float latitude;
    float longitude;
    float gps_altitude;
    float hor_speed;
} data_t;

size_t len = 64;
byte loraByteArray[64];
data_t SensorData;

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

  SensorData.time = 123456;
  SensorData.temperature = 50.0;
  SensorData.status = 2;
  SensorData.pressure = 125.2;
  SensorData.max_altitude = 4000.0;
  SensorData.longitude = 123456.0;
  SensorData.latitude = 987654.0;
  SensorData.hor_speed = 12.0;
  SensorData.GyroX = 0.1;
  SensorData.GyroY = 0.2;
  SensorData.GyroZ = 0.3;
  SensorData.AcelX = 0.9;
  SensorData.AcelY = 0.8;
  SensorData.AcelZ = 0.7;
  SensorData.bmp_altitude = 3500.5;
  SensorData.gps_altitude = 3505.5;

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
    Serial.print(F("Enviando pacote ..."));

    //copia os bytes do struct em uma byteArray
    memcpy(loraByteArray, &SensorData, sizeof(SensorData));

    int state = lora.transmit(loraByteArray, 64);

#ifdef DEBUG:
    if (state == ERR_NONE) {
        Serial.println(F("Pacote enviado com sucesso!\n"));
        
        for (int i=0;i<len;i++) Serial.print(loraByteArray[i]);

        Serial.print(F("\nDatarate:\t"));
        Serial.print(lora.getDataRate());
        Serial.println(F(" bps"));

    } else if (state == ERR_PACKET_TOO_LONG) {
    Serial.println(F("ERRO: pacote maior que 256 bytes!"));

    } else if (state == ERR_TX_TIMEOUT) {
    Serial.println(F("ERRO: timeout!"));
    }
#endif

  delay(1000);
}