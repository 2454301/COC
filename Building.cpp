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

// 大本营初始化
bool Building::initTownHall() {
	if (!GameObject::init()) {
		return false;
	}

	_health = _maxHealth = 500.0f; // 500血量

	_sprite = cocos2d::Sprite::create("TownHall.png"); // 这里需要一个图片文件
	// 添加进场景并调整一些参数
	this->addChild(_sprite);
	_sprite->setPosition(cocos2d::Vec2::ZERO);
	_sprite->setScale(2);
	return true;
}

// 加农炮还没做...
bool Building::initCannon() {
	if (!GameObject::init()) {
		return false;
	}

	// ...
	return true;
}

// 升级系统，暂时用不到
void Building::upgrade() {
	if (!_isUpgrading) {
		_isUpgrading = true;
		_upgradeTimer = 10.0f;
	}
}

// 行为更新系统，也暂时用不到
void Building::updateBehavior(float dt) {
	if (_isUpgrading) {
		_upgradeTimer -= dt;
		if (_upgradeTimer <= 0) {
			_isUpgrading = false;
			_level++;
		}
	}
}

// 受攻击判定
void Building::onHit(float damage) {
	_health -= damage;
	if (!isAlive()) {
		onDestroy();
	}
}

// 被摧毁判定，目前使用简单移除，后期可考虑使用动画
void Building::onDestroy() {
	this->removeFromParent();
}