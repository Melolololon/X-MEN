#pragma once
#include <Sprite2D.h>

namespace UltimateSkillInfo
{
	// �Q�[�W�����l
	const float INITIALIZE_VALUE = 0.0f;

	// �Q�[�W�̍ő�l
	const float MAX_VALUE = 100.0f;

	// ���x�����オ�邽�߂ɕK�v�ȗ�
	const float LEVEL_UP_VALUE = 25.0f;

	// �������x��
	const float INITIALIZE_LEVEL = 1.0f;

	// �ő僌�x��
	const float MAX_LEVEL = 4.0f;
}

namespace UltimateSkillGaugeDrawInfo
{
	// window�̃T�C�Y�ɑ΂���䗦 0 ~ 100
	const MelLib::Vector2 SIZE_PERCENT = MelLib::Vector2(10, 2);

	// �`��F
	const MelLib::Color FRONT_COLOR = MelLib::Color(255, 0, 0, 255);
	const MelLib::Color BACK_COLOR = MelLib::Color(64, 64, 64, 255);

	const MelLib::Vector2 DRAW_POSITION = MelLib::Vector2(20, 125);
}

class UltimateSkill
{
private:
	int value;
	int level;
	bool isUsingSkill;

	// �Q�[�W�̗ʂ�`�������p
	MelLib::Sprite2D frontGauge;

	// �Q�[�W�̍ő�l��`�悷��p
	MelLib::Sprite2D backGauge;

	MelLib::Vector2 frontDrawSize;
	MelLib::Vector2 backDrawSize;

	MelLib::Vector2 drawPosition;
	MelLib::Color drawColor;
private:
	// ���Q�[�W�ʂ��烌�x�����v�Z
	void CalcLevel();

	// window�̃T�C�Y����`�悷��T�C�Y���v�Z����
	void CalcSize(const MelLib::Vector2& windowSize);

	// window�̃T�C�Y����`�悷��ʒu���v�Z����
	void CalcPosition(const MelLib::Vector2& windowSize);

	void CalcColor();
public:
	UltimateSkill();
	~UltimateSkill();

	void Update();
	void Draw();

	// ���̂Ƃ���l���g�p�������ƂɃ��x�����Čv�Z
	void Use();

	// �Q�[�W�̒l�𑝂₷
	void AddValue(int addValue);

#pragma region Getter
	// �X�L�����g�p���Ă��邩�擾
	bool GetIsUsingSkill() const;
#pragma endregion

#pragma region Setter
	// �X�L�����g�p���Ă���t���O������������
	void SetIsUsingSkill(bool flag);
#pragma endregion
};
