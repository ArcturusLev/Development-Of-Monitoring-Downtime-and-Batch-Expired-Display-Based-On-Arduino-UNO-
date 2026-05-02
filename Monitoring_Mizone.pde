import processing.serial.*;

Serial myPort;
String timer1Data = "00:00";
String timer2Data = "10:00";
String realTimeTarget1 = "00:00:00";
String realTimeTarget2 = "00:00:00";
String dfPlayerstatus = " - ";

PFont titleFont, timeFont, realTimeFont, smallFont;

void setup() {
  fullScreen(); 
  printArray(Serial.list()); 
  myPort = new Serial(this, "COM3", 9600); 
  myPort.bufferUntil('\n'); 

  titleFont = createFont("DIN Engschrift Std", 100);
  timeFont = createFont("DIN Engschrift Std", 300);
  realTimeFont = createFont("DIN Engschrift Std", 50);
  smallFont = createFont("DIN Engschrift Std", 40);
}

void draw() {
  background(0); 

  // Judul utama
  fill(200);
  textFont(titleFont);
  textAlign(CENTER, CENTER);
  text("TIMER LOSSES MIZONE", width / 2, height / 8);

  // Countdown Timer 2 (besar)
  fill(100, 255, 100);
  textFont(timeFont);
  text(timer2Data, width / 2, height / 2 - 50);

  // Timer 1 (lebih kecil di bawah Timer 2)
  fill(255);
  textFont(smallFont);
  text("Timer 1: " + timer1Data, width / 2, height / 2 + 180);
 

  // Info Box untuk waktu tombol 1 dan tombol 2
  drawInfoBox();
}
void simulateData() {
  String fakeData = "01:23|09:45|12:34:56|16:45:00|ALERT";
  String[] dataParts = split(fakeData, "|");

  if (dataParts.length == 5) {
    timer1Data = dataParts[0];
    timer2Data = dataParts[1];
    realTimeTarget1 = dataParts[2];
    realTimeTarget2 = dataParts[3];
    dfPlayerstatus = dataParts[4];
  }
}

void drawInfoBox() {
  int boxWidth = 500;
  int boxHeight = 200;
  int boxX = width - boxWidth - 50;
  int boxY = height - boxHeight - 300;

  fill(50, 50, 50, 200);
  rect(boxX, boxY, boxWidth, boxHeight, 20);

  fill(255, 24, 26);
  textFont(realTimeFont);
  textAlign(CENTER, CENTER);
  text("INFO EXPIRED BLENDING", boxX + boxWidth / 2, boxY + 30);

  fill(255);
  textFont(realTimeFont);
  textAlign(LEFT, CENTER);
  text("Tanki A: " + realTimeTarget1, boxX + 20, boxY + 90);
  text("Tanki B: " + realTimeTarget2, boxX + 20, boxY + 150);
  
  if (dfPlayerstatus.equals("ALERT")){
    fill(255, 255, 0);
    textFont(realTimeFont);
    textAlign(LEFT, TOP);
    text("DFPlayer Sudah Dinyalakan", boxX + 20, boxY + boxHeight + 20);
  }
}

void serialEvent(Serial myPort) {
  String receivedData = myPort.readStringUntil('\n');
  if (receivedData != null) {
    receivedData = trim(receivedData);
    String[] dataParts = split(receivedData, "|");

    if (dataParts.length == 4) {
      timer1Data = dataParts[0];
      timer2Data = dataParts[1];
      realTimeTarget1 = dataParts[2];
      realTimeTarget2 = dataParts[3];
    }
  }
}
