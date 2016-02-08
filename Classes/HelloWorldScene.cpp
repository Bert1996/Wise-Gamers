#include "HelloWorldScene.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "stdio.h"

USING_NS_CC;

using namespace cocostudio::timeline;
using namespace cocos2d;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    auto rootNode = CSLoader::createNode("MainScene.csb");

    addChild(rootNode);

	this->scheduleUpdate();

	auto winSize = Director::getInstance()->getVisibleSize();

	//robot
	robot = (Sprite*)rootNode->getChildByName("Robot");


	int score = 0;
	//score label
	__String* tempScore = __String::createWithFormat("%i", score);

	scoreLabel = (ui::Text*)rootNode->getChildByName("ScoreLabel");
	scoreLabel->setText(tempScore->getCString());	


	highestPos = robot->getPosition();

    return true;
}

void HelloWorld::update(float deltaTime)
{
	Vec2 currentPos = robot->getPosition();		

	if (GetAsyncKeyState(0x26) && _robotUpSwitch)
	{
		_robotUpSwitch = false;
		robot->setPosition(currentPos.x, currentPos.y +1);
	}

	if (!GetAsyncKeyState(0x26))
	{
		_robotUpSwitch = true;
	}
	
	if (GetAsyncKeyState(0x28) && _robotDownSwitch)
	{
		_robotDownSwitch = false;
		robot->setPosition(currentPos.x, currentPos.y - 1);
	}

	if (!GetAsyncKeyState(0x28))
	{
		_robotDownSwitch = true;
	}

	if (GetAsyncKeyState(0x25) && _robotLeftSwitch)
	{
		_robotLeftSwitch = false;
		robot->setPosition(currentPos.x -1, currentPos.y);
	}

	if (!GetAsyncKeyState(0x25))
	{
		_robotLeftSwitch = true;
	}

	if (GetAsyncKeyState(0x27) && _robotRightSwitch)
	{
		_robotRightSwitch = false;
		robot->setPosition(currentPos.x + 1, currentPos.y);
	}

	if (!GetAsyncKeyState(0x27))
	{
		_robotRightSwitch = true;
	}

	//Display the score
	if (currentPos.y > highestPos.y)
	{
		GameManager::sharedGameManager()->AddToScore(1);
		scoreLabel->setString(StringUtils::format("%d", GameManager::sharedGameManager()->GetScore()));
		highestPos.y = currentPos.y;
	}	

	//GameManager::sharedGameManager()->AddToScore(1);
	//scoreLabel->setString(StringUtils::format("%d", GameManager::sharedGameManager()->GetScore()));
	
}


