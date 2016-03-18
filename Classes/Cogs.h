#ifndef __Cogs__
#define __Cogs__


#include "cocos2d.h"

class Cogs : public cocos2d::Node
{
public:
	Cogs();
	~Cogs();

		virtual bool init() override;
		static Cogs* create();

		void         update(float);
		void		reset();


private:
	cocos2d::Sprite* topCog;
	cocos2d::Sprite* bottomCog;
	cocos2d::Sprite* cantGrappelCog;
	cocos2d::Sprite* saw;

	float            startXPosition;
	float            startYPosition_top;
	float            startYPosition_bottom;
	float            startYPosition_cantGrappelCog;
	float            startYPosition_saw;
	float            currentSpeed;


};
#endif // __Cogs__