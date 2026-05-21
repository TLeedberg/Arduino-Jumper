#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH,SCREEN_HEIGHT, &Wire, -1);

int buttonPin = 8;
int rot = 0;
int level[8] = {0,0,0,0,0,1,0,0};
bool jumping = false;
bool directionUp = false;
int height = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(buttonPin,INPUT);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  delay(1000);
  display.clearDisplay();
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(100);
  if (digitalRead(buttonPin) == HIGH && !jumping) {
    jumping=true;
    directionUp = true;
    height++;
  }

  if (jumping){
    if(height>=20 && directionUp){
      directionUp = false;
    }

    if (directionUp){
      height = height+5;
    } else{
      height = height-5;
    }
  }
  if(height<=0) {
      jumping = false;
      height=0;
  }

  display.clearDisplay();
  display.drawFastHLine(0,60,128,WHITE);

  display.drawRotatedRect(24,52-height,16,16,rot,WHITE);
  for(int i=0; i<8; i++) {
    if(level[i] == 1){
      display.drawTriangle(i*16,60,i*16+8,44,i*16+16,60,WHITE);
    }
  }
  display.display();
  Serial.println(height);
}