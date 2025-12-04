#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"

// Game类显示游戏主场景
class Game : public cocos2d::Scene {
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    void menuCloseCallback(cocos2d::Ref* pSender);

    CREATE_FUNC(Game);
};


#endif // __GAME_SCENE_H__
