#pragma once
#include <GameObject.h>

namespace DomeInfo
{
	// �t�F�[�h�C���̎���
	const float START_TIME = 1.25f;
	// �t�F�[�h�A�E�g�̎���
	const float END_TIME = 1.25f;
	// ���݂��Ă����鎞��
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
	bool oldIsUse;
private:
	void FadeIn(const float FRAME_TIME);
	void FadeOut(const float FRAME_TIME);
	float EaseOutQuint(float s, float e, float t);

	// ���x�����Z�b�g�����炱�̊֐��Ń��x�����Ƃ̃T�C�Y���v�Z����
	void CalcSize();
public:
	Dome();
	~Dome();

	void Initialize();
	void Update();
	void Draw();

#pragma region Getter
	bool IsUse();
	bool IsEndTrigger();
#pragma endregion

#pragma region Setter
	void SetLevel(const int value);
#pragma endregion
};