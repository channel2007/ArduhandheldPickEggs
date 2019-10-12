/*****************************************************************************
*
* 程式說明：接雞蛋.
*
*****************************************************************************/
#include <Arduboy2.h>
#include <ArduboyTones.h>
#include "GameManager.h"

// 宣告arduboy實例.
Arduboy2 arduboy;
// 宣告ArduboyTones實例.
ArduboyTones sound(arduboy.audio.enabled);

//----------------------------------------------------------------------------
// 初始.
//----------------------------------------------------------------------------
void setup()
{
	// 啟動arduboy實例.
	arduboy.begin();
	// 設定每秒15 FPS更新率.
	arduboy.setFrameRate(30);
	// 初始蛋管理器.
	gameManager.init(&arduboy, &sound);
}

//----------------------------------------------------------------------------
// 主迴圈.
//----------------------------------------------------------------------------
void loop()
{
	// 等待下一帧時間到才進入繼續執行.
	if (!(arduboy.nextFrame()))
		return;
	// 將畫面清為黑色.
	arduboy.clear();
	// 更新蛋管理器.
	gameManager.update();
	// 更新畫面.
	arduboy.display();
}