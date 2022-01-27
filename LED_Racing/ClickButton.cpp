#include "ClickButton.h"

ClickButton::ClickButton() {
}

void ClickButton::init(byte pin){
  pinMode(pin,INPUT_PULLUP);
  _pin = pin;
}

bool ClickButton::isClick(){
  bool isclick = false;
  byte value = digitalRead(_pin);
  if(_lastValue==0 && value==1){
    isclick = true;
  }
  _lastValue = value;
  return isclick;
}
