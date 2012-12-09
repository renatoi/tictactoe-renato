#define COCOS2D_DEBUG 1
#include "TicTacToe.h"
#include "Resources.h"
#include <iostream>

using namespace cocos2d;

#define EFFECT_FILE "effect1.wav"

CCScene* TicTacToe::scene()
{
    CCScene * scene = NULL;
    do 
    {
        // 'scene' is an autorelease object
        scene = CCScene::create();
        CC_BREAK_IF(! scene);

        // 'layer' is an autorelease object
        TicTacToe *layer = TicTacToe::create();
        CC_BREAK_IF(! layer);

        // add layer as a child to scene
        scene->addChild(layer);
    } while (0);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool TicTacToe::init()
{
    bool bRet = false;
    do 
    {
        //////////////////////////////////////////////////////////////////////////
        // super init first
        //////////////////////////////////////////////////////////////////////////

        CC_BREAK_IF(! CCLayerColor::initWithColor( ccc4(51,51,51,255) ) );

        //////////////////////////////////////////////////////////////////////////
        // the code
        //////////////////////////////////////////////////////////////////////////

        // notification center
        CCNotificationCenter::sharedNotificationCenter()->addObserver(
            this,
            callfuncO_selector(TicTacToe::checkGameState),
            NOTIFICATION_SPOT_MARKED,
            NULL
        );

        // init sound
		CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect( CCFileUtils::sharedFileUtils()->fullPathFromRelativePath(s_SoundFart) );
		CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect( CCFileUtils::sharedFileUtils()->fullPathFromRelativePath(s_SoundFart2) );
		CocosDenshion::SimpleAudioEngine::sharedEngine()->setEffectsVolume(1);
		CocosDenshion::SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(0.5);
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic(std::string(CCFileUtils::sharedFileUtils()->fullPathFromRelativePath(s_SoundBackground)).c_str(), true);

        // add a menu item with "X" image, which is clicked to quit the program.
        CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
            "CloseNormal.png",
            "CloseSelected.png",
            this,
            menu_selector(TicTacToe::menuCloseCallback)
        );

        CC_BREAK_IF(! pCloseItem);

        // place the menu item bottom-right conner.
        pCloseItem->setPosition(ccp(CCDirector::sharedDirector()->getWinSize().width - 20, 20));

        // create a menu with the "close" menu item, it's an auto release object.
        CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
        pMenu->setPosition(CCPointZero);
        CC_BREAK_IF(! pMenu);

        // add the menu to HelloWorld layer as a child layer.
        this->addChild(pMenu, 1);

        // add title
        CCLabelBMFont* pLabel = CCLabelBMFont::create("Jogo da Veia", "bitmapFontTest.fnt");

		// not true type
		// const ccColor3B darkgrey={243,243,243};
		// pLabel->setColor(darkgrey);
        CC_BREAK_IF(! pLabel);

        // get window size and place the label upper. 
        size = CCDirector::sharedDirector()->getWinSize();
        pLabel->setPosition(ccp(size.width / 2, size.height - 50));

        // add the label to HelloWorld layer as a child layer.
        this->addChild(pLabel, 1);

		// repeatable fadein/fadeout action sequence
		CCSequence * pulseSequence = CCSequence::createWithTwoActions(CCFadeIn::create(1), CCFadeOut::create(1));
		CCRepeatForever* repeatAction = CCRepeatForever::create(pulseSequence);
		pLabel->runAction(repeatAction);

        // background
        CCSprite* pSplash = CCSprite::create(s_Background);
        CC_BREAK_IF(! pSplash);
		
        // Place the sprite on the center of the screen
        pSplash->setPosition(ccp(size.width/2, size.height/2));
		
        // Add the sprite to HelloWorld layer as a child layer.
        this->addChild(pSplash, 0);

        this->drawBoard();

        bRet = true;
    } while (0);

    return bRet;
}

void TicTacToe::drawBoard() {
    // draw board
	CCSprite* pBoard = CCSprite::create(s_Board);
		
    // Place the sprite on the center of the screen
    pBoard->setPosition(ccp(size.width/2, size.height/2));
		
    // Add the sprite to HelloWorld layer as a child layer.
    this->addChild(pBoard, 0);

	// Ok, now we want to add the spot marks
	Spot* spot;
	CCPoint pos;


	for(int i = 1; i <= 3; i++)
	{
		for (int n = 1; n <= 3; n++)
        {
            pos = ccp(VisibleRect::center().x + (180*i-360), VisibleRect::center().y + (180*n-360)*-1);

            spot = Spot::spotWithTexture();
            spot->setGridPos(i-1,n-1);
			spot->setPosition(pos);
            this->addChild(spot, 0);

            if (matrixOfSpot.size() <= i-1) {
                matrixOfSpot.push_back(std::vector<Spot*>());
            }
            matrixOfSpot[i-1].push_back(spot);

		}
	}

}

void TicTacToe::checkGameState(CCObject* obj) {
    Spot* spot = static_cast<Spot*>(obj);
    int x = spot->gridPos[0];
    int y = spot->gridPos[1];
    int m = spot->objSpotMark;
    int win = 0;

    CCLog("Mark: %d", m);
    CCLog("x: %d", x);
    CCLog("y: %d", y);

    // type
    for (int i = 0; i < 4; i++) {
        // check previous
        if (getSpotMark(x, y, -1, i) == m) {
            // check previous of previous if any
            if (getSpotMark(x, y, -2, i) == m) {
                drawLine(x, y, i);
                win++;
                continue;
            }
            // check next
            else if (getSpotMark(x, y, 1, i) == m) {
                drawLine(x, y, i);
                win++;
                continue;
            }
        }
        // check next
        if (getSpotMark(x, y, 1, i) == m) {
            // check next of next
            if (getSpotMark(x, y, 2, i) == m) {
                drawLine(x, y, i);
                win++;
                continue;
            }
        }
    }
    if (win) {
        Spot::m_state = kSpotMarkDisabled;
    }
}

int TicTacToe::getSpotMark(int x, int y, int step, int type) {
    // return spot mark or -1 if spot does not exist
    switch (type) {
        case 0:
            return (x+step >= 0 && x+step < matrixOfSpot.size()) ? matrixOfSpot[x+step][y]->objSpotMark : -1;
        case 1:
            return (y+step >= 0 && y+step < matrixOfSpot.size()) ? matrixOfSpot[x][y+step]->objSpotMark : -1;
        case 2:
            return (y+step >= 0 && y+step < matrixOfSpot.size() && x+step >= 0 && x+step < matrixOfSpot.size()) ? matrixOfSpot[x+step][y+step]->objSpotMark : -1;
        case 3:
            return (y-step >= 0 && y-step < matrixOfSpot.size() && x+step >= 0 && x+step < matrixOfSpot.size()) ? matrixOfSpot[x+step][y-step]->objSpotMark : -1;
    }
}

void TicTacToe::drawLine(int x, int y, int type) {
	CCSprite* pLine = CCSprite::create(s_Line);
    CCPoint pos;

    switch(type) {
        case 0:
            pos = ccp(VisibleRect::center().x, matrixOfSpot[x][y]->getPositionY());
            break;
        case 1:
            pLine->setRotation(90);
            pos = ccp(matrixOfSpot[x][y]->getPositionX(), VisibleRect::center().y);
            break;
        case 2:
            pLine->setRotation(45);
            pos = ccp(VisibleRect::center().x, VisibleRect::center().y);
            break;
        case 3:
            pLine->setRotation(-45);
            pos = ccp(VisibleRect::center().x, VisibleRect::center().y);
            break;
    }

    pLine->setPosition(pos);
    this->addChild(pLine, 0);
}

void TicTacToe::menuCloseCallback(CCObject* pSender)
{
    // "close" menu item clicked
    CCDirector::sharedDirector()->end();
}

TicTacToe::~TicTacToe() {
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, NOTIFICATION_SPOT_MARKED);
}