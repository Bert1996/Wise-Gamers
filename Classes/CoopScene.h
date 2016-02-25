

#include "cocos2d.h"
#include "SoundManager.h"

class CoopScene : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();
	virtual bool init();
	CREATE_FUNC(CoopScene);
};
