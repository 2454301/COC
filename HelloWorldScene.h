// 头文件套路
#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

// 一个HelloWorld类，作用是显示开屏画面
class HelloWorld : public cocos2d::Scene {
public:
    // 场景创建函数
    static cocos2d::Scene* createScene();

    // 初始化函数，此处为虚，需要在cpp中写具体实现
    virtual bool init();

    // 点击反馈函数
    void menuCloseCallback(cocos2d::Ref* pSender);

    // 套路，作用是为此类添加一个create()函数
    CREATE_FUNC(HelloWorld);
};

// 头文件套路
#endif // __HELLOWORLD_SCENE_H__
