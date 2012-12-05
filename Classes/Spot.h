#ifndef _PADDLE_H_
#define _PADDLE_H_

#define NOTIFICATION_SPOT_MARKED "SPOT_MARKED"

#include "cocos2d.h"


USING_NS_CC;

static enum SpotMark
{
	kSpotMarkEmpty,
    kSpotMarkO,
    kSpotMarkX
} sportMark, objSpotMark;

class Spot : public CCSprite, public CCTargetedTouchDelegate
{

	bool wasClicked;

public:
	static SpotMark m_state;
	CCTexture2D* spotTextureX;
	CCTexture2D* spotTextureO;
	CCTexture2D* spotTextureEmpty;
    SpotMark objSpotMark;

    Spot(void);
    virtual ~Spot(void);
    int gridPos[2];

    CCRect rect();

    bool initWithTexture(CCTexture2D* aTexture);
    virtual void onEnter();
    virtual void onExit();
    bool containsTouchLocation(CCTouch* touch);
    virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);
    virtual void ccTouchEnded(CCTouch* touch, CCEvent* event);
    virtual CCObject* copyWithZone(CCZone *pZone);

    virtual void touchDelegateRetain();
    virtual void touchDelegateRelease();

    virtual void setGridPos(int i, int n);

    static Spot* spotWithTexture();
};

#endif
