/*
 * LED_Racing
 *
 *  Created on: 1 27, 2022
 *      Author: crazy-luke
 */
#pragma once

#include <Arduino.h> 
#include <Adafruit_NeoPixel.h>

class Car {

  public:
    Car(byte id, uint32_t color);
    void initState();
    void updateState(float speed, float dist,byte loop);
    void draw(Adafruit_NeoPixel& strip);
    
    byte getId();
    uint32_t getColor();
    float getSpeed();
    float getDist();
    byte getLoop();

  private:
    int _id;
    uint32_t _color;
    byte _loop;     //跑圈数
    float _speed=0; //速度
    float _dist=0;  //距离

};
