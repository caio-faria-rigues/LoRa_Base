//Código do receptor da base para o ASR3000 v?
#include <ArduinoJson.h>
#include<LoRaLib.h>
//https://github.com/jgromes/LoRaLib/blob/master/examples/Receive/Receive.ino
#include <LoRa_E220.h>

//IMPORTANTE!!! 
#define DEBUG //Comente antes de conectar ao Supervisório

#include <main.h>
JsonDocument doc;

HardwareSerial Serial2;

//LoRa_E220 e220ttl(&Serial2, 22, 4, 18, 21, 19, UART_BPS_RATE_115200); //  esp32 RX <-- e220 TX, esp32 TX --> e220 RX AUX M0 M1
LoRa_E220 e220ttl(&Serial2, 15, 21, 19); //  RX AUX M0 M1
#define DESTINATION_ADDL 2

data_t LoRaData;
float loraRSSI;

void setup() {
  Serial.begin(115200);

  e220ttl.begin();
}

void loop() {

if (e220ttl.available()>1) {
      // read the String message
  #ifdef ENABLE_RSSI
      ResponseStructContainer rsc = e220ttl.receiveMessageRSSI(sizeof(data_t));
  #else
      ResponseStructContainer rsc = e220ttl.receiveMessage(sizeof(data_t));
  #endif

    // Is something goes wrong print error
    if (rsc.status.code!=1){
      Serial.println(rsc.status.getResponseDescription());
    }else{
      // Print the data received
      Serial.println(rsc.status.getResponseDescription());
      LoRaData = *(data_t*) rsc.data;
      Serial.println(LoRaData.time);
      Serial.println(LoRaData.temperature);
      Serial.println(LoRaData.latitude);

  #ifdef ENABLE_RSSI
      Serial.print("RSSI: "); Serial.println(rsc.rssi, DEC);
      loraRSSI = rsc.rssi;
  #endif
    }
  }

  doc['Alt'] = LoRaData.bmp_altitude;
  doc['Temp'] = LoRaData.temperature;
  doc['Lat'] = LoRaData.latitude;
  doc['Long'] = LoRaData.longitude;

  doc['Acel(x)'] = LoRaData.AcelX;
  doc['Acel(y)'] = LoRaData.AcelY;
  doc['Acel(z)'] = LoRaData.AcelZ;

  doc['Gyro(x)'] = LoRaData.GyroX;
  doc['Gyro(y)'] = LoRaData.GyroY;
  doc['Gyro(z)'] = LoRaData.GyroZ;

  doc['RSSI'] = loraRSSI;
  doc['timestamp'] = LoRaData.time;

  //envia o json para a porta SERIAL
  serializeJson(doc, Serial);
}