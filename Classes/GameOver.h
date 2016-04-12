#pragma once
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "cocos2d.h"
#include "SoundManager.h"
#include "Cogs.h"
#include "GameManager.h"
#include "stdio.h"
#include "HelloWorldScene.h"
#include "SinglePlayerScene.h"
USING_NS_CC;

class GameOver : public cocos2d::Layer
{
public:
	GameOver();
	~GameOver();

	static cocos2d::Scene* createScene();
	CREATE_FUNC(GameOver);
	virtual bool init();

	cocos2d::ui::Text* scoreLabel;
	
};

