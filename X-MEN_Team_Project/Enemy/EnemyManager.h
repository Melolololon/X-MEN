#pragma once
#include<memory>
#include<time.h>

#include"../Enemy/Enemy.h"
#include"../Enemy//FollowEnemy.h"
#include"../Enemy/BarrierEnemy.h"


namespace EnemyManage
{
	// 最初にスポーンする数
	const float SPAWN_ENEMY_NUM = 5;
	// スポーンする間隔
	const float SPAWN_TIME = 2000.0f;
}

// シングルトンパターン
class EnemyManager
{
private:

	static EnemyManager* instance;

	// 追いかける敵の配列
	std::vector<std::shared_ptr<FollowEnemy>> followEnemies;
	// バリア持ちの敵の配列
	std::vector <std::shared_ptr<BarrierEnemy>> barrierEnemies;

	// 時間計測用
	clock_t timerStart;

public:

	static EnemyManager* GetInstance();

	// 初期配置用
	void Initialize();
	void Update();
	
	void Destroy();

	// セッター

	void SetPlayerPos(const MelLib::Vector3& pos);

private:

	EnemyManager() = default;
	~EnemyManager() = default;

	// 代入、コピーの禁止
	void operator=(const EnemyManager& obj) = delete;
	EnemyManager(const EnemyManager& obj) = delete;

	// 敵の出現
	void PopFollowEnemyInitialize();
	// 時間での敵の出現
	void PopEnemyTime();

};

