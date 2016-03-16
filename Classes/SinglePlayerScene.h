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
	virtual bool init();
	void update(float)override;
	void resetCog();
	void CheckForClosest();
	void ScrollingBackground();
	cocos2d::Sprite* cog1;
	cocos2d::Sprite* cog2;
	cocos2d::Sprite* cog_dangerous;
	cocos2d::Sprite* player;
	cocos2d::Sprite*		background;
	cocos2d::Sprite*		background2;
	int tol;
	virtual bool onTouchBegan(cocos2d::Touch*, cocos2d::Event*);
	virtual void onTouchEnded(cocos2d::Touch*, cocos2d::Event*);
	void cogCollide();
	void wallCollide();
	void playerSpin();
	void movePlayer();
	bool _touched;
	bool _clockwise;
	bool _clicked;
	bool _alive;
	bool CloseEnough();
	Vec2 dirVector;
	Vec2 _closeCog;
	float r = 0;;
	float sr = 0;
	int _cogNumber;
	CREATE_FUNC(SinglePlayerScene);

private:
	cocos2d::ui::Text* scoreLabel;
};
