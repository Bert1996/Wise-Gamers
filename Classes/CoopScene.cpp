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
	player1 = (Sprite*)rootNode->getChildByName("player");
	background = (Sprite*)rootNode->getChildByName("leftBack1");
	background2 = (Sprite*)rootNode->getChildByName("leftBack2");

	//Varable Inits
	_player1touched = false;
	_player1clicked = false;
	_player1alive = true;
	player1flipped = false;
	_player1cogNumber = 0;
	player1dirVector = Vec2(0.0f, 0.0f);
	_player1closeCog = cog1->getPosition();

	//Sprite Inits for Player2
	cog3 = (Sprite*)rootNode->getChildByName("cog3");
	cog4 = (Sprite*)rootNode->getChildByName("cog4");
	background3 = (Sprite*)rootNode->getChildByName("leftBack3");
	background4 = (Sprite*)rootNode->getChildByName("leftBack4");

	//Varable Inits
	_player2touched = false;
	_player2clicked = false;
	_player2alive = true;
	player2flipped = false;
	_player2cogNumber = 0;
	player2dirVector = Vec2(0.0f, 0.0f);
	_player2closeCog = cog1->getPosition();

	// Touch Listener
	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = CC_CALLBACK_2(CoopScene::onTouchBegan, this);
	touchListener->onTouchEnded = CC_CALLBACK_2(CoopScene::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

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
	cog4->setRotation(r);

	player1CheckForClosest();
	//player1CogCollide();
	player2CheckForClosest();
	player2CogCollide();

	if (!_player1touched)
	{
		player1WallCollide();
		player1MovePlayer();
		player1ScrollingBackground();

		
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
	/////////////////////////////////////////
	if (!_player2touched)
	{
		player2WallCollide();
		player2MovePlayer();
		//player2ScrollingBackground();
	}
	else
	{
		player2PlayerSpin();
	}

	if (cog3->getPosition().y < 0.0f)
	{
		player2ResetCog(1);
	}
	if (cog4->getPosition().y < 0.0f)
	{
		player2ResetCog(2);
	}
	///////////////////////////////////

	if (!_player1alive /*|| !_player2alive*/)
	{
		auto mainScene = GameOver::createScene();
		CCDirector::getInstance()->replaceScene(mainScene);
	}
}

bool CoopScene::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event) {
	auto size = Director::getInstance()->getVisibleSize();
	int leftWall = ((size.width / 100) * 10);
	int rightWall = ((size.width / 100) * 40);

	if (touch->getLocation().x < (size.width / 2))
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
	else
	{
		if (player1->getPosition().x > _player2closeCog.x)
		{
			_player1clockwise = false;
		}
		else
		{
			_player1clockwise = true;

		}
		if (!_player2clicked && player2CloseEnough())
			_player2touched = true;

		_player2clicked = true;
	}
	return true;
}

void CoopScene::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event)
{
	_player1touched = false;
	_player2touched = false;
}

void CoopScene::player1ResetCog(int cogNum)
{
	auto size = Director::getInstance()->getVisibleSize();
	int leftWall = ((size.width / 100) * 10);
	int rightWall = ((size.width / 100) * 40);

	RandomHelper rand = RandomHelper();
	int randomX = rand.random_int(leftWall, rightWall);

	if (cogNum == 1)
	{
		cog1->setPosition(randomX, size.height);
		_player1clicked = false;
	}

	else
	{
		cog2->setPosition(randomX, size.height);
		_player1clicked = false;
	}
}

void CoopScene::player1CheckForClosest()
{
	Vec2 play = player1->getPosition();
	Vec2 play1 = player1->getPosition();
	Vec2 cog = cog1->getPosition();
	Vec2 cog_1 = cog2->getPosition();
	play -= cog;
	play1 -= cog_1;
	if (!_player1touched)
	{
		if (play.length() > play1.length())
		{
			_player1cogNumber = 2;
			_player1closeCog = cog2->getPosition();
		}
		else
		{
			_player1cogNumber = 1;
			_player1closeCog = cog1->getPosition();
		}
	}
}

void CoopScene::player1CogCollide()
{
	CCRect pBox = player1->getBoundingBox();
	Vec2 c1Box = cog1->getPosition();
	Vec2 c2Box = cog2->getPosition();
	if (pBox.intersectsCircle(c1Box, 31) || pBox.intersectsCircle(c2Box, 31))
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
	cog1->setPosition(cog1->getPosition().x, cog1->getPosition().y - 4.0f);
	cog2->setPosition(cog2->getPosition().x, cog2->getPosition().y - 4.0f);
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

	if (background->getPosition().y < 0)
	{
		background->setPosition(Bg1Pos.x, size.height * 2);
	}

	if (background2->getPosition().y < 0)
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

void CoopScene::player2ResetCog(int cogNum)
{
	auto size = Director::getInstance()->getVisibleSize();
	int leftWall = ((size.width / 100) * 60);
	int rightWall = ((size.width / 100) * 90);

	RandomHelper rand = RandomHelper();
	int randomX = rand.random_int(leftWall, rightWall);

	if (cogNum == 1)
	{
		cog3->setPosition(randomX, size.height);
		_player2clicked = false;
	}

	else
	{
		cog4->setPosition(randomX, size.height);
		_player2clicked = false;
	}
}

void CoopScene::player2CheckForClosest()
{
	Vec2 play = player1->getPosition();
	Vec2 play1 = player1->getPosition();
	Vec2 cog = cog3->getPosition();
	Vec2 cog_1 = cog4->getPosition();
	play -= cog;
	play1 -= cog_1;
	if (!_player2touched)
	{
		if (play.length() > play1.length())
		{
			_player2cogNumber = 2;
			_player2closeCog = cog3->getPosition();
		}
		else
		{
			_player2cogNumber = 1;
			_player2closeCog = cog4->getPosition();
		}
	}
}

void CoopScene::player2CogCollide()
{
	CCRect pBox = player1->getBoundingBox();
	Vec2 c1Box = cog3->getPosition();
	Vec2 c2Box = cog4->getPosition();
	if (pBox.intersectsCircle(c1Box, 31) || pBox.intersectsCircle(c2Box, 31))
	{
		_player1alive = false;
		cocos2d::log("cog collide");
	}
}

void CoopScene::player2WallCollide()
{
	auto size = Director::getInstance()->getVisibleSize(); //27 ! 72
	float leftWall = ((size.width / 100) * 0);
	float rightWall = ((size.width / 100) * 100);

	Vec2 play = player1->getPosition();

	if (play.x < leftWall || play.x > rightWall)
	{
		//MASSIVE EXPLOSION AND HORN SOUNDS
		_player2alive = false;
		cocos2d::log("wall collide");
	}
}

void CoopScene::player2PlayerSpin()
{
	float s = sin(0.1);
	float c = cos(0.1);

	Vec2 point = _player2closeCog;
	Vec2 Player = player1->getPosition();
	Vec2 playerpos = Player;

	playerpos -= point;

	if (!_player1clockwise)
	{
		player1->setFlipY(true);
		player2flipped = true;
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
		player2SetRobotRotation();

	}
	else
	{
		//Clockwise Spin
		player1->setFlipY(false);
		player2flipped = false;

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
		player2SetRobotRotation();
	}
}

void CoopScene::player2MovePlayer()
{
	auto size = Director::getInstance()->getVisibleSize();
	float moveY = ((size.height / 100) * 19);

	//Player Movement
	//x
	float rot = cos(CC_DEGREES_TO_RADIANS(player1->getRotation() - 90));

	if (player2dirVector.length() > 0)
	{
		player2dirVector.normalize();
	}

	if (player2flipped)
		player1->setPosition(player1->getPosition().x + -(rot * 4), player1->getPosition().y);

	if (!player2flipped)
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
	cog3->setPosition(cog3->getPosition().x, cog3->getPosition().y - 4.0f);
	cog4->setPosition(cog4->getPosition().x, cog4->getPosition().y - 4.0f);
}

bool CoopScene::player2CloseEnough()
{
	Vec2 play = player1->getPosition();
	Vec2 cog = _player2closeCog;
	play -= cog;

	if (play.length() > 300.0f)
	{
		return false;
	}

	return true;
}

void CoopScene::player2ScrollingBackground()
{
	auto size = Director::getInstance()->getVisibleSize();

	//scrolling background
	Vec2 Bg1Pos = background->getPosition();
	Vec2 Bg2Pos = background2->getPosition();

	background->setPosition(Bg1Pos.x, Bg1Pos.y - _player2scrollSpeed);
	background2->setPosition(Bg2Pos.x, Bg2Pos.y - _player2scrollSpeed);

	if (background->getPosition().y < 0)
	{
		background->setPosition(Bg1Pos.x, size.height * 2);
	}

	if (background2->getPosition().y < 0)
	{
		background2->setPosition(Bg2Pos.x, size.height * 2);
	}
}

void CoopScene::player2SetRobotRotation()
{
	Vec2 cog = _player2closeCog;
	Vec2 play = player1->getPosition();

	float angle = atan2(cog.y - play.y, cog.x - play.x);
	angle = CC_RADIANS_TO_DEGREES(angle);

	player1->setRotation(-angle);
}