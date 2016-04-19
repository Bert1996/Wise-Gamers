#include "HelloWorldScene.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

using namespace cocostudio::timeline;

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
    
    auto rootNode = CSLoader::createNode("Menu.csb");

	//Starting Sound
	SoundManager::PlayBackGroundMusic();
	
	//Sprites
	cog = (Sprite*)rootNode->getChildByName("cog");

	//Main Menu Buttons
	auto SinglePlayerButton = rootNode->getChildByName<cocos2d::ui::Button*>("SinglePlayer");
	auto CoopPlayerButton = rootNode->getChildByName<cocos2d::ui::Button*>("Co-op");
	auto VersusPlayerButton = rootNode->getChildByName<cocos2d::ui::Button*>("Versus");

	SinglePlayerButton->addTouchEventListener([&](Ref* sender, cocos2d::ui::Widget::TouchEventType type)
	{
		//Reset Score before starting game
		GameManager::sharedGameManager()->ResetScore();

		auto mainScene = SinglePlayerScene::createScene();
		
		switch (type)
		{

		case ui::Widget::TouchEventType::BEGAN:
			SoundManager::PlayButtonSound();
			break;
		case ui::Widget::TouchEventType::ENDED:

			CCDirector::getInstance()->replaceScene(mainScene);

			break;
		default:
			break;
		}
	});

	CoopPlayerButton->addTouchEventListener([&](Ref* sender, cocos2d::ui::Widget::TouchEventType type)
	{
		auto mainScene = CoopScene::createScene();

		switch (type)
		{

		case ui::Widget::TouchEventType::BEGAN:
			SoundManager::PlayButtonSound();
			break;
		case ui::Widget::TouchEventType::ENDED:
			
			CCDirector::getInstance()->replaceScene(mainScene);

			break;
		default:
			break;
		}
	});

	VersusPlayerButton->addTouchEventListener([&](Ref* sender, cocos2d::ui::Widget::TouchEventType type)
	{
		auto mainScene = VersusScene::createScene();

		switch (type)
		{

		case ui::Widget::TouchEventType::BEGAN:
			SoundManager::PlayButtonSound();
			break;
		case ui::Widget::TouchEventType::ENDED:

			CCDirector::getInstance()->replaceScene(mainScene);

			break;
		default:
			break;
		}
	});

	r = 0.0f;
    addChild(rootNode);
	this->scheduleUpdate();
    return true;
}

void HelloWorld::update(float)
{
	r++;
	cog->setRotation(r);

}
