#pragma once
#include <GameObject.h>

namespace DomeInfo
{
	// フェードインの時間
	const float START_TIME = 1.25f;
	// フェードアウトの時間
	const float END_TIME = 1.25f;
	// 存在していられる時間
	const float MAX_TIME = 4;
}

// ドーム終了後に
// スローモーションを何秒間持続させるかを保持しているネームスペース
namespace DomeEndSlowMotionInfo
{
	const float TIME = 1;
}

class Dome : public MelLib::GameObject
{
private:
	float liveTime;
	int level;
	float alpha;
	float easingValue;
	bool isUse;
	bool oldIsUse;

	// ドームが終了してからの経過時間 0 ~ 1
	float endElapsedTime;
	bool oldIsEnd;
	bool isEnd;
private:
	void FadeIn(const float FRAME_TIME);
	void FadeOut(const float FRAME_TIME);

	// 0 ~ 1
	float EaseOutQuint(float t);

	// レベルをセットしたらこの関数でレベルごとのサイズを計算する
	void CalcSize();

	// ドーム終了後の後処理、終了してからの経過時間を計算したりする
	void EndPostProcess(const float FRAME_TIME);
public:
	Dome();
	~Dome();

	void Initialize();
	void Update();
	void Draw();

#pragma region Getter
	bool IsUse();
	bool IsEndTrigger();

	// ドームが消えた後処理が終了したかを取得できる関数
	bool IsPostProcessEndTrigger();
#pragma endregion

#pragma region Setter
	void SetLevel(const int value);
#pragma endregion
};