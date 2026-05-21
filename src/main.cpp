#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH,SCREEN_HEIGHT, &Wire, -1);

void setup() {
  // put your setup code here, to run once:
  pinMode(8,INPUT);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  delay(1000);
  display.setTextColor(WHITE,BLACK);
  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextSize(5);
  display.println("Hello");
  display.display();
}

void loop() {
  // put your main code here, to run repeatedly:
}