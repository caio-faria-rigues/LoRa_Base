#define LORA_FREQ 915.0f
#define LORA_BW 125.0f
#define LORA_SF 9u
#define LORA_CR 7u
#define LORA_SYNCWORD 0x12u //ou 8
#define LORA_PL 8u

#define LORA_POWER 20
#define LORA_CURRENTLIMIT 100u
#define LORA_GAIN 0u

#define CS_PIN 4
#define DIO0_PIN 2
#define DIO1_PIN 3

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
