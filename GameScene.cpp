#include "GameScene.h"
#include "Soldier.h"
#include "Building.h"

USING_NS_CC;

// 这里是游戏主场景，现在还没有背景，会有兵种选择菜单和初始出来的建筑
// 可选择兵种并放置，兵种会按照逻辑自动攻击

// 创建场景（后续可添加更多关卡）
Scene* Game::createScene() {
    return Game::create();
}

Scene* Level_1::createScene() {
    return Level_1::create();
}

// 初始化
bool Game::init() {
    if (!Scene::init()) {
        return false;
    }

    // 添加地图
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

    // 设置兵种数量标签
    _barbarianLabel = Label::createWithTTF("Barbarian:0", "fonts/Marker Felt.ttf", 18);
    _barbarianLabel->setPosition(Vec2(64, 688));
    this->addChild(_barbarianLabel, 11);
    _archerLabel = Label::createWithTTF("Arhcer:0", "fonts/Marker Felt.ttf", 18);
    _archerLabel->setPosition(Vec2(64, 656));
    this->addChild(_archerLabel, 11);
    _giantLabel = Label::createWithTTF("Giant:0", "fonts/Marker Felt.ttf", 18);
    _giantLabel->setPosition(Vec2(64, 624));
    this->addChild(_giantLabel, 11);
    _goblinLabel = Label::createWithTTF("Goblin:0", "fonts/Marker Felt.ttf", 18);
    _goblinLabel->setPosition(Vec2(64, 592));
    this->addChild(_goblinLabel, 11);

    updateTroopLabels();

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

    // 胜利判断逻辑
    if (_buildings.empty()) { // 场上建筑均被摧毁
        auto victory = Sprite::create("Victory.png");
        victory->setPosition(Vec2(352, 352));
        this->addChild(victory, 11);

        updateVillageTroopCounts();

        auto delay = DelayTime::create(1.0f);
        this->runAction(delay);
        auto popScene = CallFunc::create([]() {
            Director::getInstance()->popScene();
            });
        this->runAction(Sequence::create(delay, popScene, nullptr)); // 播放胜利CG
    }
    
    // 失败判断逻辑
    bool hasAliveSoldiers = false;
    for (auto soldier : _soldiers) {
        if (soldier && soldier->isAlive() && !soldier->isDestroyed()) {
            hasAliveSoldiers = true;
            break;
        }
    }

    bool hasAvailableTroops = (_availableBarbarians > 0 || _availableArchers > 0 ||
        _availableGiants > 0 || _availableGoblins > 0);

    if (!hasAliveSoldiers && !hasAvailableTroops) { // 场上兵种均阵亡，且可使用兵种已耗尽
        auto defeat = Sprite::create("Defeat.png");
        defeat->setPosition(Vec2(352, 352));
        this->addChild(defeat, 11);

        updateVillageTroopCounts();

        auto delay = DelayTime::create(1.0f);
        auto popScene = CallFunc::create([]() {
            Director::getInstance()->popScene();
            });
        this->runAction(Sequence::create(delay, popScene, nullptr)); // 播放战败CG
    }
}

void Game::createInitialBuildings() {
    // 基类为空实现，由子类重写
}

// 更新兵种数量标签
void Game::updateTroopLabels() {
    _barbarianLabel->setString(StringUtils::format("Barbarian:%d", _availableBarbarians));
    _archerLabel->setString(StringUtils::format("Archer:%d", _availableArchers));
    _giantLabel->setString(StringUtils::format("Giant:%d", _availableGiants));
    _goblinLabel->setString(StringUtils::format("Goblin:%d", _availableGoblins));
}

// 初始化建筑
// 第一关的建筑布局
void Level_1::createInitialBuildings() {

    auto townHall = Building::createTownHall();
    townHall->setPosition(Vec2(368, 368));
    this->addChild(townHall);
    _buildings.push_back(townHall);

    auto cannon = Building::createCannon();
    cannon->setPosition(Vec2(448, 448));
    this->addChild(cannon);
    _buildings.push_back(cannon);
}

// 创建兵种选择菜单
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

// 注册触摸事件
void Game::setupTouchEvents() {
    auto listener = EventListenerTouchOneByOne::create();

    listener->onTouchBegan = CC_CALLBACK_2(Game::onTouchBegan, this);
    listener->onTouchEnded = CC_CALLBACK_2(Game::onTouchEnded, this);

    listener->setSwallowTouches(true); // 阻止事件传递

    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

// 点击开始
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
    bool canPlace = false;

    // 确定选择的是哪种兵种
    switch (_selectedSoldierType) {
    case SOLDIER_BARBARIAN:
        if (_availableBarbarians > 0) {
            newSoldier = Soldier::createBarbarian();
            canPlace = true;
            _availableBarbarians--;
            updateVillageTroopCounts();
        }
        break;

    case SOLDIER_ARCHER:
        if (_availableArchers > 0) {
            newSoldier = Soldier::createArcher();
            canPlace = true;
            _availableArchers--;
            updateVillageTroopCounts();
        }
        break;

    case SOLDIER_GIANT:
        if (_availableGiants > 0) {
            newSoldier = Soldier::createGiant();
            canPlace = true;
            _availableGiants--;
            updateVillageTroopCounts();
        }
        break;

    case SOLDIER_GOBLIN:
        if (_availableGoblins > 0) {
            newSoldier = Soldier::createGoblin();
            canPlace = true;
            _availableGoblins--;
            updateVillageTroopCounts();
        }
        break;

    default:
        return;
    }

    if (newSoldier && canPlace) {
        newSoldier->setPosition(touchLocation); // 在点击位置放置兵种

        newSoldier->setBuildings(_buildings); // 把场上的建筑集合传递给这个兵，让它选择攻击目标

        this->addChild(newSoldier);
        _soldiers.push_back(newSoldier);

        updateTroopLabels(); // 更新
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