#ifndef __BUILDING_H__
#define __BUILDING_H__

#include "GameObject.h"

class Building : public GameObject {
public:
	// 建筑的创建
	static Building* createTownHall();
	static Building* createCannon(); // 加农炮的还没写

	// 升级系统（未完成）
	void upgrade();
	bool isUpgrading() {
		return _isUpgrading;
	}

	// 行为更新系统（未完成）
	void updateBehavior(float dt) override;
	// 受攻击和被摧毁
	void onHit(float damage) override;
	void onDestroy() override;

private:
	// 初始化
	bool initTownHall();
	bool initCannon();

	int _level = 1; // 默认1级
	float _goldProduction; // 资源建筑可能用上
	float _defense; // 防御力（？）不确定游戏里有没有这一项，先写着

	bool _isUpgrading = false;
	float _upgradeTimer = 0.0f;
};

#endif // __BUILDING_H__