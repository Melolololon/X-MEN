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

// �h�[���I�����
// �X���[���[�V���������b�Ԏ��������邩��ێ����Ă���l�[���X�y�[�X
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

	// �h�[�����I�����Ă���̌o�ߎ��� 0 ~ 1
	float endElapsedTime;
	bool oldIsEnd;
	bool isEnd;
private:
	void FadeIn(const float FRAME_TIME);
	void FadeOut(const float FRAME_TIME);

	// 0 ~ 1
	float EaseOutQuint(float t);

	// ���x�����Z�b�g�����炱�̊֐��Ń��x�����Ƃ̃T�C�Y���v�Z����
	void CalcSize();

	// �h�[���I����̌㏈���A�I�����Ă���̌o�ߎ��Ԃ��v�Z�����肷��
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

	// �h�[�����������㏈�����I�����������擾�ł���֐�
	bool IsPostProcessEndTrigger();
#pragma endregion

#pragma region Setter
	void SetLevel(const int value);
#pragma endregion
};