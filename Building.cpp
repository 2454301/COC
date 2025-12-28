#include "Soldier.h"
#include "Building.h"

Building* Building::createTownHall() {
    Building* building = new Building();
    if (building && building->initTownHall()) {
        building->autorelease();
        return building;
    }
    delete building;
    return nullptr;
}

bool Building::initTownHall() {
    if (!GameObject::init()) {
        return false;
    }
    _health = _maxHealth = 500.0f;
    _type = BUILDING_RESOURCE;
    _size = 96;

    _sprite = cocos2d::Sprite::create("TownHall.png");
    if (_sprite) {
        this->addChild(_sprite);
        _sprite->setPosition(cocos2d::Vec2::ZERO);
    }

    setupHealthBar(); // ✨ 调用血条初始化
    return true;
}

Building* Building::createCannon() {
    Building* building = new Building();
    if (building && building->initCannon()) {
        building->autorelease();
        return building;
    }
    delete building;
    return nullptr;
}

bool Building::initCannon() {
    if (!GameObject::init()) {
        return false;
    }
    _health = _maxHealth = 300.0f;
    _type = BUILDING_DEFENSE;
    _attack = 40.0f;
    _range = 250.0f;
    _size = 64;

    _sprite = cocos2d::Sprite::create("COCCannon.png");
    if (_sprite) {
        this->addChild(_sprite);
        _sprite->setPosition(cocos2d::Vec2::ZERO);
    }

    setupHealthBar(); // ✨ 调用血条初始化
    return true;
}

Building* Building::createArmyCamp() {
    Building* building = new Building();
    if (building && building->initArmyCamp()) {
        building->autorelease();
        return building;
    }
    delete building;
    return nullptr;
}

bool Building::initArmyCamp() {
    if (!GameObject::init()) {
        return false;
    }
    _health = _maxHealth = 300.0f;
    _size = 64;
    _sprite = cocos2d::Sprite::create("ArmyCamp.png");
    if (_sprite) {
        this->addChild(_sprite);
        _sprite->setPosition(cocos2d::Vec2::ZERO);
    }
    setupHealthBar();
    return true;
}

Building* Building::createBarracks() {
    Building* building = new Building();
    if (building && building->initBarracks()) {
        building->autorelease();
        return building;
    }
    delete building;
    return nullptr;
}

bool Building::initBarracks() {
    if (!GameObject::init()) {
        return false;
    }
    _health = _maxHealth = 300.0f;
    _size = 96;
    _sprite = cocos2d::Sprite::create("COCBarracks.png");
    if (_sprite) {
        this->addChild(_sprite);
        _sprite->setPosition(cocos2d::Vec2::ZERO);
    }
    setupHealthBar();
    return true;
}

Building* Building::createGoldMine() {
    Building* building = new Building();
    if (building && building->initGoldMine()) {
        building->autorelease();
        return building;
    }
    delete building;
    return nullptr;
}

bool Building::initGoldMine() {
    if (!GameObject::init()) {
        return false;
    }
    _health = _maxHealth = 300.0f;
    _type = BUILDING_RESOURCE;
    _size = 64;
    _sprite = cocos2d::Sprite::create("COCGold Mine.png");
    if (_sprite) {
        this->addChild(_sprite);
        _sprite->setPosition(cocos2d::Vec2::ZERO);
    }
    setupHealthBar();
    return true;
}

Building* Building::createElixirCollector() {
    Building* building = new Building();
    if (building && building->initElixirCollector()) {
        building->autorelease();
        return building;
    }
    delete building;
    return nullptr;
}

bool Building::initElixirCollector() {
    if (!GameObject::init()) {
        return false;
    }
    _health = _maxHealth = 300.0f;
    _type = BUILDING_RESOURCE;
    _size = 64;
    _sprite = cocos2d::Sprite::create("COCElixir Collector.png");
    if (_sprite) {
        this->addChild(_sprite);
        _sprite->setPosition(cocos2d::Vec2::ZERO);
    }
    setupHealthBar();
    return true;
}

void Building::setupHealthBar() {
    if (!_sprite) return;

    // 1. 创建血条背景
    _healthBarBg = cocos2d::Sprite::create("HealthBar.png");
    if (_healthBarBg) {
        _healthBarBg->setColor(cocos2d::Color3B::BLACK);
        _healthBarBg->setScale(0.3f);
        // 根据 _size 动态设置高度，防止遮挡建筑
        _healthBarBg->setPosition(cocos2d::Vec2(0, _size / 2 + 10));
        this->addChild(_healthBarBg, 10);

        // 2. 创建进度条
        auto barSprite = cocos2d::Sprite::create("HealthBar.png");
        if (barSprite) {
            _healthBar = cocos2d::ProgressTimer::create(barSprite);
            _healthBar->setType(cocos2d::ProgressTimer::Type::BAR);
            _healthBar->setMidpoint(cocos2d::Vec2(0, 0.5f));
            _healthBar->setBarChangeRate(cocos2d::Vec2(1, 0));
            _healthBar->setPercentage(100.0f);
            _healthBar->setPosition(_healthBarBg->getPosition());
            _healthBar->setScale(_healthBarBg->getScale());
            this->addChild(_healthBar, 11);
        }
    }
    if (_healthBarBg) _healthBarBg->setVisible(false);
    if (_healthBar) _healthBar->setVisible(false);
}

void Building::updateHealthBar() {
    if (_healthBar) {
        float percentage = (_health / _maxHealth) * 100.0f;
        _healthBar->setPercentage(percentage);
        if (percentage < 30.0f) {
            _healthBar->getSprite()->setColor(cocos2d::Color3B::RED);
        }
    }
}

GameObject* Building::findFirstSoldier() {
    if (_availableSoldiers.empty() || _type != BUILDING_DEFENSE) {
        return nullptr;
    }
    cocos2d::Vec2 myPos = this->getPosition();
    for (auto soldier : _availableSoldiers) {
        if (soldier && soldier->isAlive()) {
            float distance = myPos.distance(soldier->getPosition());
            if (distance <= _range) {
                return soldier;
            }
        }
    }
    return nullptr;
}

void Building::attack(GameObject* target) {
    if (target == nullptr || !target->isAlive()) return;
    _target = target;
    _isAttacking = true;
    _attackTimer = 0.0f;
}

void Building::stop() {
    _isAttacking = false;
    _target = nullptr;
}

void Building::updateBehavior(float dt) {
    if (!isAlive()) return;
    if (_target == nullptr || !_target->isAlive()) {
        _target = findFirstSoldier();
        if (_target) attack(_target);
        else {
            stop();
            return;
        }
    }
    if (_isAttacking && _target && _target->isAlive()) {
        _attackTimer += dt;
        if (_attackTimer >= 1.0f) {
            _target->onHit(_attack);
            _attackTimer = 0.0f;
        }
    }
}

void Building::onHit(float damage) {
    _health -= damage;

    // 显示血条
    if (_healthBarBg) _healthBarBg->setVisible(true);
    if (_healthBar) _healthBar->setVisible(true);

    updateHealthBar();

    // ✨ 停止之前的“隐藏任务”，重新计时 3 秒后隐藏
    this->unschedule("hide_hp_key");
    this->scheduleOnce([this](float dt) {
        if (_healthBarBg) _healthBarBg->setVisible(false);
        if (_healthBar) _healthBar->setVisible(false);
        }, 3.0f, "hide_hp_key");

    if (!isAlive()) {
        onDestroy();
    }
}

void Building::onDestroy() {
    GameObject::onDestroy();
    auto removeAction = cocos2d::CallFunc::create([this]() {
        if (this->getParent()) {
            this->removeFromParent();
        }
        });
    auto sequence = cocos2d::Sequence::create(
        cocos2d::DelayTime::create(0.1f),
        removeAction,
        nullptr
    );
    this->runAction(sequence);
}