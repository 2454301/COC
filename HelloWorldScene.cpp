#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "GameScene.h"
#include "VillageScene.h"

USING_NS_CC;

Scene* HelloWorld::createScene() {
    return HelloWorld::create();
}

// 对错误情况的处理方式
static void problemLoading(const char* filename) {
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// 对初始化函数init的实现
bool HelloWorld::init() {
    // 套路，先调用父类的初始化函数
    if ( !Scene::init() ) {
        return false;
    }

    // 获取可视范围信息
    auto visibleSize = Director::getInstance()->getVisibleSize();

    // 创建一个“开始游戏”按钮
    auto startItem = MenuItemImage::create("GameStartButton.png", "GameStartButton.png", CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));

    if (startItem == nullptr || startItem->getContentSize().width <= 0 || startItem->getContentSize().height <= 0) {
        problemLoading("'GameStartButton.png' and 'GameStartButton.png'");
    }
    else {
        float x = visibleSize.width / 2;
        float y = 200;

        // 设置按钮的位置坐标（屏幕中间偏下位置）
        startItem->setPosition(Vec2(x,y));

        // 设置按钮大小
        startItem->setScale(1.2);
    }

    // 创建一个菜单变量（MenuItemImage类需要固定在菜单上）
    // 将开始按钮添加到菜单
    auto menu = Menu::create(startItem, NULL);

    // 菜单位置设置在原点
    menu->setPosition(Vec2::ZERO);

    // 将菜单添加到场景
    this->addChild(menu, 1);

    // 创建一个精灵（游戏开屏图片）
    auto sprite = Sprite::create("StartScene.jpeg");
    if (sprite == nullptr) {
        problemLoading("'StartScene.jpeg'");
    }
    else {
        // 将图片置于场景中央
        sprite->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));

        // 设置图片大小，使其占满整个画面
        sprite->setScale(1.3);

        // 将图片添加到场景
        this->addChild(sprite, 0);
    }
    return true;
}

// 点击按钮后，触发此函数，将当前场景更换为游戏场景
void HelloWorld::menuCloseCallback(Ref* pSender) {
    // 创建一个游戏场景
    auto gamescene = Village::createScene();

    // 更换场景
    Director::getInstance()->replaceScene(gamescene);
}
