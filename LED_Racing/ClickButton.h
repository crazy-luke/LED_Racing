/*
 * LED_Racing
 *
 *  Created on: 1 27, 2022
 *      Author: crazy-luke
 */
#pragma once

#include <Arduino.h> 

class ClickButton {

  public:
    ClickButton();
    bool isClick();
    void init(byte pin);
    
  private:
    byte _pin;
    byte _lastValue = 1;
};
