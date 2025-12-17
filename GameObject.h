#ifndef  __GAME_OBJECT_H__
#define  __GAME_OBJECT_H__

#include "cocos2d.h"

// 这里是兵种类和建筑类的总基类
class GameObject : public cocos2d::Node {
public:
	CREATE_FUNC(GameObject);

	enum BuildingType {
		BUILDING_NORMAL,
		BUILDING_DEFENSE,
		BUILDING_RESOURCE
	};

	// 初始化
	virtual bool init() override {
		if (!Node::init()) {
			return false;
		}
		this->scheduleUpdate();
		return true;
	}
	
	// 更新行为
	virtual void updateBehavior(float dt) {}
	// 受攻击
	virtual void onHit(float damage) {}
	// 被摧毁
	virtual void onDestroy() {
		_isDestroyed = true;
	}

	// 设置血量
	void setHealth(float health) {
		_health = health;
	}
	// 获取血量
	float getHealth() {
		return _health;
	}
	// 判断存活
	bool isAlive() {
		return _health > 0 && !_isDestroyed;
	}
	// 判断摧毁
	bool isDestroyed() {
		return _isDestroyed;
	}

protected:
	// 当前血量
	float _health = 0.0f;
	// 最大血量
	float _maxHealth = 0.0f;
	// 判断是否被摧毁
	bool _isDestroyed = false;
	// 图片显示
	cocos2d::Sprite* _sprite = nullptr;

private:
	// 这里是逐帧调用所有对象的行为更新系统的代码
	void update(float dt) {
		if (!isAlive()) {
			return;
		}

		updateBehavior(dt);
	}
};

#endif // __GAME_OBJECT_H__