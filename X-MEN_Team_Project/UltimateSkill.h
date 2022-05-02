#pragma once

namespace UltimateSkillInfo
{
	// �Q�[�W�����l
	const int INITIALIZE_VALUE = 0;

	// �Q�[�W�̍ő�l
	const int MAX_VALUE = 100;

	// ���x�����オ�邽�߂ɕK�v�ȗ�
	const int LEVEL_UP_VALUE = 25;

	// �������x��
	const int INITIALIZE_LEVEL = 1;

	// �ő僌�x��
	const int MAX_LEVEL = 4;
}

class UltimateSkill
{
private:
	int value;
	int level;
	bool isUsingSkill;
private:
	// ���Q�[�W�ʂ��烌�x�����v�Z
	void CalcLevel();
public:
	UltimateSkill();
	~UltimateSkill();

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
