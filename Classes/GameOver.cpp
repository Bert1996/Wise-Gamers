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

	auto home = rootNode->getChildByName<cocos2d::ui::Button*>("Home");



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

	if (GameManager::sharedGameManager()->GetScore() == -1)
	{
		__String* tempScore = __String::createWithFormat("%i", score);
		scoreLabel = (ui::Text*)rootNode->getChildByName("scoreLabel");
		scoreLabel->setText(tempScore->getCString());
		scoreLabel->setString(StringUtils::format("Player 1 wins"));

		first = (ui::Text*)rootNode->getChildByName("first");

		sec = (ui::Text*)rootNode->getChildByName("sec");

		third = (ui::Text*)rootNode->getChildByName("third");

		fourth = (ui::Text*)rootNode->getChildByName("fourth");

		fifth = (ui::Text*)rootNode->getChildByName("fifth");

		first->setOpacity(0);
		sec->setOpacity(0);
		third->setOpacity(0);
		fourth->setOpacity(0);
		fifth->setOpacity(0);
	}
	if (GameManager::sharedGameManager()->GetScore() == -2)
	{
		__String* tempScore = __String::createWithFormat("%i", score);
		scoreLabel = (ui::Text*)rootNode->getChildByName("scoreLabel");
		scoreLabel->setText(tempScore->getCString());
		scoreLabel->setString(StringUtils::format("Player 2 wins"));
	}
	if (GameManager::sharedGameManager()->GetScore() > 0)
	{
		__String* tempScore = __String::createWithFormat("%i", score);
		scoreLabel = (ui::Text*)rootNode->getChildByName("scoreLabel");
		scoreLabel->setText(tempScore->getCString());
		scoreLabel->setString(StringUtils::format("%d", GameManager::sharedGameManager()->GetScore()));
		UpdateHighScores();

		first = (ui::Text*)rootNode->getChildByName("first");
		first->setText(tempScore->getCString());
		first->setString(StringUtils::format("%d", (int)cocos2d::UserDefault::getInstance()->getFloatForKey("first")));

		sec = (ui::Text*)rootNode->getChildByName("sec");
		sec->setText(tempScore->getCString());
		sec->setString(StringUtils::format("%d", (int)cocos2d::UserDefault::getInstance()->getFloatForKey("sec")));

		third = (ui::Text*)rootNode->getChildByName("third");
		third->setText(tempScore->getCString());
		third->setString(StringUtils::format("%d", (int)cocos2d::UserDefault::getInstance()->getFloatForKey("third")));

		fourth = (ui::Text*)rootNode->getChildByName("fourth");
		fourth->setText(tempScore->getCString());
		fourth->setString(StringUtils::format("%d", (int)cocos2d::UserDefault::getInstance()->getFloatForKey("fourth")));

		fifth = (ui::Text*)rootNode->getChildByName("fifth");
		fifth->setText(tempScore->getCString());
		fifth->setString(StringUtils::format("%d", (int)cocos2d::UserDefault::getInstance()->getFloatForKey("fifth")));

		first->setOpacity(255);
		sec->setOpacity(255);
		third->setOpacity(255);
		fourth->setOpacity(255);
		fifth->setOpacity(255);
	}

	addChild(rootNode);
	return true;
}

bool GameOver::UpdateHighScores()
{
	if (!cocos2d::UserDefault::getInstance()->getFloatForKey("first") &&
		!cocos2d::UserDefault::getInstance()->getFloatForKey("sec") &&
		!cocos2d::UserDefault::getInstance()->getFloatForKey("third") &&
		!cocos2d::UserDefault::getInstance()->getFloatForKey("fourth") &&
		!cocos2d::UserDefault::getInstance()->getFloatForKey("fifth"))
	{
		cocos2d::UserDefault::getInstance()->setFloatForKey("first", 0);
		cocos2d::UserDefault::getInstance()->setFloatForKey("sec", 0);
		cocos2d::UserDefault::getInstance()->setFloatForKey("third", 0);
		cocos2d::UserDefault::getInstance()->setFloatForKey("fourth", 0);
		cocos2d::UserDefault::getInstance()->setFloatForKey("fifth", 0);
	}
	float first = cocos2d::UserDefault::getInstance()->getFloatForKey("first");
	float sec = cocos2d::UserDefault::getInstance()->getFloatForKey("sec");
	float third = cocos2d::UserDefault::getInstance()->getFloatForKey("third");
	float fourth = cocos2d::UserDefault::getInstance()->getFloatForKey("fourth");
	float fifth = cocos2d::UserDefault::getInstance()->getFloatForKey("fifth");

	float score = GameManager::sharedGameManager()->GetScore();

	if (score > first)
	{
		cocos2d::UserDefault::getInstance()->setFloatForKey("first", score);
		return true;
	}
	if (score > sec)
	{
		cocos2d::UserDefault::getInstance()->setFloatForKey("fifth", fourth);
		cocos2d::UserDefault::getInstance()->setFloatForKey("fourth", third);
		cocos2d::UserDefault::getInstance()->setFloatForKey("third", sec);
		cocos2d::UserDefault::getInstance()->setFloatForKey("sec", score);
		return true;
	}
	if (score > third)
	{
		cocos2d::UserDefault::getInstance()->setFloatForKey("fifth", fourth);
		cocos2d::UserDefault::getInstance()->setFloatForKey("fourth", third);
		cocos2d::UserDefault::getInstance()->setFloatForKey("third", score);
		return true;
	}
	if (score > fourth)
	{
		cocos2d::UserDefault::getInstance()->setFloatForKey("fifth", fourth);
		cocos2d::UserDefault::getInstance()->setFloatForKey("fourth", score);
		return true;
	}
	if (score > fifth)
	{
		cocos2d::UserDefault::getInstance()->setFloatForKey("fifth", score);
		return true;
	}

	return false;

}

GameOver::GameOver()
{

}


GameOver::~GameOver()
{
}


