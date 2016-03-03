#include "SinglePlayerScene.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "stdio.h"
#include <math.h>
Scene* SinglePlayerScene::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = SinglePlayerScene::create();


	// add layer as a child to scene
	scene->addChild(layer);
	// return the scene
	return scene;
}

bool SinglePlayerScene::init()
{
	if (!Layer::init())
	{
		return false;
	}

	auto rootNode = CSLoader::createNode("SinglePlayer.csb");
	cog1 = (Sprite*)rootNode->getChildByName("cog");
	cog2 = (Sprite*)rootNode->getChildByName("cog2");
	player = (Sprite*)rootNode->getChildByName("player");
	background = (Sprite*)rootNode->getChildByName("labBackground1");
	background2 = (Sprite*)rootNode->getChildByName("labBackground2");
	
	//score label
	int score = 0;	
	__String* tempScore = __String::createWithFormat("%i", score);
	scoreLabel = (ui::Text*)rootNode->getChildByName("scoreLabel");
	scoreLabel->setText(tempScore->getCString());

	//scrolling tolerance for background
	tol = 0;

	_touched = false;
	_clicked = false;
	_alive = true;
	_cogNumber = 0;
	dirVector =  Vec2(0.0f, 0.0f);
	_closeCog = cog1->getPosition();
	//player->setPosition(449.57, 146.03);
	// Touch Listener
	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = CC_CALLBACK_2(SinglePlayerScene::onTouchBegan, this);
	touchListener->onTouchEnded = CC_CALLBACK_2(SinglePlayerScene::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

	addChild(rootNode);
	this->scheduleUpdate();

	return true;
}

void SinglePlayerScene::update(float)
{
	if (_alive)
	{
		r++;
		cog1->setRotation(r);
		cog2->setRotation(r);
		CheckForClosest();
		

		if (!_touched)
		{
			wallCollide();
			movePlayer();
			ScrollingBackground();
		}
		else
		{
			playerSpin();
		}
		cogCollide();
		resetCog();

		//Display the score
		GameManager::sharedGameManager()->AddToScore(1);
		scoreLabel->setString(StringUtils::format("%d", GameManager::sharedGameManager()->GetScore()));
	}	
}

void SinglePlayerScene::resetCog()
{
	RandomHelper rand = RandomHelper();
	int randomX = rand.random_int(335, 676);
	if (cog1->getPosition().y < 0.0f)
	{
		cog1->setPosition(randomX, 800);
		_clicked = false;
	}

	if (cog2->getPosition().y < 0.0f)
	{
		cog2->setPosition(randomX, 800);
		_clicked = false;
	}
	
}

void SinglePlayerScene::CheckForClosest()
{
	Vec2 play = player->getPosition();
	Vec2 play1 = player->getPosition();
	Vec2 cog = cog1->getPosition();
	Vec2 cog_1 = cog2->getPosition();
	play -= cog;
	play1 -= cog_1;
	if (!_touched)
	{
		if (play.length() > play1.length())
		{
			_cogNumber = 2;
			_closeCog = cog2->getPosition();
		}
		else
		{
			_cogNumber = 1;
			_closeCog = cog1->getPosition();
		}
	}
}

bool SinglePlayerScene::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event) {
	cocos2d::log("touch began");
	if (player->getPosition().x > _closeCog.x)
		_clockwise = false;
	else
		_clockwise = true;



	if(!_clicked && CloseEnough())
		_touched = true;

	_clicked = true;
	return true;
}

void SinglePlayerScene::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event) {

	_touched = false;
	cocos2d::log("touch ended");
}

void SinglePlayerScene::cogCollide()
{
	if (player->getBoundingBox().intersectsRect(cog1->getBoundingBox()))
	{
		_alive = false;
		cocos2d::log("cog collide");
	}
	if (player->getBoundingBox().intersectsRect(cog2->getBoundingBox()))
	{
		_alive = false;
		cocos2d::log("cog collide");
	}
}

void SinglePlayerScene::wallCollide()
{
	Vec2 play = player->getPosition();

	if (play.x < 262 || play.x > 746)
	{
		//MASSIVE EXPLOSION AND HORN SOUNDS
		_alive = false;
		cocos2d::log("wall collide");
	}
}

void SinglePlayerScene::playerSpin()
{
	if (!_clockwise)
	{
		//AntiClockwise Spin
		
		float s = sin(0.1);
		float c = cos(0.1);

		Vec2 point = _closeCog;
		Vec2 playerpos = player->getPosition();

		playerpos -= point;

		float x = playerpos.x * c - playerpos.y * s;
		float y = playerpos.x * s + playerpos.y * c;

		playerpos.x = x + point.x;
		playerpos.y = y + point.y;

		player->setPosition(playerpos);
		player->setRotation(sr -= 5.5);
	}
	else
	{
		//Clockwise Spin
		float s = sin(0.1);
		float c = cos(0.1);

		Vec2 point = _closeCog;
		Vec2 playerpos = player->getPosition();

		playerpos -= point;

		float x = playerpos.x * c + playerpos.y * s;
		float y = -playerpos.x * s + playerpos.y * c;

		playerpos.x = x + point.x;
		playerpos.y = y + point.y;


		player->setPosition(playerpos);
		player->setRotation(sr += 5.5);
	}

}

void SinglePlayerScene::movePlayer()
{
	dirVector.y = (float)cos(CC_DEGREES_TO_RADIANS(sr));
	dirVector.x = (float)sin(CC_DEGREES_TO_RADIANS(sr));
	if (dirVector.length() > 0)
	{
		dirVector.normalize();
	}
	player->setPosition(player->getPosition().x + (dirVector.x * 4), player->getPosition().y); //+// (dirVector.y*4));
	cog1->setPosition(cog1->getPosition().x, cog1->getPosition().y - 4.0f);
	cog2->setPosition(cog2->getPosition().x, cog2->getPosition().y - 4.0f);
	if (player->getPosition().y != 146.0f && player->getPosition().y > 146.0f)
	{
		player->setPosition(player->getPosition().x, player->getPosition().y - 1.0f);
	}
	if (player->getPosition().y != 146.0f && player->getPosition().y < 146.0f)
	{
		player->setPosition(player->getPosition().x, player->getPosition().y + 1.0f);
	}
}

bool SinglePlayerScene::CloseEnough()
{
	Vec2 play = player->getPosition();
	Vec2 cog = _closeCog;
	play -= cog;

	if (play.length() > 400.0f)
	{
		return false;
	}

	return true;
}

static const int scrollSpeed = 4.0f;

void SinglePlayerScene::ScrollingBackground()
{

	//scrolling background
	Vec2 Bg1Pos = background->getPosition();
	Vec2 Bg2Pos = background2->getPosition();

	background->setPosition(Bg1Pos.x, Bg1Pos.y - scrollSpeed);
	background2->setPosition(Bg2Pos.x, Bg2Pos.y - scrollSpeed);

	if (background->getPosition().y < -390 + tol)
	{
		background->setPosition(Bg1Pos.x, 1100);
	}

	if (background2->getPosition().y < -390 + tol)
	{
		background2->setPosition(Bg2Pos.x, 1100);
	}
}


 
