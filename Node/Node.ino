#include "aes.h"
#include <SoftwareSerial.h>
#include <stdio.h>
#include <string.h>
#include <DHT.h>
#include <Wire.h>
#include <time.h>

#define LED 5
#define DHT_PIN 4
#define M0 19
#define M1 18
#define TXD2 17
#define RXD2 16

#define BUFFER_SIZE 512

const int DHTTYPE = DHT22;

DHT dht(DHT_PIN, DHTTYPE);

float temp = 0;
float humi = 0;

char text_global[BUFFER_SIZE];
char data_received[BUFFER_SIZE];
char data_send[BUFFER_SIZE];
char data_decrypted[BUFFER_SIZE];

uint8_t key[16] = {
        0x4b, 0x65, 0x79, 0x2d, 0x4d, 0x61, 0x48, 0x6f,
        0x61, 0x41, 0x45, 0x53, 0x2d, 0x31, 0x32, 0x38
    };

void setup() {
  Serial2.begin(115200, SERIAL_8N1, RXD2, TXD2);
  pinMode(LED, OUTPUT);
  dht.begin();
  Serial.begin(115200);
  pinMode(M0, OUTPUT);
  pinMode(M1, OUTPUT);
  digitalWrite(M0, LOW);
  digitalWrite(M1, LOW);
  digitalWrite(LED, HIGH);
}

void sensorDHT(float &temp, float &humi) {
  float t = dht.readTemperature();
  float h = dht.readHumidity();
  if (isnan(t) || isnan(h)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  temp = t;
  humi = h;
}

void sendData(float &temp, float &humi) {
  memset(text_global, 0, sizeof(text_global));
  memset(data_send, 0, sizeof(data_send));
  snprintf(text_global, sizeof(text_global), "!TEMP:%.1f:HUMI:%.1f#", temp, humi);
  clock_t start_encrypt = clock();
  handleDataEncrypt(text_global, data_send, key);
  clock_t end_encrypt = clock();
  double encrypt_time_ns = (double)(end_encrypt - start_encrypt) * 1e9 / CLOCKS_PER_SEC;
  Serial.print("Data global: ");
  Serial.println(text_global);
  Serial.print("Data cipher: ");
  Serial.println(data_send);
  Serial.print("Process time encrypt: ");
  Serial.print(encrypt_time_ns);
  Serial.println("ns");
  Serial2.print(data_send);
  Serial.println("-----------------------------------------------------------------------");
}

void toggleLed(char data[]) {
  String dataStr = String(data); 
  if (dataStr[0] == '!') {
    int index = dataStr.indexOf(':'); 
    if (index > 0) {
      String text = dataStr.substring(1, index); 
      String value = dataStr.substring(index + 1); 
      if (text == "LED") { 
        if (value == "1") {
          digitalWrite(LED, LOW);
        } else if (value == "0") {
          digitalWrite(LED, HIGH); 
        }
      }
    }
  }
}


void handleDataReceived() {
  if (Serial2.available() > 0) {
    memset(data_received, 0, sizeof(data_received));
    memset(data_decrypted, 0, sizeof(data_decrypted));
    size_t m = Serial2.readBytesUntil('\n', data_received, sizeof(data_received) - 1);
    data_received[m] = '\0';
    Serial.print("Data received: ");
    Serial.println(data_received);
    clock_t start_decrypt = clock();
    handleDataDecrypt(data_received, data_decrypted, key);
    clock_t end_decrypt = clock();
    double decrypt_time_ns = (double)(end_decrypt - start_decrypt) * 1e9 / CLOCKS_PER_SEC;
    Serial.print("Data decrypted: ");
    Serial.println(data_decrypted);
    Serial.print("Process time decrypt: ");
    Serial.print(decrypt_time_ns);
    Serial.println("ns");
    toggleLed(data_decrypted);
    Serial.println("-----------------------------------------------------------------------");
  }
}

int counter = 0;
void loop() {
  counter ++;
  if (counter == 10000) {
    sensorDHT(temp, humi);
    sendData(temp, humi);
    counter = 0;
  }
  handleDataReceived();
  delay(1);
}
