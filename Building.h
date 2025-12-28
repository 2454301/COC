#ifndef __BUILDING_H__
#define __BUILDING_H__

#include "GameObject.h"
#include <vector>

class Soldier;

class Building : public GameObject {
public:
    // 静态创建方法
    static Building* createTownHall();
    static Building* createCannon();
    static Building* createGoldMine();
    static Building* createElixirCollector();
    static Building* createArmyCamp();
    static Building* createBarracks();

    // 设置攻击目标
    void attack(GameObject* target);
    void stop();

    // Getter 方法
    int getSize() { return _size; }
    BuildingType getType() { return _type; }

    // 传入兵种集合
    void setSoldiers(const std::vector<Soldier*>& soldiers) {
        _availableSoldiers = soldiers;
    }

    // 寻找第一个进入范围的兵种
    GameObject* findFirstSoldier();

    // 行为逻辑重写
    void updateBehavior(float dt) override;
    void onHit(float damage) override;
    void onDestroy() override;

private:
    // 初始化私有方法
    bool initTownHall();
    bool initCannon();
    bool initGoldMine();
    bool initElixirCollector();
    bool initArmyCamp();
    bool initBarracks();

    // --- 血条相关实现（新增加的方法） ---
    void setupHealthBar();  // 初始化血条
    void updateHealthBar(); // 更新血条

    // --- 血条组件变量 ---
    cocos2d::ProgressTimer* _healthBar = nullptr;
    cocos2d::Sprite* _healthBarBg = nullptr;

    // --- 建筑属性 ---
    int _level = 1;
    int _size = 0;
    float _attack = 0.0f;
    float _range = 0.0f;
    BuildingType _type = BUILDING_NORMAL;

    // --- 战斗状态 ---
    bool _isAttacking = false;
    GameObject* _target = nullptr;
    float _attackTimer = 0.0f;

    // 兵种集合
    std::vector<Soldier*> _availableSoldiers;
};

#endif // __BUILDING_H__