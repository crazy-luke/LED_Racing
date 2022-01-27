#include "Car.h"

Car::Car(byte id,uint32_t color) {
  _id = id;
  _color = color;
}

void Car::initState(){
  _speed = 0.0;
  _dist = 0.0;
  _loop = 0.0;
}

uint32_t Car::getColor() {
  return _color;
}

byte Car::getId(){
  return _id;
}

void Car::draw(Adafruit_NeoPixel& strip) {
  for(int i=0; i<= 2; i++){ 
   strip.setPixelColor(((word)_dist % strip.numPixels())+i, _color);
  }                   
}

float Car::getDist(){
  return _dist;
}
float Car::getSpeed(){
  return _speed;
}

byte Car::getLoop(){
  return _loop;
}

void Car::updateState(float speed, float dist, byte loop){
  _speed = speed;
  _dist = dist;
  _loop = loop;
}
