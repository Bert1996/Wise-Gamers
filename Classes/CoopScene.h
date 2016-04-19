#pragma once
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "cocos2d.h"
#include "SoundManager.h"
#include "Cogs.h"
#include "GameManager.h"
#include "stdio.h"
#include "GameOver.h"
#include "cocos2d.h"

class CoopScene : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();
	virtual bool init();
	CREATE_FUNC(CoopScene);

	void update(float)override;
	virtual bool onTouchBegan(cocos2d::Touch*, cocos2d::Event*);
	virtual void onTouchEnded(cocos2d::Touch*, cocos2d::Event*);

	//Player1 Methods
	void player1ResetCog(int cogNum);
	void player1CheckForClosest();
	void player1ScrollingBackground();
	void player1CogCollide();
	void player1WallCollide();
	void player1PlayerSpin();
	void player1MovePlayer();
	bool player1CloseEnough();
	void player1SetRobotRotation();

private:
	//Player1
	cocos2d::Sprite* background;
	cocos2d::Sprite* background2;
	cocos2d::Sprite* cog1;
	cocos2d::Sprite* cog2;
	cocos2d::Sprite* player1;
	cocos2d::ui::Text* scoreLabel;

	Vec2 player1dirVector;
	Vec2 _player1closeCog;
	bool _player1touched;
	bool _player1clockwise;
	bool _player1clicked;
	bool player1flipped;
	bool _player1alive;
	int _player1cogNumber;
	float _player1scrollSpeed;
	float r = 0;
	float cogWdith;
	cocos2d::Sprite* cog3;
};
