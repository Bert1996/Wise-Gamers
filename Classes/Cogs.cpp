#include "Cogs.h"
#include "cocostudio/CocoStudio.h"

using namespace cocos2d;

Cogs* Cogs::create()
{
	Cogs* cogNode = new Cogs();
	if (cogNode->init())
	{
		cogNode->autorelease();
		return cogNode;
	}
	else
	{
		CC_SAFE_DELETE(cogNode);
		return nullptr;
	}
	return cogNode;
}

bool Cogs::init()
{
	if	(!Node::init())
	{
		return false;
	}

	auto rootNode = CSLoader::createNode("Cog.csb");
	addChild(rootNode);

	auto winSize = Director::getInstance()->getVisibleSize();
	this->setPosition(Vec2(0.0f, winSize.height*0.7f));
	this->setAnchorPoint(Vec2(0.0f, 0.5f));
	this->scheduleUpdate();

	topCog = (Sprite*)rootNode->getChildByName("topCog");
	bottomCog = (Sprite*)rootNode->getChildByName("bottomCog");
	cantGrappelCog = (Sprite*)rootNode->getChildByName("cog_cantGrappel");
	saw = (Sprite*)rootNode->getChildByName("saw");

	startXPosition = winSize.width;
	startYPosition_top = topCog->getPositionY();
	startYPosition_bottom = bottomCog->getPositionY();
	startYPosition_cantGrappelCog = bottomCog->getPositionY();
	startYPosition_saw = bottomCog->getPositionY();
	topCog->setPosition(startXPosition, startYPosition_top);
	bottomCog->setPosition(startXPosition, startYPosition_bottom);
	cantGrappelCog->setPosition(startXPosition, startYPosition_cantGrappelCog);
	saw->setPosition(startXPosition, startYPosition_saw);

	//Speed of cogs at start.
	currentSpeed = 100.0f;
}

void Cogs::update(float delta)
{
	auto  winSize = Director::getInstance()->getVisibleSize();

	//Move the cogs to the left.
	Vec2 currentTopPos = topCog->getPosition();
	topCog->setPosition(currentTopPos.x - currentSpeed*delta, currentTopPos.y);

	Vec2 currentBottomPos = bottomCog->getPosition();
	bottomCog->setPosition(currentBottomPos.x - currentSpeed*delta, currentBottomPos.y);

	Vec2 currentCantGrappelCogPos = cantGrappelCog->getPosition();
	cantGrappelCog->setPosition(currentCantGrappelCogPos.x - currentSpeed*delta, currentCantGrappelCogPos.y);

	Vec2 currentSawPos = saw->getPosition();
	saw->setPosition(currentBottomPos.x - currentSpeed*delta, currentBottomPos.y);

	if (currentTopPos.x < -bottomCog->getBoundingBox().size.width*0.3f)
	{

		float newXPos = winSize.width + bottomCog->getBoundingBox().size.width*0.7f;
		//Get a random position for the regenerated cogs.
		float yOffset = rand_0_1()*winSize.height*0.1f;

		//Set the new positionings.
		topCog->setPosition(newXPos, currentTopPos.y - yOffset);
		bottomCog->setPosition(newXPos, currentBottomPos.y - yOffset);
		cantGrappelCog->setPosition(newXPos, currentCantGrappelCogPos.y - yOffset);
		saw->setPosition(newXPos, currentBottomPos.y - yOffset);

		//Increase the speed of the cogs each time they are repositioned.
		currentSpeed += 10.0f;
	}
}

Cogs::Cogs()
{
}

//-------------------------------------------------------------------------

Cogs::~Cogs()
{

}

void Cogs::reset()
{
	topCog->setPosition(startXPosition, startYPosition_top);
	bottomCog->setPosition(startXPosition, startYPosition_bottom);
	cantGrappelCog->setPosition(startXPosition, startYPosition_cantGrappelCog);
	saw->setPosition(startXPosition, startYPosition_bottom);

	currentSpeed = 200.0f;
}