#ifndef __TICTACTOE_SCENE_H__
#define __TICTACTOE_SCENE_H__
#define NOTIFICATION_SPOT_MARKED "SPOT_MARKED"

#include "cocos2d.h"
#include "VisibleRect.h"
#include "SimpleAudioEngine.h"
#include "Spot.h"
#include <vector>

class TicTacToe : public cocos2d::CCLayerColor
{

    CCSize size;

protected:
    std::vector<std::vector<Spot*>> matrixOfSpot;

public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

    // there's no 'id' in cpp, so we recommand to return the exactly class pointer
    static cocos2d::CCScene* scene();

    void drawBoard();

    void checkGameState(CCObject*);

    // a selector callback
    void menuCloseCallback(CCObject* pSender);

    ~TicTacToe();

    // implement the "static node()" method manually
    CREATE_FUNC(TicTacToe);
};

#endif  // __HELLOWORLD_SCENE_H__