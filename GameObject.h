#ifndef  __GAME_OBJECT_H__
#define  __GAME_OBJECT_H__

#include "cocos2d.h"

// 这里是兵种类和建筑类的总基类
class GameObject : public cocos2d::Node {
public:
	CREATE_FUNC(GameObject);

	virtual bool init() override {
		if (!Node::init()) {
			return false;
		}
		this->scheduleUpdate();
		return true;
	}
	
	virtual void updateBehavior(float dt) {}
	virtual void onHit(float damage) {}
	virtual void onDestroy() {}

	void setHealth(float health) {
		_health = health;
	}
	float getHealth() {
		return _health;
	}
	bool isAlive() {
		return _health > 0;
	}

protected:
	float _health = 0.0f;
	float _maxHealth = 0.0f;
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