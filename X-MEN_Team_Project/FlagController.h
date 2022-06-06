#pragma once

// ���Ԃ��������t���O�Ǘ������₷�������₷�����邽�߂̃N���X
class FlagController
{
private:
	bool isFlag;
	float time;

	// �O������Z�b�g
	float maxTime;
public:
	FlagController();
	~FlagController();

	void Initialize();

	void Update(const float DELTA_TIME);

	// �t���O���I���ɂ���
	void FlagOn(bool isTimeReset = true);

	// �t���O�̏�Ԃ�Ԃ�
	bool IsFlag() const;
	float GetTime() const;

	void SetMaxTime(const float VALUE);
};
