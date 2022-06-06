#pragma once

// 時間をつかったフラグ管理をしやすくかつ見やすくするためのクラス
class FlagController
{
private:
	bool isFlag;
	float time;

	// 外部からセット
	float maxTime;
public:
	FlagController();
	~FlagController();

	void Initialize();

	void Update(const float DELTA_TIME);

	// フラグをオンにする
	void FlagOn(bool isTimeReset = true);

	// フラグの状態を返す
	bool IsFlag() const;
	float GetTime() const;

	void SetMaxTime(const float VALUE);
};
