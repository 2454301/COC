#include "GameScene.h"
#include "Soldier.h"
#include "Building.h"

USING_NS_CC;

// 这里是游戏主场景，现在还没有背景，会有兵种选择菜单和初始出来的建筑
// 可选择兵种并放置，兵种会按照逻辑自动攻击

Scene* Game::createScene() {
    return Game::create();
}

Scene* Level_1::createScene() {
    return Level_1::create();
}

bool Game::init() {
    if (!Scene::init()) {
        return false;
    }

    auto map = TMXTiledMap::create("COCMap.tmx");
    map->setAnchorPoint(Vec2::ZERO);
    map->setPosition(Vec2::ZERO);
    map->setScale(0.5);
    this->addChild(map, -2);

    // 初始化建筑
    createInitialBuildings();
    // 创建兵种菜单
    createSoldierMenu();
    // 注册触摸事件
    setupTouchEvents();

    // 启用逐帧更新
    this->scheduleUpdate();

    return true;
}

void Game::update(float dt) {
    // 对合法兵种更新行为
    for (auto soldier : _soldiers) {
        if (soldier && soldier->isAlive() && !soldier->isDestroyed()) {
            soldier->updateBehavior(dt);
        }
    }
    // 对合法建筑更新行为
    for (auto building : _buildings) {
        if (building && building->isAlive() && !building->isDestroyed()) {
            building->setSoldiers(_soldiers);
            building->updateBehavior(dt);
        }
    }
    // 检测：把死亡的兵种从集合中移除
    auto s_it = _soldiers.begin();
    while (s_it != _soldiers.end()) {
        auto soldier = *s_it;
        if (!soldier || soldier->isDestroyed() || !soldier->isAlive()) {
            s_it = _soldiers.erase(s_it);
        }
        else {
            ++s_it;
        }
    }
    // 检测：把摧毁的建筑从集合中移除
    auto b_it = _buildings.begin();
    while (b_it != _buildings.end()) {
        auto building = *b_it;
        if (!building || building->isDestroyed() || !building->isAlive()) {
            if (building) {
                building->removeFromParent();
            }
            b_it = _buildings.erase(b_it);
        }
        else {
            ++b_it;
        }
    }

    if (_buildings.empty()) {
        auto victory = Sprite::create("Victory.png");
        victory->setPosition(Vec2(352, 352));
        this->addChild(victory, 11);
        auto delay = DelayTime::create(1.0f);
        this->runAction(delay);
        auto popScene = CallFunc::create([]() {
            Director::getInstance()->popScene();
            });
        this->runAction(Sequence::create(delay, popScene, nullptr));
    }
}

void Game::createInitialBuildings() {
    // 基类为空实现，由子类重写
}

// 初始化建筑
void Level_1::createInitialBuildings() {

    auto townHall = Building::createTownHall();
    townHall->setPosition(Vec2(352, 352));
    townHall->setScale(0.64);
    this->addChild(townHall);
    _buildings.push_back(townHall);

    auto cannon = Building::createCannon();
    cannon->setPosition(Vec2(400, 400));
    cannon->setScale(0.32);
    this->addChild(cannon);
    _buildings.push_back(cannon);
}

void Game::createSoldierMenu() {
    auto visibleSize = Director::getInstance()->getVisibleSize();

    // 野蛮人兵种选择按钮
    auto barbarianBtn = MenuItemImage::create("Barbarian.png", "Barbarian.png", CC_CALLBACK_1(Game::onBarbarianClicked, this));
    barbarianBtn->setPosition(visibleSize.width * 0.2, 50);
    barbarianBtn->setScale(1.5);

    // 弓箭手兵种选择按钮
    auto archerBtn = MenuItemImage::create("Archer.png", "Archer.png", CC_CALLBACK_1(Game::onArcherClicked, this));
    archerBtn->setPosition(visibleSize.width * 0.4, 50);
    archerBtn->setScale(1.5);

    // 巨人兵种选择按钮
    auto giantBtn = MenuItemImage::create("Giant.png", "Giant.png", CC_CALLBACK_1(Game::onGiantClicked, this));
    giantBtn->setPosition(visibleSize.width * 0.6, 50);
    giantBtn->setScale(1.5);

    // 哥布林兵种选择按钮
    auto goblinBtn = MenuItemImage::create("Goblin.png", "Goblin.png", CC_CALLBACK_1(Game::onGoblinClicked, this));
    goblinBtn->setPosition(visibleSize.width * 0.8, 50);
    goblinBtn->setScale(1.5);

    // 全部添加到菜单
    auto menu = Menu::create(barbarianBtn, archerBtn, giantBtn, goblinBtn, nullptr);
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

// 放置兵种会调用此处函数
void Game::onTouchEnded(Touch* touch, Event* event) {
    if (!_isPlacingSoldier || _selectedSoldierType == SOLDIER_NONE) {
        return;
    }

    Vec2 touchLocation = touch->getLocation(); // 获取点击位置坐标

    Soldier* newSoldier = nullptr;

    // 确定选择的是哪种兵种
    switch (_selectedSoldierType) {
    case SOLDIER_BARBARIAN:
        newSoldier = Soldier::createBarbarian();
        break;

    case SOLDIER_ARCHER:
        newSoldier = Soldier::createArcher();
        break;

    case SOLDIER_GIANT:
        newSoldier = Soldier::createGiant();
        break;

    case SOLDIER_GOBLIN:
        newSoldier = Soldier::createGoblin();
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

// 以下代码重复较多，以后优化，现在先跑起来

// 点击野蛮人按钮后调用
void Game::onBarbarianClicked(Ref* sender) {
    auto button = dynamic_cast<MenuItemImage*>(sender);
    if (!button) return;

    // 如果已经选在野蛮人按钮上，就取消选择，图标向下移动一点
    if (_selectedSoldierButton == button) {
        _selectedSoldierButton->setPosition(cocos2d::Vec2(_selectedSoldierButton->getPositionX(),
                                                          _selectedSoldierButton->getPositionY() - 50));
        _isPlacingSoldier = false;
        _selectedSoldierType = SOLDIER_NONE;
        _selectedSoldierButton = nullptr;
    }
    // 否则更新到这个按钮，图标向上移动一点
    else {
        if (_selectedSoldierButton != nullptr) {
            _selectedSoldierButton->setPosition(cocos2d::Vec2(_selectedSoldierButton->getPositionX(),
                                                              _selectedSoldierButton->getPositionY() - 50));
        }
        _isPlacingSoldier = true;
        _selectedSoldierType = SOLDIER_BARBARIAN;
        _selectedSoldierButton = button;
        _selectedSoldierButton->setPosition(cocos2d::Vec2(_selectedSoldierButton->getPositionX(),
                                                          _selectedSoldierButton->getPositionY() + 50));
    }
}

// 弓箭手按钮，逻辑同上
void Game::onArcherClicked(Ref* sender) {
    auto button = dynamic_cast<MenuItemImage*>(sender);
    if (!button) return;

    if (_selectedSoldierButton == button) {
        _selectedSoldierButton->setPosition(cocos2d::Vec2(_selectedSoldierButton->getPositionX(),
                                                          _selectedSoldierButton->getPositionY() - 50));
        _isPlacingSoldier = false;
        _selectedSoldierType = SOLDIER_NONE;
        _selectedSoldierButton = nullptr;
    }
    else {
        if (_selectedSoldierButton != nullptr) {
            _selectedSoldierButton->setPosition(cocos2d::Vec2(_selectedSoldierButton->getPositionX(),
                                                              _selectedSoldierButton->getPositionY() - 50));
        }
        _isPlacingSoldier = true;
        _selectedSoldierType = SOLDIER_ARCHER;
        _selectedSoldierButton = button;
        _selectedSoldierButton->setPosition(cocos2d::Vec2(_selectedSoldierButton->getPositionX(),
                                                          _selectedSoldierButton->getPositionY() + 50));
    }
}

void Game::onGiantClicked(Ref* sender) {
    auto button = dynamic_cast<MenuItemImage*>(sender);
    if (!button) return;

    if (_selectedSoldierButton == button) {
        _selectedSoldierButton->setPosition(cocos2d::Vec2(_selectedSoldierButton->getPositionX(),
            _selectedSoldierButton->getPositionY() - 50));
        _isPlacingSoldier = false;
        _selectedSoldierType = SOLDIER_NONE;
        _selectedSoldierButton = nullptr;
    }
    else {
        if (_selectedSoldierButton != nullptr) {
            _selectedSoldierButton->setPosition(cocos2d::Vec2(_selectedSoldierButton->getPositionX(),
                _selectedSoldierButton->getPositionY() - 50));
        }
        _isPlacingSoldier = true;
        _selectedSoldierType = SOLDIER_GIANT;
        _selectedSoldierButton = button;
        _selectedSoldierButton->setPosition(cocos2d::Vec2(_selectedSoldierButton->getPositionX(),
            _selectedSoldierButton->getPositionY() + 50));
    }
}

void Game::onGoblinClicked(Ref* sender) {
    auto button = dynamic_cast<MenuItemImage*>(sender);
    if (!button) return;

    if (_selectedSoldierButton == button) {
        _selectedSoldierButton->setPosition(cocos2d::Vec2(_selectedSoldierButton->getPositionX(),
            _selectedSoldierButton->getPositionY() - 50));
        _isPlacingSoldier = false;
        _selectedSoldierType = SOLDIER_NONE;
        _selectedSoldierButton = nullptr;
    }
    else {
        if (_selectedSoldierButton != nullptr) {
            _selectedSoldierButton->setPosition(cocos2d::Vec2(_selectedSoldierButton->getPositionX(),
                _selectedSoldierButton->getPositionY() - 50));
        }
        _isPlacingSoldier = true;
        _selectedSoldierType = SOLDIER_GOBLIN;
        _selectedSoldierButton = button;
        _selectedSoldierButton->setPosition(cocos2d::Vec2(_selectedSoldierButton->getPositionX(),
            _selectedSoldierButton->getPositionY() + 50));
    }
}