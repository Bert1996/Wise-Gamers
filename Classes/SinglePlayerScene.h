

#include "cocos2d.h"
#include "SoundManager.h"

class SinglePlayerScene : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();
	virtual bool init();
	CREATE_FUNC(SinglePlayerScene);
};
