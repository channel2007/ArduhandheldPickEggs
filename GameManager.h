/***********************************************************************************
*
* 程式說明：遊戲管理器.h
*
**********************************************************************************/
#ifndef _EGG_MANAGER_h
#define _EGG_MANAGER_h

#include <Arduboy2.h>
#include <ArduboyTones.h>
#include "arduino.h"

#define SCREEN_WIDTH  128     // 螢幕寬.
#define SCREEN_HEIGHT  64     // 螢幕高.

#define EGG_MAX       10      // 蛋數量.
#define EGG_RADIUS    4       // 蛋半徑.

// 蛋結構.
struct Egg {
  bool open;        // 是否使用.
  bool type;        // 型態(true:實心蛋 false:空心蛋);
  Rect collideRect; // 碰撞區域.
  int16_t  x;       // x.
  int16_t  y;       // y.
};

// 宣告列舉.
enum GameMode{ GamePlay, GameOver};

class GameManager
{
 public:
   // 初始.
  void init(Arduboy2 *adboy2, ArduboyTones *arduboyTones);
  // 更新.
  void update();

private:
  // 蛋掉落速度.
  byte eggDropSpeed = 1;
  // 蛋產生時間(以Frame計算)
  byte eggGeneratorFrame = 30;
  
  // Arduboy2指標.
  Arduboy2 *arduboy;
  // ArduboyTones指標.
  ArduboyTones *sound;
  
  // 蛋結構陣列.
  Egg eggs[EGG_MAX];
  // 籃子.
  Rect basketRect;

  // 設定遊戲模式.
  GameMode gameMode = GamePlay;

  // 生命.
  int16_t life = 5;
  // 總分.
  int16_t score = 0;
 
  // 增加蛋.
  void add();
  // 遊戲重新開始.
  void gameRestart();
  // 遊戲中.
  void gamePlay();
  // 遊戲結束.
  void gameOver();
};
extern GameManager gameManager;

#endif
