//Código do receptor da base para o ASR3000 v?
#include <ArduinoJson.h>
#include<LoRaLib.h>
//https://github.com/jgromes/LoRaLib/blob/master/examples/Receive/Receive.ino
#include <LoRa_E220.h>

#include <main.h>
//IMPORTANTE!!! 
#define DEBUG //Comente antes de conectar ao Supervisório

HardwareSerial Serial2;

//LoRa_E220 e220ttl(&Serial2, 22, 4, 18, 21, 19, UART_BPS_RATE_115200); //  esp32 RX <-- e220 TX, esp32 TX --> e220 RX AUX M0 M1
LoRa_E220 e220ttl(&Serial2, 15, 21, 19); //  RX AUX M0 M1
#define DESTINATION_ADDL 2

data_t SensorData;

void setup() {
  Serial.begin(115200);

  e220ttl.begin();

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

  //envia mensagem
	ResponseStatus rs = e220ttl.sendFixedMessage(0, DESTINATION_ADDL, 23, &SensorData, sizeof(data_t));

#ifdef DEBUG
	Serial.println(rs.getResponseDescription());
#endif
}

void loop() {
  delay(1000);
}