#ifndef  __GAME_OBJECT_H__
#define  __GAME_OBJECT_H__

#include "cocos2d.h"

class GameObject : public cocos2d::Node {
public:
	CREATE_FUNC(GameObject);

	enum BuildingType {
		BUILDING_NORMAL,
		BUILDING_DEFENSE,
		BUILDING_RESOURCE
	};

	virtual bool init() override {
		if (!Node::init()) {
			return false;
		}
		this->scheduleUpdate();
		return true;
	}

	virtual void updateBehavior(float dt) {}
	virtual void onHit(float damage) {}
	virtual void onDestroy() {
		_isDestroyed = true;
	}

	void setHealth(float health) {
		_health = health;
	}
	float getHealth() {
		return _health;
	}
	bool isAlive() {
		return _health > 0 && !_isDestroyed;
	}
	bool isDestroyed() {
		return _isDestroyed;
	}

protected:
	float _health = 0.0f;
	float _maxHealth = 0.0f;
	bool _isDestroyed = false;
	cocos2d::Sprite* _sprite = nullptr;

private:
	void update(float dt) {
		if (!isAlive()) {
			return;
		}

		updateBehavior(dt);
	}
};

#endif // __GAME_OBJECT_H__