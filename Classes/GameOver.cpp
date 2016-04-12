#include "GameOver.h"



cocos2d::Scene * GameOver::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = GameOver::create();


	// add layer as a child to scene
	scene->addChild(layer);
	return scene;
}

bool GameOver::init()
{
	if (!Layer::init())
	{
		return false;
	}
	auto rootNode = CSLoader::createNode("GameOverScreen.csb");

	auto retry = rootNode->getChildByName<cocos2d::ui::Button*>("Retry");
	auto home = rootNode->getChildByName<cocos2d::ui::Button*>("Home");

	retry->addTouchEventListener([&](Ref* sender, cocos2d::ui::Widget::TouchEventType type)
	{
		//Reset Score before starting game
		GameManager::sharedGameManager()->ResetScore();

		auto mainScene = SinglePlayerScene::createScene();

		switch (type)
		{

		case ui::Widget::TouchEventType::BEGAN:
			//SoundManager::PlayButtonSound();
			break;
		case ui::Widget::TouchEventType::ENDED:

			CCDirector::getInstance()->replaceScene(mainScene);

			break;
		default:
			break;
		}
	});

	home->addTouchEventListener([&](Ref* sender, cocos2d::ui::Widget::TouchEventType type)
	{
		//Reset Score before starting game
		GameManager::sharedGameManager()->ResetScore();

		auto mainScene = HelloWorld::createScene();

		switch (type)
		{

		case ui::Widget::TouchEventType::BEGAN:
			//SoundManager::PlayButtonSound();
			break;
		case ui::Widget::TouchEventType::ENDED:

			CCDirector::getInstance()->replaceScene(mainScene);

			break;
		default:
			break;
		}
	});
	int score = 0;
	__String* tempScore = __String::createWithFormat("%i", score);
	scoreLabel = (ui::Text*)rootNode->getChildByName("scoreLabel");
	scoreLabel->setText(tempScore->getCString());
	scoreLabel->setString(StringUtils::format("%d", GameManager::sharedGameManager()->GetScore()));

	addChild(rootNode);
	return true;
}

GameOver::GameOver()
{
	
}


GameOver::~GameOver()
{
}


