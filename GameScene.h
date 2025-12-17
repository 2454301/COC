#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "Soldier.h"
#include "Building.h"

// 显示游戏主场景
class Game : public cocos2d::Scene {
public:
    // 场景创建
    static cocos2d::Scene* createScene();
    CREATE_FUNC(Game);

    // 初始化界面
    virtual bool init();

    // 枚举类，便于选择
    enum SoldierType {
        SOLDIER_NONE,
        SOLDIER_BARBARIAN,
        SOLDIER_ARCHER,
        SOLDIER_GIANT
    };

private:
    // 触摸开始和结束检测函数
    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);

    // 创建初始建筑
    void createInitialBuildings();
    // 创建兵种菜单
    void createSoldierMenu();
    // 注册触摸事件
    void setupTouchEvents();
    // 多兵种点击检测
    void onBarbarianClicked(cocos2d::Ref* sender);
    void onArcherClicked(cocos2d::Ref* sender);
    void onGiantClicked(cocos2d::Ref* sender);

    // 建筑集合
    std::vector<Building*> _buildings;
    // 兵种集合
    std::vector<Soldier*> _soldiers;
    // 兵种选择按钮
    cocos2d::MenuItemImage* _selectedSoldierButton = nullptr;
    // 确定是否放置兵种
    bool _isPlacingSoldier = false;
    // 确定放置的是哪种兵种
    SoldierType _selectedSoldierType = SOLDIER_NONE;

    // 逐帧调用，更新行为
    void update(float dt) override;
};

#endif // __GAME_SCENE_H__