#include "aes.h"
#include <SoftwareSerial.h>
#include <stdio.h>
#include <string.h>
#include <Wire.h>
#include <time.h>

#define BUTTON 5
#define M0 19
#define M1 18
#define TXD2 17
#define RXD2 16

#define BUFFER_SIZE 512

char text_global[BUFFER_SIZE];
char data_send[BUFFER_SIZE];
char data_received[BUFFER_SIZE];
char data_decrypted[BUFFER_SIZE];


uint8_t key[16] = {
        0x4b, 0x65, 0x79, 0x2d, 0x4d, 0x61, 0x48, 0x6f,
        0x61, 0x41, 0x45, 0x53, 0x2d, 0x31, 0x32, 0x38
    };

void setup() {
  Serial2.begin(115200, SERIAL_8N1, RXD2, TXD2);
  pinMode(BUTTON, INPUT);
  Serial.begin(115200);
  pinMode(M0, OUTPUT);
  pinMode(M1, OUTPUT);
  digitalWrite(M0, LOW);
  digitalWrite(M1, LOW);
}

void handleDataReceived() {
  if (Serial2.available() > 0) {
    memset(data_received, 0, sizeof(data_received));
    memset(data_decrypted, 0, sizeof(data_decrypted));
    size_t m = Serial2.readBytesUntil('\n', data_received, sizeof(data_received) - 1);
    // data_received[m] = '\0';
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
    Serial.println("-----------------------------------------------------------------------");
  }
}

void sendData() {
  memset(text_global, 0, sizeof(text_global));
  memset(data_send, 0, sizeof(data_send));

  int index = 0;
  while (Serial.available() > 0 && index < BUFFER_SIZE - 1) { 
    char c = Serial.read(); 
    if (c == '\n') break;  
    text_global[index++] = c;
  }
  text_global[index] = '\0'; 
  if (text_global[0] != '\0') {
    Serial.print("Data: ");
    Serial.println(text_global);

    handleDataEncrypt(text_global, data_send, key);

    Serial.print("Text cipher: ");
    Serial.println(data_send);
    Serial.println("-----------------------------------------------------------------------");
    Serial2.print(data_send); 
  }
}

void loop() {
  sendData();
  handleDataReceived();
  delay(1);
}
