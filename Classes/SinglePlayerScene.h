#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "cocos2d.h"
#include "SoundManager.h"
#include "Cogs.h"
#include "GameManager.h"
#include "stdio.h"

USING_NS_CC;
class SinglePlayerScene : public cocos2d::Layer
{
public:

	static cocos2d::Scene* createScene();
	CREATE_FUNC(SinglePlayerScene);
private:

	cocos2d::ui::Text* scoreLabel;
	void update(float)override;
	virtual bool init();
	virtual bool onTouchBegan(cocos2d::Touch*, cocos2d::Event*);
	virtual void onTouchEnded(cocos2d::Touch*, cocos2d::Event*);
	void resetCog(int cogNum);
	void CheckForClosest();
	void ScrollingBackground();
	void cogCollide();
	void wallCollide();
	void playerSpin();
	void movePlayer();
	bool CloseEnough();
	void SetRobotRotation();

	cocos2d::Sprite* cog1;
	cocos2d::Sprite* cog2;
	cocos2d::Sprite* player;
	cocos2d::Sprite* background;
	cocos2d::Sprite* background2;

	Vec2 dirVector;
	Vec2 _closeCog;
	bool _touched;
	bool _clockwise;
	bool _clicked;
	bool flipped;
	bool _alive;
	int _cogNumber;
	float r = 0;
	float _scrollSpeed;
};
