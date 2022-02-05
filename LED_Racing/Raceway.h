/*
 * LED_Racing
 *
 *  Created on: 1 27, 2022
 *      Author: crazy-luke
 */
#pragma once

#include <Arduino.h> 

class Raceway {
  
  public:
    Raceway(byte ledNum);
    ~Raceway();
    //陡坡
    void setRamp(byte height,byte startPos,byte peakPos,byte endPos);
    int getRampDist(float dist);
    int getRampPos(byte ledPos);  
      
    //泥泞路        
    void setMuddy(float friction,byte startPos,byte endPos);       
    float getMuddyDist(float dist);
    float getMuddyPos(byte ledPos);
    
  private:
    byte* _rampArray;
    float* _muddyArray;
    byte _ledNum = 0;
};
