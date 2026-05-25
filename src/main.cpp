#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SoftwareReset.hpp>


#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH,SCREEN_HEIGHT, &Wire, -1);

int buttonPin = 8;
int rot = 0;
int level[] = {0,0,0,0,0,0,0,0,0,1,0,0,0,2,1,0,0,0,0,0,1,0,0,0,0,0,0,1,1,0,0,0,3,1,1,1,0,0,0,0,1,0,0,0,0,0,1,0,0,0,2,1,1,0,0,0}; //1=spike 2=pad 3=orb
int levelLength = 56;
bool jumping = false;
bool directionUp = false;
int height = 0;
int bigOffset = 0;
int littleOffset = 0;
bool playing = true;
bool win = false;
bool orb = false;
int jumpHeight = 22;

void setup() {
  pinMode(buttonPin,INPUT);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  delay(1000);
  display.clearDisplay();
}

void loop() {
  if (playing){
  delay(100);
  display.clearDisplay();

  littleOffset=littleOffset+8; //4
  if(littleOffset==16){
    littleOffset=0;
    bigOffset++;
  }

  //draw & check jump pads
  for(int i=0; i<8; i++) {
    if(level[i+bigOffset] == 2){
      display.drawRoundRect(i*16-littleOffset,54,16,6,2,WHITE);
    }
  }
  for(int x=16;x<32;x++){
    for(int y=44-height;y<60-height;y++){
      if(display.getPixel(x,y)==true){
        jumping = true;
        directionUp = true;
        height+=height+4;
        jumpHeight=26;
      }
    }
  }

  //draw & check spikes
  for(int i=0; i<8; i++) {
    if(level[i+bigOffset] == 1){
      display.drawTriangle(i*16-littleOffset+3,60,i*16+8-littleOffset,48,i*16+13-littleOffset,60,WHITE);
    }
  }
  for(int x=16;x<32;x++){
    for(int y=44-height;y<60-height;y++){
      if(display.getPixel(x,y)==true){
        playing=false;
      }
    }
  }

  //draw & check orbs
  for(int i=0; i<8; i++) {
    if(level[i+bigOffset] == 3){
      display.drawCircle(i*16-littleOffset+8,60-24,6,WHITE);
    }  
  }
  orb = false;
  for(int x=16;x<32;x++){
    for(int y=44-height;y<60-height;y++){
      if(display.getPixel(x,y)==true){
        orb=true;
        jumpHeight=30;
      }
    }
  }

  //jump
  if (digitalRead(buttonPin) == HIGH && ((!jumping) || orb)) {
    jumping=true;
    directionUp = true;
    height++;
  }

  if (jumping){
    rot = rot+22;
    if(height>=jumpHeight && directionUp){
      directionUp = false;
    }

    if (directionUp){
      height = height+5;
    } else{
      height = height-4;
    }
  }
  if(height<=1 && !directionUp) {
      jumping = false;
      height=0;
      rot = 0;
      jumpHeight = 22;
  }

  //draw cube and ground
  display.drawFastHLine(0,60,128,WHITE);
  display.drawRotatedRect(24,51-height,16,16,rot,WHITE);

  //draw progress bar
  display.drawRect(16, 0, 96, 8, WHITE);
  display.fillRect(18,2,constrain(float(bigOffset)/float(levelLength)*92,0,92),4,WHITE);

  //complete level
  if (bigOffset>=levelLength){
    win=true;
    playing=false;
  }

  display.display();
  }
  else if(win){
    //win
    delay(1000);
    display.clearDisplay();
    display.setCursor(0,17);
    display.setTextSize(3);
    display.setTextColor(WHITE);
    display.setTextWrap(false);
    display.println("You\nWin");
    display.display();
    display.startscrollleft(0x00,0x0F);
    while(digitalRead(buttonPin) != HIGH){;}
    softwareReset::standard();
  }else{
    //lose
    delay(1000);
    display.clearDisplay();
    display.setCursor(0,17);
    display.setTextSize(3);
    display.setTextColor(WHITE);
    display.setTextWrap(false);
    display.println("Game\nOver");
    display.display();
    display.startscrollleft(0x00,0x0F);
    while(digitalRead(buttonPin) != HIGH){;}
    softwareReset::standard();
  }
}