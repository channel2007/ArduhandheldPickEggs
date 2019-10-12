/***********************************************************************************
*
* 程式說明：遊戲管理器.cpp
*
**********************************************************************************/
#include "GameManager.h"

//----------------------------------------------------------------------------
// 初始.
//----------------------------------------------------------------------------
void GameManager::init(Arduboy2 *adboy2, ArduboyTones *arduboyTones){  
	// 紀錄指標.
	arduboy = adboy2;
	sound = arduboyTones;
  
	// 設定遊戲模式.
	GameMode gameMode = GamePlay;    
	// 遊戲重新開始.
	gameRestart();
}

//----------------------------------------------------------------------------
// 更新.
//----------------------------------------------------------------------------
void GameManager::update() {
	// 遊戲中.
	if(gameMode == GamePlay)
		gamePlay();
	// Game Over.
	else if(gameMode == GameOver)
		gameOver();
}

//----------------------------------------------------------------------------
// 增加蛋.
//----------------------------------------------------------------------------
void GameManager::add() {
	// 亂數蛋型態機率.
	int16_t t = random(100);
	// 亂數出現座標.
	int16_t x = random(10,118);
 
	// 搜尋沒用的蛋結構.
	for (byte i = 0; i < EGG_MAX; i++) {
		if (!eggs[i].open) {		
			// 開啟蛋.
			eggs[i].open = true;
			eggs[i].x = x;
			// 設定蛋型態.
			if (t > 20)
				eggs[i].type = true;	// 實心.
			else
				eggs[i].type = false;	// 空心.
			return;
		}
	}
}

//----------------------------------------------------------------------------
// 遊戲重新開始.
//----------------------------------------------------------------------------
void GameManager::gameRestart(){
  // 初始蛋結構陣列.
  for (byte i = 0; i < EGG_MAX; i++) {
    eggs[i].open = false;
    eggs[i].type = false;   
    eggs[i].collideRect.width = EGG_RADIUS*2;
    eggs[i].collideRect.height = EGG_RADIUS*2;
    eggs[i].x = eggs[i].collideRect.x = 0;
    eggs[i].y = eggs[i].collideRect.y = -8;    
  }

  // 蛋掉落速度.
  eggDropSpeed = 1;
  // 蛋產生時間(以Frame計算)
  eggGeneratorFrame = 30;
  // 生命.
  life = 5;
  // 總分.
  score = 0;

  // 初始籃子.
  basketRect.width = 20;
  basketRect.height = 8;
  basketRect.x = 54;
  basketRect.y = 54;  

  // 設定遊戲中.
  gameMode = GamePlay;
}

//----------------------------------------------------------------------------
// 遊戲中.
//----------------------------------------------------------------------------
void GameManager::gamePlay(){
  // 右移動.
  if(arduboy->pressed(RIGHT_BUTTON) && (basketRect.x < (SCREEN_WIDTH - 20)))
    basketRect.x += 5;
  
  // 左移動.
  if(arduboy->pressed(LEFT_BUTTON) && (basketRect.x > 0))
    basketRect.x -= 5;
  
  // 顯示籃子.
  arduboy->fillRect(basketRect.x, basketRect.y, basketRect.width, basketRect.height);

  // 生蛋.
  if (arduboy->everyXFrames(eggGeneratorFrame))
	gameManager.add();
  
  // 搜尋所有蛋結構.
  for (byte i = 0; i < EGG_MAX; i++) {
    // 只處理開啟使用的蛋.
    if (eggs[i].open) {
      // 往下掉.
      eggs[i].y += eggDropSpeed;
     
      // 判斷蛋型態.
      if (eggs[i].type) {
        // 畫圓(實心).
        arduboy->fillCircle(eggs[i].x, eggs[i].y, EGG_RADIUS);
      }
      else {
        // 畫圓(空心).
        arduboy->drawCircle(eggs[i].x, eggs[i].y, EGG_RADIUS);
      }
      
      // 蛋掉出下界限.
      if (eggs[i].y > SCREEN_HEIGHT) {
        // 關閉蛋.
        eggs[i].open = false;
        eggs[i].y = -8;
        // 掉落實心蛋扣生命.
        if(eggs[i].type){
          // 播放扣生命音效.
          sound->noTone();
          sound->tone(NOTE_C7, 30);          
          // 扣生命.
          life--;
          // Game Over.
          if(life<=0){
			// 播放扣生命音效.
            sound->noTone();
            sound->tone(NOTE_E7, 120, NOTE_C7, 120, NOTE_E7, 120);
            gameMode = GameOver;
          }          
        }
      }
     
      // 更新蛋碰撞區域.
      eggs[i].collideRect.x = eggs[i].x - EGG_RADIUS;
      eggs[i].collideRect.y = eggs[i].y - EGG_RADIUS;      
      //arduboy->drawRect (eggs[i].collideRect.x, eggs[i].collideRect.y, eggs[i].collideRect.width, eggs[i].collideRect.height);  
      
      // 判斷籃子碰蛋.
      if(arduboy->collide (basketRect, eggs[i].collideRect)){
        if(eggs[i].type){
			// 播放加分音效.
			sound->noTone();
			sound->tone(NOTE_B7, 30);
			// 加分.
			score++;
			// 每接2顆蛋就加快生蛋時間.
			if((score%2)==0){
				eggGeneratorFrame--;
			    if(eggGeneratorFrame < 10)
					eggGeneratorFrame = 10;            
			}
			// 每接20顆蛋就增加生蛋下降速度.
			if((score%20)==0)
				eggDropSpeed++;
			// 每接10顆蛋生命就+1.
			if ((score % 10) == 0)
				life++;
        }else{
			// 播放扣分數音效.
			sound->noTone();
			sound->tone(NOTE_F7, 30);          
			// 扣分數
			score--;
			if(score < 0)
				score = 0;
        }                   
        // 關閉蛋.
        eggs[i].open = false;
        eggs[i].y = -8;        
      }      
    }
  }
  // 顯示生命.
  arduboy->setCursor(0, 0);
  arduboy->print("Life:"); 
  arduboy->print(life);
  // 顯示分數.
  arduboy->setCursor(60, 0);
  arduboy->print(score);  
}

//----------------------------------------------------------------------------
// 遊戲結束.
//----------------------------------------------------------------------------
void GameManager::gameOver(){
  // 顯示分數.
  arduboy->setCursor(60, 0);
  arduboy->print(score);  
  // 顯示Game Over.
  arduboy->setCursor(40, 30);
  arduboy->print("Game Over");
  // 按下A、B按鈕重新開始遊戲.
  if(arduboy->pressed(A_BUTTON) || arduboy->pressed(B_BUTTON)) {
    // 遊戲重新開始.
    gameRestart();
  }  
}
GameManager gameManager;
