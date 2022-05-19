#pragma once

namespace GameManageData
{
	// ヒットストップするフレーム数
	const int HITSTOP_FRAME = 20;
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

public:

	static GameManager* GetInstance();

	void Initialize();
	void Destroy();

	// ヒットストップ中ならtrue
	bool IsHitStop();
	void Update();

	// セッター　
	void SetCanGameClear(bool flg);
	void SetCanGameOver(bool flg);

	void SetHitStop(bool flg);

	// ゲッター
	bool GetCanGameClear() const;
	bool GetCanGameOver() const;

private:

	GameManager() = default;
	~GameManager() = default;

	// 代入コピーの禁止
	void operator=(const GameManager& obj) = delete;
	GameManager(const GameManager& obj) = delete;

};

