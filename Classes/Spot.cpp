#include "Spot.h"
#include "Resources.h"
#include "SimpleAudioEngine.h"

Spot::Spot(void)
{
	wasClicked = false;
    objSpotMark = kSpotMarkEmpty;
	spotTextureX = CCTextureCache::sharedTextureCache()->addImage(s_MarkX);
	spotTextureO = CCTextureCache::sharedTextureCache()->addImage(s_MarkO);
	spotTextureEmpty = CCTextureCache::sharedTextureCache()->addImage(s_MarkEmpty);
}

SpotMark Spot::m_state = kSpotMarkEmpty;

Spot::~Spot(void)
{
}

CCRect Spot::rect()
{
    CCSize s = getTexture()->getContentSize();
    return CCRectMake(-s.width / 2, -s.height / 2, s.width, s.height);
}

Spot* Spot::spotWithTexture()
{

    Spot* pSpot = new Spot();
    pSpot->initWithTexture(pSpot->spotTextureEmpty);
    pSpot->autorelease();

    return pSpot;
}

bool Spot::initWithTexture(CCTexture2D* aTexture)
{
    if( CCSprite::initWithTexture(aTexture) ) 
    {
        Spot::m_state = kSpotMarkEmpty;
    }
    
    return true;
}

void Spot::onEnter()
{
    CCDirector* pDirector = CCDirector::sharedDirector();
    pDirector->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
    CCSprite::onEnter();
}

void Spot::onExit()
{
    CCDirector* pDirector = CCDirector::sharedDirector();
    pDirector->getTouchDispatcher()->removeDelegate(this);
    CCSprite::onExit();
}    

bool Spot::containsTouchLocation(CCTouch* touch)
{
    return rect().containsPoint(convertTouchToNodeSpaceAR(touch));
}

bool Spot::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
    if ( !containsTouchLocation(touch) || wasClicked ) return false;
	wasClicked = true;
	if (Spot::m_state == kSpotMarkX)
	{
		this->setTexture(spotTextureO);
        this->objSpotMark = Spot::m_state = kSpotMarkO;
	}
	else
	{
		this->setTexture(spotTextureX);
        this->objSpotMark = Spot::m_state = kSpotMarkX;
	}

    return true;
}

CCObject* Spot::copyWithZone(CCZone *pZone)
{
    this->retain();
    return this;
}

void Spot::ccTouchEnded(CCTouch* touch, CCEvent* event)
{   
	if (Spot::m_state == kSpotMarkX)
    {
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(std::string(CCFileUtils::sharedFileUtils()->fullPathFromRelativePath(s_SoundFart)).c_str());
	}
	else
    {
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(std::string(CCFileUtils::sharedFileUtils()->fullPathFromRelativePath(s_SoundFart2)).c_str());
	}

    CCNotificationCenter::sharedNotificationCenter()->postNotification(NOTIFICATION_SPOT_MARKED, (CCObject*) this);
} 

void Spot::touchDelegateRetain()
{
    this->retain();
}

void Spot::touchDelegateRelease()
{
    this->release();
}

void Spot::setGridPos(int i, int n) {
    gridPos[0] = i;
    gridPos[1] = n;
};
