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
    if (!Scene::init()) {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();

    // 1. 创建开始按钮
    auto startItem = MenuItemImage::create("GameStartButton.png", "GameStartButton.png", CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));

    if (startItem == nullptr || startItem->getContentSize().width <= 0 || startItem->getContentSize().height <= 0) {
        problemLoading("'GameStartButton.png'");
    }
    else {
        startItem->setPosition(Vec2(visibleSize.width / 2, 200));
        startItem->setScale(1.2f);

        auto runBreathAction = [startItem]() {
            startItem->stopAllActions();
            auto scaleUp = EaseSineInOut::create(ScaleTo::create(1.5f, 1.3f));
            auto scaleDown = EaseSineInOut::create(ScaleTo::create(1.5f, 1.2f));
            startItem->runAction(RepeatForever::create(Sequence::create(scaleUp, scaleDown, nullptr)));
            };

        // 初始启动呼吸
        runBreathAction();

        // --- ✨ 核心修改：添加鼠标监听器 ---
        auto mouseListener = EventListenerMouse::create();

        // 记录鼠标是否已经在按钮上的状态，防止重复触发动画
        static bool isMouseOver = false;

        mouseListener->onMouseMove = [startItem, runBreathAction](Event* event) mutable {
            EventMouse* e = (EventMouse*)event;
            Vec2 location = e->getLocationInView();
            // 坐标转换：将屏幕坐标转为相对于父节点的坐标
            Vec2 localPos = startItem->getParent()->convertToNodeSpace(location);

            // 检查鼠标是否在按钮的范围内
            if (startItem->getBoundingBox().containsPoint(localPos)) {
                if (!isMouseOver) { // 只有“新移入”时才执行
                    isMouseOver = true;
                    startItem->stopAllActions(); // 停止呼吸
                    // 丝滑变大到 1.5 倍
                    startItem->runAction(EaseBackOut::create(ScaleTo::create(0.3f, 1.5f)));
                }
            }
            else {
                if (isMouseOver) { // 只有“刚离开”时才执行
                    isMouseOver = false;
                    // 恢复呼吸动画
                    runBreathAction();
                }
            }
            };

        _eventDispatcher->addEventListenerWithSceneGraphPriority(mouseListener, this);
    }

    auto menu = Menu::create(startItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    auto sprite = Sprite::create("StartScene.jpeg");
    if (sprite) {
        sprite->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
        sprite->setScale(1.3);
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
