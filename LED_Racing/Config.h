/*
 * LED_Racing
 *
 *  Created on: 1 27, 2022
 *      Author: crazy-luke
 */
#pragma once

#define PIN_LED       A1  //灯带信号
#define PIN_AUDIO     11  //喇叭
#define PIN_MODE      4   //难度切换按钮
#define PIN_REST      5   //复位按钮
#define PIN_EVN       6   //环境显示按钮
#define PIN_P1_LED    3   //P1 按钮提示灯
#define PIN_P2_LED    2   //P2 按钮提示灯
#define PIN_P1_1      7   //P1 按钮1
#define PIN_P1_2      8   //P1 按钮2
#define PIN_P2_1      9  //P2 按钮1
#define PIN_P2_2      10  //P2 按钮2

#define USE_PCM 0     //PCM 内容比较占空间，不是必须的，1 启用， 0 不启用

const byte LedNum = 180;    //LED 数量  
const byte Brightness= 180; //灯亮度

const float ACEL=0.2;       //加速
const float Friction=0.020; //摩擦力系数
const float RampF=0.003;    //斜坡阻力系数
const float MuddyF = 0.03;  //泥泞路阻力系数
const float HardModeRatio = 1.5;  //困难模式系数，根据需要可以调大

const byte LoopMax=4;       //比赛圈数

const int WinMusic[] = {  //胜利音效
  2600, 2600, 0, 2600,  0, 2100, 2600, 0, 3100
};

const byte P1_ID = 1;   //选手ID
const byte P2_ID = 2;   




#if USE_PCM
  #include "MusicPCM.h"
#endif
playMusicPCM(byte player_id){
#if USE_PCM
    if(player_id == P1_ID){
      playRedWinMusic();
    }else{
      playGreenWinMusic();
    }
#endif    
}



  
