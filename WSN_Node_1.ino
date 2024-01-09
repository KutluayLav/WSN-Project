#include <SPI.h>
#include <RF24.h>
#include <nRF24L01.h>

RF24 radio(6, 7);  // CE, CSN
#define sensorPin 2
const byte pipe[][6] = { "00001", "00002" };

void setup() {
  pinMode(sensorPin, INPUT);
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(pipe[0]);
  radio.openReadingPipe(1, pipe[1]);
  radio.setPALevel(RF24_PA_MIN);
};

void loop() {
  int sensorData = digitalRead(sensorPin);
  int mn = 1024;
  int mx = 0;
  int result10 = 0;

  radio.stopListening();
  delay(5);

  if (sensorData) {
    for (int i = 0; i < 100; ++i) {
      int val = analogRead(sensorPin);
      mn = min(mn, val);
      mx = max(mx, val);
    }
    int delta = mx - mn;
    result10 = 20 * log10(delta);
  }
  char cstr[15];
  itoa(result10, cstr, 10);
  bool state = radio.write(&cstr, sizeof(cstr));

  delay(5);

  radio.startListening();
  /*
  if (radio.available()) {
    while (radio.available()) {
      char txt[32] = "";
      radio.read(&txt, sizeof(txt));
      Serial.println(txt);
    }
  }*/
  while (!radio.available())
    ;
  char txt[15] = "";
  radio.read(&txt, sizeof(txt));

  if (txt == '6000') {  // 6000 ise internete veri gönderiyor
    while (!radio.available())
      ;
    radio.read(&txt, sizeof(txt));
  }
  Serial.println(txt);
  char combinedDatas[30] = "";
  strcat(combinedDatas, txt);
  strcat(combinedDatas, cstr);
  Serial.println(combinedDatas);

  delay(100);
};
