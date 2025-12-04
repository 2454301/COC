#include "GameScene.h"

USING_NS_CC;

Scene* Game::createScene() {
    return Game::create();
}

static void problemLoading(const char* filename) {
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in GameScene.cpp\n");
}

bool Game::init() {
    if (!Scene::init()) {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();

    // 具体的实现有待商议，目前先放置图标，确认场景切换功能正常
    auto sprite = Sprite::create("HelloWorld.png");
    if (sprite == nullptr) {
        problemLoading("'HelloWorld.png'");
    }
    else {
        sprite->setPosition(visibleSize.width / 2, visibleSize.height / 2);
        this->addChild(sprite, 1);
    }

    return true;
}