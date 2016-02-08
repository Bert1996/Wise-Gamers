#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "stdio.h"
#include "GameManager.h"
#include <iomanip>

USING_NS_CC;

using namespace cocostudio::timeline;
using namespace cocos2d;

class HelloWorld : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();

    // implement the "static create()" method manually
	CREATE_FUNC(HelloWorld);

	void update(float);

	Vec2 highestPos;

	

private:
	cocos2d::Sprite* robot;
	cocos2d::ui::Text* scoreLabel;	


	bool _robotLeftSwitch;
	bool _robotRightSwitch;

	
	bool _robotUpSwitch;
	bool _robotDownSwitch;

};

#endif // __HELLOWORLD_SCENE_H__
