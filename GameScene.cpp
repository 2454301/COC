#include "GameScene.h"
#include "Soldier.h"
#include "Building.h"

USING_NS_CC;

// 这里是游戏主场景，现在还没有背景，会有兵种选择菜单和初始出来的建筑
// 可选择兵种并放置，兵种会按照逻辑自动攻击

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

    // 初始化建筑
    createInitialBuildings();
    // 创建兵种菜单
    createSoldierMenu();
    // 注册触摸事件
    setupTouchEvents();

    return true;
}

void Game::createInitialBuildings() {
    auto visibleSize = Director::getInstance()->getVisibleSize();

    // 目前只放了一个大本营
    auto townHall = Building::createTownHall();
    townHall->setPosition(visibleSize.width / 2, visibleSize.height / 2);
    this->addChild(townHall);
    _buildings.push_back(townHall);
}

void Game::createSoldierMenu() {
    auto visibleSize = Director::getInstance()->getVisibleSize();

    // 野蛮人兵种选择按钮
    auto barbarianBtn = MenuItemImage::create("Barbarian.png", "Barbarian.png", CC_CALLBACK_1(Game::onBarbarianClicked, this));
    barbarianBtn->setPosition(visibleSize.width * 0.2, 50);
    barbarianBtn->setScale(2.5);

    // 弓箭手兵种选择按钮
    auto archerBtn = MenuItemImage::create("Archer.png", "Archer.png", CC_CALLBACK_1(Game::onArcherClicked, this));
    archerBtn->setPosition(visibleSize.width * 0.4, 50);
    archerBtn->setScale(2.5);

    // 全部添加到菜单
    auto menu = Menu::create(barbarianBtn, archerBtn, nullptr);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 11);
}

void Game::setupTouchEvents() {
    auto listener = EventListenerTouchOneByOne::create();

    listener->onTouchBegan = CC_CALLBACK_2(Game::onTouchBegan, this);
    listener->onTouchEnded = CC_CALLBACK_2(Game::onTouchEnded, this);

    listener->setSwallowTouches(true); // 阻止事件传递

    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

bool Game::onTouchBegan(Touch* touch, Event* event) {
    return true; // 必须返回true，onTouchEnded才会被调用
}

void Game::onTouchEnded(Touch* touch, Event* event) {
    if (!_isPlacingSoldier || _selectedSoldierType == SOLDIER_NONE) {
        return;
    }

    Vec2 touchLocation = touch->getLocation();

    Soldier* newSoldier = nullptr;

    // 确定选择的是哪种兵种
    switch (_selectedSoldierType) {
    case SOLDIER_BARBARIAN:
        newSoldier = Soldier::createBarbarian();
        break;

    case SOLDIER_ARCHER:
        newSoldier = Soldier::createArcher();
        break;

    default:
        return;
    }

    if (newSoldier) {
        newSoldier->setPosition(touchLocation); // 在点击位置放置兵种

        newSoldier->setBuildings(_buildings); // 把场上的建筑集合传递给这个兵，让它选择攻击目标

        this->addChild(newSoldier);
        _soldiers.push_back(newSoldier);
    }
}

// 点击野蛮人按钮后调用
void Game::onBarbarianClicked(Ref* sender) {
    auto button = dynamic_cast<MenuItemImage*>(sender);
    if (!button) return;

    // 如果已经选在野蛮人按钮上，就取消选择
    if (_selectedSoldierButton == button) {
        _isPlacingSoldier = false;
        _selectedSoldierType = SOLDIER_NONE;
        _selectedSoldierButton = nullptr;
    }
    // 否则更新到这个按钮
    else {
        _isPlacingSoldier = true;
        _selectedSoldierType = SOLDIER_BARBARIAN;
        _selectedSoldierButton = button;
    }
}

// 弓箭手按钮，逻辑同上
void Game::onArcherClicked(Ref* sender) {
    auto button = dynamic_cast<MenuItemImage*>(sender);
    if (!button) return;

    if (_selectedSoldierButton == button) {
        _isPlacingSoldier = false;
        _selectedSoldierType = SOLDIER_NONE;
        _selectedSoldierButton = nullptr;
    }
    else {
        _isPlacingSoldier = true;
        _selectedSoldierType = SOLDIER_ARCHER;
        _selectedSoldierButton = button;
    }
}