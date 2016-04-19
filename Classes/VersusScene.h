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

class VersusScene : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();
	virtual bool init();
	CREATE_FUNC(VersusScene);

private:
	void update(float)override;
	virtual bool onTouchBegan(cocos2d::Touch*, cocos2d::Event*);
	virtual void onTouchEnded(cocos2d::Touch*, cocos2d::Event*);

	//Player1 Methods
	void player1ResetCogY(int cogNum);
	void player1ResetCogX();
	void player1CheckForClosest();
	void player1ScrollingBackground();
	void player1CogCollide();
	void player1WallCollide();
	void player1PlayerSpin();
	void player1MovePlayer();
	bool player1CloseEnough();
	void player1SetRobotRotation();

	//Player2 Methods
	void player2ResetCog(int cogNum);
	void player2ResetCogX();
	void player2CheckForClosest();
	void player2ScrollingBackground();
	void player2CogCollide();
	void player2WallCollide();
	void player2PlayerSpin();
	void player2MovePlayer();
	bool player2CloseEnough();
	void player2SetRobotRotation();

private:
	//Player1
	cocos2d::Sprite* background;
	cocos2d::Sprite* background2;
	cocos2d::Sprite* cog1;
	cocos2d::Sprite* cog2;
	cocos2d::Sprite* player1;

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
	//Player2
	cocos2d::Sprite* background3;
	cocos2d::Sprite* background4;
	cocos2d::Sprite* cog3;
	cocos2d::Sprite* cog4;
	cocos2d::Sprite* player2;

	Vec2 player2dirVector;
	Vec2 _player2closeCog;
	bool _player2touched;
	bool _player2clockwise;
	bool _player2clicked;
	bool player2flipped;
	bool _player2alive;
	int _player2cogNumber;
	float _player2scrollSpeed;
};
