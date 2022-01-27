#include "Raceway.h"

Raceway::Raceway(byte ledNum) {
  _ledNum = ledNum;
  _rampArray = new byte[ledNum];
  _muddyArray = new float[ledNum];
  
  for(int i=0;i<ledNum;i++){
    _rampArray[i]=127;  //平地参考高度
    _muddyArray[i]=0.0;
  }
}

void Raceway::setRamp(byte height,byte startPos,byte peakPos,byte endPos)
{
  int len = peakPos-startPos;
  float stepRamp = (float)height/len;
  for(int i=0;i<len;i++) {
    _rampArray[startPos+i]= 127 -i * stepRamp;
  }
  
  _rampArray[peakPos] = 127; 
  
  len= endPos - peakPos;
  stepRamp = (float)height/len;
  for(int i=0;i<len;i++){
    _rampArray[peakPos+i+1]=127+ height-i*stepRamp;
  }
}

int Raceway::getRampPos(byte ledPos){
  return 127 - _rampArray[ledPos];
}

int Raceway::getRampDist(float dist) {
  byte ledPos = (word)dist % _ledNum;
  return getRampPos(ledPos);
}

void Raceway::setMuddy(float friction,byte startPos,byte endPos)
{                          
  int len = endPos-startPos;
  for(int i=0;i<=len;i++) {
    _muddyArray[startPos+i]= friction;
  }
}

float Raceway::getMuddyPos(byte ledPos){
  return _muddyArray[ledPos];
}

float Raceway::getMuddyDist(float dist) {
  byte ledPos = (word)dist % _ledNum;
  return getMuddyPos(ledPos);
}
