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

	//Sprite Inits
	cog1 = (Sprite*)rootNode->getChildByName("cog");
	cog2 = (Sprite*)rootNode->getChildByName("cog2");
	player = (Sprite*)rootNode->getChildByName("player");
	background = (Sprite*)rootNode->getChildByName("labBackground1");
	background2 = (Sprite*)rootNode->getChildByName("labBackground2");
	help = (Sprite*)rootNode->getChildByName("Image_1");

	//score label
	int score = 0;
	_scrollSpeed = 5.0f;
	__String* tempScore = __String::createWithFormat("%i", score);
	scoreLabel = (ui::Text*)rootNode->getChildByName("scoreLabel");
	scoreLabel->setText(tempScore->getCString());

	//Varable Inits
	_touched = false;
	_clicked = false;
	_alive = true;
	flipped = false;
	_cogNumber = 0;
	dirVector = Vec2(0.0f, 0.0f);
	_closeCog = cog1->getPosition();
	cogWdith = cog1->getContentSize().height / 4;
	// Touch Listener
	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = CC_CALLBACK_2(SinglePlayerScene::onTouchBegan, this);
	touchListener->onTouchEnded = CC_CALLBACK_2(SinglePlayerScene::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

	resetCogX();

	addChild(rootNode);
	this->scheduleUpdate();

	
	if (!cocos2d::UserDefault::getInstance()->getBoolForKey("tut"))
	{
		cocos2d::UserDefault::getInstance()->setBoolForKey("tut", true);
		_tutorial = false;
	}
	else
	{
		_tutorial = true;
		help->setOpacity(0);
	}
	
	return true;
}

void SinglePlayerScene::update(float)
{
	if (_tutorial)
	{
		if (_alive)
		{
			r++;
			cog1->setRotation(r);
			cog2->setRotation(r);
			CheckForClosest();
			cogCollide();
			RangeIndicator();
			Speed();

			if (!_touched)
			{
				wallCollide();
				movePlayer();
				ScrollingBackground();
				//Display the score
				GameManager::sharedGameManager()->AddToScore(1);
				scoreLabel->setString(StringUtils::format("%d", GameManager::sharedGameManager()->GetScore()));
			}
			else
			{
				playerSpin();
			}

			if (cog1->getPosition().y < 0.0f)
			{
				resetCog(1);
			}
			if (cog2->getPosition().y < 0.0f)
			{
				resetCog(2);
			}

		}
		else
		{
			auto mainScene = GameOver::createScene();
			CCDirector::getInstance()->replaceScene(mainScene);
		}
	}

}

void SinglePlayerScene::resetCog(int cogNum)
{
	auto size = Director::getInstance()->getVisibleSize();
	int leftWall = ((size.width / 100) * 30);
	int rightWall = ((size.width / 100) * 68);

	RandomHelper rand = RandomHelper();
	int randomX = rand.random_int(leftWall, rightWall);

	if (cogNum == 1)
	{
		cog1->setPosition(randomX, size.height);
		_clicked = false;
	}

	else
	{
		cog2->setPosition(randomX, size.height);
		_clicked = false;
	}
}

void SinglePlayerScene::resetCogX()
{
	auto size = Director::getInstance()->getVisibleSize();
	int leftWall = ((size.width / 100) * 30);
	int rightWall = ((size.width / 100) * 68);

	RandomHelper rand = RandomHelper();
	int randomX = rand.random_int(leftWall, rightWall);

	cog1->setPosition(randomX, cog1->getPositionY());
	randomX = rand.random_int(leftWall, rightWall);
	cog2->setPosition(randomX, cog2->getPositionY());
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
	if (_tutorial)
	{
		if (player->getPosition().x > _closeCog.x)
		{
			_clockwise = false;
		}
		else
		{
			_clockwise = true;

		}
		if (!_clicked && CloseEnough())
			_touched = true;

		_clicked = true;
	}
	else
	{
		_tutorial = cocos2d::UserDefault::getInstance()->getBoolForKey("tut");
		help->setOpacity(0);
	}
	return true;
}

void SinglePlayerScene::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event) {

	_touched = false;
	cocos2d::log("touch ended");
}

void SinglePlayerScene::cogCollide()
{
	Vec2 pBox = player->getPosition();
	
	if (pBox.x > _closeCog.x - cogWdith && pBox.x < _closeCog.x + cogWdith && pBox.y > _closeCog.y - cogWdith && pBox.y < _closeCog.y + cogWdith)
	{
		_alive = false;
	}

	
}

void SinglePlayerScene::wallCollide()
{
	auto size = Director::getInstance()->getVisibleSize(); //27 ! 72
	float leftWall = ((size.width / 100) * 27);
	float rightWall = ((size.width / 100) * 72);

	Vec2 play = player->getPosition();

	if (play.x < leftWall || play.x > rightWall)
	{
		//MASSIVE EXPLOSION AND HORN SOUNDS
		_alive = false;
		cocos2d::log("wall collide");
	}
}

void SinglePlayerScene::playerSpin()
{
	float s = sin(0.1);
	float c = cos(0.1);

	Vec2 point = _closeCog;
	Vec2 Player = player->getPosition();
	Vec2 playerpos = Player;

	playerpos -= point;

	if (!_clockwise)
	{
		player->setFlipY(true);
		flipped = true;
		//AntiClockwise Spin
		float x = playerpos.x * c - playerpos.y * s;
		float y = playerpos.x * s + playerpos.y * c;

		playerpos.x = x + point.x;
		playerpos.y = y + point.y;

		//Moves player closer
		/*if (Player.y >= point.y - 4.0f && Player.y <= point.y + 4.0f && Player.x > point.x)
		{
		playerpos.x -= 20.0f;
		}*/

		player->setPosition(playerpos);
		SetRobotRotation();

	}
	else
	{
		//Clockwise Spin
		player->setFlipY(false);
		flipped = false;

		float x = playerpos.x * c + playerpos.y * s;
		float y = -playerpos.x * s + playerpos.y * c;

		playerpos.x = x + point.x;
		playerpos.y = y + point.y;

		//Moves player closer
		/*if (Player.y >= point.y - 4.0f && Player.y <= point.y + 4.0f && Player.x > point.x)
		{
		playerpos.x -= 20.0f;
		}*/

		player->setPosition(playerpos);
		SetRobotRotation();
	}
}

void SinglePlayerScene::movePlayer()
{
	auto size = Director::getInstance()->getVisibleSize();
	float moveY = ((size.height / 100) * 19);

	//Player Movement
	//x
	float rot = cos(CC_DEGREES_TO_RADIANS(player->getRotation() - 90));

	if (dirVector.length() > 0)
	{
		dirVector.normalize();
	}

	if (flipped)
		player->setPosition(player->getPosition().x + -(rot * 4), player->getPosition().y);

	if (!flipped)
		player->setPosition(player->getPosition().x + (rot * 4), player->getPosition().y);

	//y
	if (player->getPosition().y != moveY && player->getPosition().y > moveY) //19% of size.y
	{
		player->setPosition(player->getPosition().x, player->getPosition().y - 2.0f);
	}
	if (player->getPosition().y != moveY && player->getPosition().y < moveY)
	{
		player->setPosition(player->getPosition().x, player->getPosition().y + 2.0f);
	}

	//Cog Movement
	cog1->setPosition(cog1->getPosition().x, cog1->getPosition().y - _scrollSpeed);
	cog2->setPosition(cog2->getPosition().x, cog2->getPosition().y - _scrollSpeed);
}

bool SinglePlayerScene::CloseEnough()
{
	Vec2 play = player->getPosition();
	Vec2 cog = _closeCog;
	play -= cog;

	if (play.length() > 300.0f)
	{
		return false;
	}
	return true;
}

void SinglePlayerScene::ScrollingBackground()
{
	auto size = Director::getInstance()->getVisibleSize();

	//scrolling background
	Vec2 Bg1Pos = background->getPosition();
	Vec2 Bg2Pos = background2->getPosition();

	background->setPosition(Bg1Pos.x, Bg1Pos.y - 5.0f);
	background2->setPosition(Bg2Pos.x, Bg2Pos.y - 5.0f);

	if (background2->getPosition().y == size.height)
	{
		background->setPosition(Bg1Pos.x, size.height * 2);
	}
	if (background->getPosition().y == size.height)
	{
		background2->setPosition(Bg2Pos.x, size.height * 2);
	}
}

void SinglePlayerScene::SetRobotRotation()
{
	Vec2 cog = _closeCog;
	Vec2 play = player->getPosition();

	float angle = atan2(cog.y - play.y, cog.x - play.x);
	angle = CC_RADIANS_TO_DEGREES(angle);

	player->setRotation(-angle);
}

void SinglePlayerScene::InRange()
{
	//decides cog colour
	if (_cogNumber == 1)
	{
		cog1->setColor(cocos2d::Color3B::GREEN);
	}
	else
	{
		cog1->setColor(cocos2d::Color3B::GRAY);
	}
	if (_cogNumber == 2)
	{
		cog2->setColor(cocos2d::Color3B::GREEN);
	}
	else
	{
		cog2->setColor(cocos2d::Color3B::GRAY);
	}
}


void SinglePlayerScene::RangeIndicator()
{
	Vec2 play = player->getPosition();
	Vec2 cog = _closeCog;
	play -= cog;

	InRange();
}

void SinglePlayerScene::Speed()
{
	float score = GameManager::sharedGameManager()->GetScore();

	if ((int)score % 500 == 0)
	{
		_scrollSpeed += 2.5f;
	}
}
