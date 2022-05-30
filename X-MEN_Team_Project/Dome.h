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
private:
	void FadeIn(const float FRAME_TIME);
	void FadeOut(const float FRAME_TIME);
	float EaseOutQuint(float s, float e, float t);
public:
	Dome();
	~Dome();

	void Update();
	void Draw();

	// �Q�[���I�u�W�F�N�g�}�l�[�W���[����폜����L���[�ɒǉ�
	void EraseFunc();

#pragma region Getter
	bool IsUse();
#pragma endregion

#pragma region Setter
	void SetLevel(const int value);
#pragma endregion
};