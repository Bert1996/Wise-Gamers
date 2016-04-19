#include "CoopScene.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

using namespace cocostudio::timeline;
Scene* CoopScene::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = CoopScene::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

bool CoopScene::init()
{
	if (!Layer::init())
	{
		return false;
	}

	auto rootNode = CSLoader::createNode("Coop.csb");

	//Sprite Inits for Player1
	cog1 = (Sprite*)rootNode->getChildByName("cog");
	cog2 = (Sprite*)rootNode->getChildByName("cog2");
	cog3 = (Sprite*)rootNode->getChildByName("cog3");

	player1 = (Sprite*)rootNode->getChildByName("player");
	background = (Sprite*)rootNode->getChildByName("back1");
	background2 = (Sprite*)rootNode->getChildByName("back2");

	//Varable Inits
	_player1touched = false;
	_player1clicked = false;
	_player1alive = true;
	player1flipped = false;
	_player1cogNumber = 0;
	player1dirVector = Vec2(0.0f, 0.0f);
	_player1closeCog = cog1->getPosition();
	_player1scrollSpeed = 5.0f;

	GameManager::sharedGameManager()->ResetScore();
	// Touch Listener
	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = CC_CALLBACK_2(CoopScene::onTouchBegan, this);
	touchListener->onTouchEnded = CC_CALLBACK_2(CoopScene::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

	//score label
	int score = 0;
	__String* tempScore = __String::createWithFormat("%i", score);
	scoreLabel = (ui::Text*)rootNode->getChildByName("scoreLabel");
	scoreLabel->setText(tempScore->getCString());

	addChild(rootNode);
	this->scheduleUpdate();
	return true;
}

void CoopScene::update(float)
{
	r++;
	cog1->setRotation(r);
	cog2->setRotation(r);
	cog3->setRotation(r);

	player1CheckForClosest();
	player1CogCollide();

	if (!_player1touched)
	{
		player1WallCollide();
		player1MovePlayer();
		player1ScrollingBackground();
		GameManager::sharedGameManager()->AddToScore(1);
		scoreLabel->setString(StringUtils::format("%d", GameManager::sharedGameManager()->GetScore()));

	}
	else
	{
		player1PlayerSpin();
	}

	if (cog1->getPosition().y < 0.0f)
	{
		player1ResetCog(1);
	}
	if (cog2->getPosition().y < 0.0f)
	{
		player1ResetCog(2);
	}
	if (cog3->getPosition().y < 0.0f)
	{
		player1ResetCog(3);
	}
	///////////////////////////////////

	if (!_player1alive)
	{
		auto mainScene = GameOver::createScene();
		CCDirector::getInstance()->replaceScene(mainScene);
	}
}

bool CoopScene::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event) {
	auto size = Director::getInstance()->getVisibleSize();
	int leftWall = ((size.width / 100) * 19);
	int rightWall = ((size.width / 100) * 83);

	if (touch->getLocation().x < (size.width / 2) && player1->getPosition().x < (size.width / 2))
	{
		if (player1->getPosition().x > _player1closeCog.x)
		{
			_player1clockwise = false;
		}
		else
		{
			_player1clockwise = true;

		}
		if (!_player1clicked && player1CloseEnough())
			_player1touched = true;

		_player1clicked = true;
	}

	if (touch->getLocation().x > (size.width / 2) && player1->getPosition().x > (size.width / 2))
	{
		if (player1->getPosition().x > _player1closeCog.x)
		{
			_player1clockwise = false;
		}
		else
		{
			_player1clockwise = true;

		}
		if (!_player1clicked && player1CloseEnough())
			_player1touched = true;

		_player1clicked = true;
	}

	return true;
}

void CoopScene::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event)
{
	_player1touched = false;
}

void CoopScene::player1ResetCog(int cogNum)
{
	auto size = Director::getInstance()->getVisibleSize();
	int leftWall = ((size.width / 100) * 22);
	int rightWall = ((size.width / 100) * 80);

	RandomHelper rand = RandomHelper();
	int randomX = rand.random_int(leftWall, rightWall);

	if (cogNum == 1)
	{
		cog1->setPosition(randomX, size.height);
		_player1clicked = false;
	}
	randomX = rand.random_int(leftWall, rightWall);
	if (cogNum == 2)
	{
		cog2->setPosition(randomX, size.height);
		_player1clicked = false;
	}
	randomX = rand.random_int(leftWall, rightWall);

	if (cogNum == 3)
	{
		cog3->setPosition(randomX, size.height);
		_player1clicked = false;
	}
}

void CoopScene::player1CheckForClosest()
{
	Vec2 play = player1->getPosition();
	Vec2 play1 = player1->getPosition();
	Vec2 play2 = player1->getPosition();
	Vec2 cog = cog1->getPosition();
	Vec2 cog_1 = cog2->getPosition();
	Vec2 cog_2 = cog3->getPosition();
	play -= cog;
	play1 -= cog_1;
	play2 -= cog_2;

	float a = play.length();
	float b = play1.length();
	float c = play2.length();

	if (!_player1touched)
	{
		if (play.length() < play1.length() && play.length() < play2.length())
		{
			_player1closeCog = cog1->getPosition();
		}
		if (play1.length() < play.length() && play1.length() < play2.length())
		{
			_player1closeCog = cog2->getPosition();
		}
		if (play2.length() < play.length() && play2.length() < play1.length())
		{
			_player1closeCog = cog3->getPosition();
		}
	
	}
}

void CoopScene::player1CogCollide()
{
	CCRect pBox = player1->getBoundingBox();
	Vec2 c1Box = cog1->getPosition();
	Vec2 c2Box = cog2->getPosition();
	Vec2 c3Box = cog3->getPosition();

	if (pBox.intersectsCircle(c1Box, 31) || pBox.intersectsCircle(c2Box, 31) || pBox.intersectsCircle(c3Box, 31))
	{
		_player1alive = false;
		cocos2d::log("cog collide");
	}
}

void CoopScene::player1WallCollide()
{
	auto size = Director::getInstance()->getVisibleSize(); //27 ! 72
	float leftWall = ((size.width / 100) * 0);
	float rightWall = (size.width);

	Vec2 play = player1->getPosition();

	if (play.x < leftWall || play.x > rightWall)
	{
		//MASSIVE EXPLOSION AND HORN SOUNDS
		_player1alive = false;
		cocos2d::log("wall collide");
	}
}

void CoopScene::player1PlayerSpin()
{
	float s = sin(0.1);
	float c = cos(0.1);

	Vec2 point = _player1closeCog;
	Vec2 Player = player1->getPosition();
	Vec2 playerpos = Player;

	playerpos -= point;

	if (!_player1clockwise)
	{
		player1->setFlipY(true);
		player1flipped = true;
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

		player1->setPosition(playerpos);
		player1SetRobotRotation();

	}
	else
	{
		//Clockwise Spin
		player1->setFlipY(false);
		player1flipped = false;

		float x = playerpos.x * c + playerpos.y * s;
		float y = -playerpos.x * s + playerpos.y * c;

		playerpos.x = x + point.x;
		playerpos.y = y + point.y;

		//Moves player closer
		/*if (Player.y >= point.y - 4.0f && Player.y <= point.y + 4.0f && Player.x > point.x)
		{
		playerpos.x -= 20.0f;
		}*/

		player1->setPosition(playerpos);
		player1SetRobotRotation();
	}
}

void CoopScene::player1MovePlayer()
{
	auto size = Director::getInstance()->getVisibleSize();
	float moveY = ((size.height / 100) * 19);

	//Player Movement
	//x
	float rot = cos(CC_DEGREES_TO_RADIANS(player1->getRotation() - 90));

	if (player1dirVector.length() > 0)
	{
		player1dirVector.normalize();
	}

	if (player1flipped)
		player1->setPosition(player1->getPosition().x + -(rot * 4), player1->getPosition().y);

	if (!player1flipped)
		player1->setPosition(player1->getPosition().x + (rot * 4), player1->getPosition().y);

	//y
	if (player1->getPosition().y != moveY && player1->getPosition().y > moveY) //19% of size.y
	{
		player1->setPosition(player1->getPosition().x, player1->getPosition().y - 2.0f);
	}
	if (player1->getPosition().y != moveY && player1->getPosition().y < moveY)
	{
		player1->setPosition(player1->getPosition().x, player1->getPosition().y + 2.0f);
	}

	//Cog Movement
	cog1->setPosition(cog1->getPosition().x, cog1->getPosition().y - _player1scrollSpeed);
	cog2->setPosition(cog2->getPosition().x, cog2->getPosition().y - _player1scrollSpeed);
	cog2->setPosition(cog3->getPosition().x, cog3->getPosition().y - _player1scrollSpeed);
}

bool CoopScene::player1CloseEnough()
{
	Vec2 play = player1->getPosition();
	Vec2 cog = _player1closeCog;
	play -= cog;

	if (play.length() > 300.0f)
	{
		return false;
	}

	return true;
}

void CoopScene::player1ScrollingBackground()
{
	auto size = Director::getInstance()->getVisibleSize();

	//scrolling background
	Vec2 Bg1Pos = background->getPosition();
	Vec2 Bg2Pos = background2->getPosition();

	background->setPosition(Bg1Pos.x, Bg1Pos.y - _player1scrollSpeed);
	background2->setPosition(Bg2Pos.x, Bg2Pos.y - _player1scrollSpeed);

	if (background2->getPosition().y == size.height)
	{
		background->setPosition(Bg1Pos.x, size.height * 2);
	}
	if (background->getPosition().y == size.height)
	{
		background2->setPosition(Bg2Pos.x, size.height * 2);
	}
}

void CoopScene::player1SetRobotRotation()
{
	Vec2 cog = _player1closeCog;
	Vec2 play = player1->getPosition();

	float angle = atan2(cog.y - play.y, cog.x - play.x);
	angle = CC_RADIANS_TO_DEGREES(angle);

	player1->setRotation(-angle);
}
