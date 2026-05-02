#include <Wire.h>
#include <RTClib.h>
#include <DFRobotDFPlayerMini.h>
#include <SoftwareSerial.h>

RTC_DS3231 rtc;
SoftwareSerial mySerial(10, 11); // RX, TX untuk DFPlayer
DFRobotDFPlayerMini dfPlayer;

const int sensorPin = 3;  // Pin sensor digital
const int speaker = 2;    // Pin speaker
const int button1 = 4;    // Tombol 1
const int button2 = 5;    // Tombol 2

bool objectDetected = false;
unsigned long startTime = 0;
unsigned long timer1 = 60000;  // 1 menit
unsigned long timer2 = 600000; // 10 menit
bool timer1Active = false;
bool timer2Active = false;
unsigned long timer2Remaining = timer2;

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);  // Komunikasi dengan DFPlayer

  pinMode(speaker, OUTPUT);
  pinMode(sensorPin, INPUT);
  pinMode(button1, INPUT_PULLUP);
  pinMode(button2, INPUT_PULLUP);

  if (!rtc.begin()) {
    Serial.println("RTC tidak terdeteksi!");
    while (1);
  }

  if (rtc.lostPower()) {
    Serial.println("RTC kehilangan daya, mengatur ulang waktu...");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); // Set waktu sesuai waktu compile
  }

  if (!dfPlayer.begin(mySerial)) {
    Serial.println("DFPlayer tidak terdeteksi!");
    while (1);
  }
  
  dfPlayer.volume(20);  // Set volume (0-30)
}

void loop() {
  int sensorValue = digitalRead(sensorPin); // Membaca nilai sensor digital
  bool currentDetection = (sensorValue == HIGH);

  if (currentDetection && !objectDetected) {
    objectDetected = true;
    startTime = millis();
    timer1Active = true;
  }

  if (!currentDetection && objectDetected) {
    objectDetected = false;
    timer1Active = false;
    timer2Active = false;
    timer2Remaining = timer2; // Reset Timer 2 ke 10 menit
  }

  if (timer1Active && millis() - startTime >= timer1) {
    timer1Active = false;
    timer2Active = true;
    startTime = millis(); // Reset waktu untuk Timer 2
  }

  if (timer2Active) {
    timer2Remaining = timer2 - (millis() - startTime);
    if (timer2Remaining <= 0) {
      timer2Active = false;
      timer2Remaining = timer2; // Reset ke 10 menit setelah selesai
    }
  }

  // 🔊 DFPlayer berbunyi pada menit ke-3, 2, dan 1 sebelum timer habis
  if (timer2Remaining == 180000) {
    dfPlayer.play(1); // Putar file nomor 1
  } else if (timer2Remaining == 120000) {
    dfPlayer.play(2); // Putar file nomor 2
  } else if (timer2Remaining == 60000) {
    dfPlayer.play(3); // Putar file nomor 3
  }

  // 🔴 Membaca waktu dari RTC
  DateTime now = rtc.now();
  char currentTimeStr[9];
  sprintf(currentTimeStr, "%02d:%02d:%02d", now.hour(), now.minute(), now.second());

  // 🔵 Hitung waktu 4 jam ke depan
  DateTime targetTime = now + TimeSpan(4, 0, 0, 0);
  char targetTimeStr[9];
  sprintf(targetTimeStr, "%02d:%02d:%02d", targetTime.hour(), targetTime.minute(), targetTime.second());

  // 🔵 Jika tombol 1 atau tombol 2 ditekan, tampilkan waktu sekarang & 4 jam ke depan
  if (digitalRead(button1) == LOW || digitalRead(button2) == LOW) {
    Serial.print("Waktu Sekarang: ");
    Serial.print(currentTimeStr);
    Serial.print(" | Waktu Target: ");
    Serial.println(targetTimeStr);
    delay(500);  // Hindari debounce tombol
  }

  delay(500);
}
