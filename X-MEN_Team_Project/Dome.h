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

class Dome : public MelLib::GameObject
{
private:
	float liveTime;
	int level;
	float alpha;
	float easingValue;
	bool isUse;
private:
	void FadeIn(const float FRAME_TIME);
	void FadeOut(const float FRAME_TIME);
	float EaseOutQuint(float s, float e, float t);
public:
	Dome();
	~Dome();

	void Update();
	void Draw();

	// ゲームオブジェクトマネージャーから削除するキューに追加
	void EraseFunc();

#pragma region Getter
	bool IsUse();
#pragma endregion

#pragma region Setter
	void SetLevel(const int value);
#pragma endregion
};