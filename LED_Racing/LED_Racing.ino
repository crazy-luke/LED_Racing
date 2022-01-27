/*
 * LED_Racing
 *
 *  Created on: 1 27, 2022
 *      Author: crazy-luke
 */
 
#include <Adafruit_NeoPixel.h>
#include "ClickButton.h" 
#include "Raceway.h" 
#include "Car.h"
#include "Config.h"

Adafruit_NeoPixel strip = Adafruit_NeoPixel(LedNum, PIN_LED, NEO_GRB + NEO_KHZ800);
unsigned long kRampTS;  // 斜坡影响间隔
float kF = Friction;
float kRampF = RampF;
float kMuddyF = MuddyF;

Raceway raceway= Raceway(LedNum);       //赛道
ClickButton resetBtn,evnBtn,modeBtn,p1_btn1,p1_btn2,p2_btn1,p2_btn2;

Car car1 = Car(P1_ID,strip.Color(255,0,0)); 
Car car2 = Car(P2_ID,strip.Color(0,255,0));

void setup() {
  Serial.begin(9600);
  strip.begin(); 
  strip.setBrightness(Brightness);
  
  resetBtn.init(PIN_REST);
  evnBtn.init(PIN_EVN);
  modeBtn.init(PIN_MODE);
  p1_btn1.init(PIN_P1_1);
  p1_btn2.init(PIN_P1_2);
  p2_btn1.init(PIN_P2_1);
  p2_btn2.init(PIN_P2_2);
  pinMode(PIN_P1_LED,OUTPUT);
  pinMode(PIN_P2_LED,OUTPUT);
    
  raceway.setRamp(20,10,20,25);
  raceway.setRamp(30,80,90,95); 
  raceway.setMuddy(kMuddyF,45,63); 
  raceway.setMuddy(kMuddyF,100,110); 
  
  for(byte i=0;i<LedNum;i++){
    strip.setPixelColor(i, strip.Color(0,0,0));
  }
  strip.show();   
}

bool isRacing = false;
void loop() {
  if(resetBtn.isClick()) {
    Serial.println("click start");
    startRace();
    isRacing = true;
  }
  if(modeBtn.isClick()){
    switchMode();
  }
  if(evnBtn.isClick()){
    showEvnLed();
  }

  if (!isRacing){
    delay(50);
    return;
  }
  racing();
}


void startRace(){
  Serial.println("startRace");
  
  car1.initState();
  car2.initState();
  for(int i=0;i<LedNum;i++){
    strip.setPixelColor(i, strip.Color(0,0,0));
  }
  strip.show();
  delay(500);
  strip.setPixelColor(12, strip.Color(0,255,0));
  strip.setPixelColor(11, strip.Color(0,255,0));
  strip.show();
  tone(PIN_AUDIO,400);
  delay(1000);
  noTone(PIN_AUDIO);                  
  strip.setPixelColor(12, strip.Color(0,0,0)); 
  strip.setPixelColor(11, strip.Color(0,0,0));
  strip.setPixelColor(10, strip.Color(255,255,0));
  strip.setPixelColor(9, strip.Color(255,255,0));
  strip.show();
  tone(PIN_AUDIO,600);
  delay(1000);
  noTone(PIN_AUDIO);                  
  strip.setPixelColor(9, strip.Color(0,0,0));
  strip.setPixelColor(10, strip.Color(0,0,0));
  strip.setPixelColor(8, strip.Color(255,0,0));
  strip.setPixelColor(7, strip.Color(255,0,0));
  strip.show();
  tone(PIN_AUDIO,1200);
  delay(1000);
  noTone(PIN_AUDIO);        
}


 
//更新车辆状态：速度、位置、圈数
void updateCarState(Car& car, bool acel){
    //更新速度 距离
    float speed = car.getSpeed(); 
    if(acel){
      if(speed<0){  //反向时助力
        speed += ACEL;
      }
      speed += ACEL;
    }

    float rampF = kRampF * raceway.getRampDist(car.getDist());
    if(rampF != 0){
      unsigned long ts = millis();
      //重力影响间隔
      if((speed >= 0 && ts - kRampTS > 25)  || (speed <= 0 && ts - kRampTS > 35)){
        speed -= rampF;
        kRampTS = ts;
      }
    }

    float muddyF = raceway.getMuddyDist(car.getDist());
    if(speed>0){
      speed -= muddyF;
    }

    if(speed<0 ){  //反向需要更大阻力
      speed -= speed * kF * 3;
    }else{
      speed -= speed * kF;
    }
    
    float dist = car.getDist() + speed;
    byte loop = car.getLoop();
    byte newloop = dist > LedNum * loop ? loop+1 : loop;
    car.updateState(speed, dist, newloop);
}

unsigned long newLoopBeepTS=0; // 音效持续间隔
void checkNewLoop(byte oldLoop, byte newLoop){
    //新的一圈
  if (newLoop > oldLoop) {
    Serial.print(" oldLoop=");Serial.print(oldLoop);
    Serial.print(" newLoop=");Serial.print(newLoop);
    Serial.println(" new loop!!");
    newLoopBeepTS = millis();
    tone(PIN_AUDIO,600);    //start beep
  }
  if(newLoopBeepTS>0){
    if(millis() - newLoopBeepTS>100){
      noTone(PIN_AUDIO);  //stop beep 
      newLoopBeepTS = 0;
    }
  }
}

//显示赛车
byte draworder=0;
void drawCar(){
  if ((millis() & 512) == (512*draworder)) {
    if (draworder==0) {
      draworder=1;
    } else {
      draworder=0;
    }   
  }
  if (draworder==0) {
    car1.draw(strip);
    car2.draw(strip);
  }else {
    car2.draw(strip);
    car1.draw(strip);
  }         
}

// 谁赢了
void checkWinner(){
  if (car1.getLoop()>=LoopMax) {
    isRacing = false;
    winnerEffect(car1);
  } else if (car2.getLoop()>=LoopMax) {
    isRacing = false;
    winnerEffect(car2);
  }
}

void winnerEffect(Car& car){
  bool oddEven = false;
  int msize = sizeof(WinMusic) / sizeof(int);
  for (int note = 0; note < msize; note++) {
    for(int i=0;i<LedNum;i++){
      strip.setPixelColor(i, i%2 == oddEven ? car.getColor() : strip.Color(0,0,0));
    }
    oddEven = !oddEven;
    strip.show();
    tone(PIN_AUDIO, WinMusic[note],200);
    delay(230);
    noTone(PIN_AUDIO);
  }   
  playMusicPCM(car.getId());
  
  for(byte i=0;i<LedNum;i++) {
    strip.setPixelColor(i, strip.Color(0,0,0));
  }
  strip.show(); 
}

//显示环境
bool evnLedON = false;
void showEvnLed(){
  evnLedON = !evnLedON;
  for(byte i=0;i<LedNum;i++){
    if(evnLedON){
      int rampF = raceway.getRampPos(i);
      float muddyM = raceway.getMuddyPos(i);
      if(rampF!=0){
        strip.setPixelColor(i, strip.Color(31,8,0));
        Serial.print("ramp pos =");Serial.print(i);
        Serial.print(" value = ");Serial.println(rampF);
      }else if(muddyM!=0){
        strip.setPixelColor(i, strip.Color(0,8,31));
        Serial.print("muddy pos =");Serial.print(i);
        Serial.print(" value = ");Serial.println(muddyM);       
      }else {
        strip.setPixelColor(i, strip.Color(0,0,0));
      }
    }else{
      strip.setPixelColor(i, strip.Color(0,0,0));
    }
  }
  strip.show(); 
}

bool hardMode = false; //难度
void switchMode(){
  hardMode = !hardMode;
  if(hardMode){
    kF = Friction * HardModeRatio;
    kRampF = RampF* HardModeRatio;
    kMuddyF = MuddyF* HardModeRatio;
  }else{
    kF = Friction;
    kRampF = RampF;
    kMuddyF = MuddyF;      
  }

  // 提示： 紫色 + 2次蜂鸣为高难度
  uint32_t color =  hardMode ? strip.Color(246,14,227) : strip.Color(14,246,227);
  for(int i=0;i<LedNum;i++){
    strip.setPixelColor(i, i%2 == 1 ? color : strip.Color(0,0,0));
  }  
  strip.show();  
  if(hardMode){
    tone(PIN_AUDIO,100);
    delay(500);
    noTone(PIN_AUDIO);  
    delay(500); 
    tone(PIN_AUDIO,100);
    delay(500);
    noTone(PIN_AUDIO); 
    delay(500);
  }else{
    tone(PIN_AUDIO,600);
    delay(500);
    noTone(PIN_AUDIO);   
    delay(1500);     
  }

  for(int i=0;i<LedNum;i++){
    strip.setPixelColor(i, strip.Color(0,0,0));
  }  
  strip.show();  
}
void racing(){
  for(int i=0;i<LedNum;i++){
    strip.setPixelColor(i, strip.Color(0,0,0));
  }  

  byte oldloop1 = car1.getLoop();
  bool isClick = p1_btn1.isClick() || p1_btn2.isClick();
  if(isClick){
    digitalWrite(PIN_P1_LED,HIGH);
  }
  updateCarState(car1,isClick);
  checkNewLoop(oldloop1,car1.getLoop()); 
  byte oldloop2 = car2.getLoop();
  bool isClick2 = p2_btn1.isClick() || p2_btn2.isClick();
  if(isClick2){
    digitalWrite(PIN_P2_LED,HIGH);
  } 
  updateCarState(car2,isClick2);
  checkNewLoop(oldloop2,car2.getLoop());
  drawCar();
  checkWinner();
  strip.show();
  delay(10);

  digitalWrite(PIN_P1_LED,LOW);
  digitalWrite(PIN_P2_LED,LOW);
}
