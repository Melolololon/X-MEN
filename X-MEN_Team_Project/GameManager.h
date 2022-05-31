#pragma once

namespace GameManageData
{
	// ヒットストップするフレーム数
	const int HITSTOP_FRAME = 5;

	// デフォルトゲーム内時間
	const float DEFAULT_GAME_TIME = 1;
	// ゲーム内時間を変える際に元の時間から設定したい時間に切り替えるまでどれくらいの時間かけるか(秒指定)
	const float CHANGE_GAME_TIME_SEC = 2;
}

// シングルトンパターン
class GameManager
{
private:

	// インスタンス
	static GameManager* instance;

	// ヒットストップ用カウント
	int hitstopCount;
	// ヒットストップ中かどうか
	bool isHitStopNow;

	bool canGameClear;
	bool canGameOver;

	// 0 ~ 1 ゲーム内時間
	float beforeSetGameTime;
	float currentSetGameTime;
	float gameTime;
	float changeGameTimeEasingValue;
	bool isChangeGameTime;

public:

	static GameManager* GetInstance();

	void Initialize();
	void Destroy();

	// ヒットストップ中ならtrue
	bool IsHitStop();
	void Update();
	void UpdateChangeGameTime();

	// セッター　
	void SetCanGameClear(bool flg);
	void SetCanGameOver(bool flg);

	void SetHitStop(bool flg);

	// ゲーム無い時間を設定する関数群
	void SetDefaultGameTime();
	// 0 ~ 1
	void SetGameTime(float setTime);

	// ゲッター
	bool GetCanGameClear() const;
	bool GetCanGameOver() const;

	// ゲーム内時間を取得、この値を移動ベクトルにかけることでゲーム内時間が変わっても（たとえばスローになっても）
	// その時間にあった移動量になる
	float GetGameTime() const;
private:

	GameManager() = default;
	~GameManager() = default;

	// 代入コピーの禁止
	void operator=(const GameManager& obj) = delete;
	GameManager(const GameManager& obj) = delete;

	void ChangeGameTime();
};

