#pragma once

namespace GameManageData
{
	// �q�b�g�X�g�b�v����t���[����
	const int HITSTOP_FRAME = 5;

	// �f�t�H���g�Q�[��������
	const float DEFAULT_GAME_TIME = 1;
	// �Q�[�������Ԃ�ς���ۂɌ��̎��Ԃ���ݒ肵�������Ԃɐ؂�ւ���܂łǂꂭ�炢�̎��Ԃ����邩(�b�w��)
	const float CHANGE_GAME_TIME_SEC = 2;
}

// �V���O���g���p�^�[��
class GameManager
{
private:

	// �C���X�^���X
	static GameManager* instance;

	// �q�b�g�X�g�b�v�p�J�E���g
	int hitstopCount;
	// �q�b�g�X�g�b�v�����ǂ���
	bool isHitStopNow;

	bool canGameClear;
	bool canGameOver;

	// 0 ~ 1 �Q�[��������
	float beforeSetGameTime;
	float currentSetGameTime;
	float gameTime;
	float changeGameTimeEasingValue;
	bool isChangeGameTime;

public:

	static GameManager* GetInstance();

	void Initialize();
	void Destroy();

	// �q�b�g�X�g�b�v���Ȃ�true
	bool IsHitStop();
	void Update();
	void UpdateChangeGameTime();

	// �Z�b�^�[�@
	void SetCanGameClear(bool flg);
	void SetCanGameOver(bool flg);

	void SetHitStop(bool flg);

	// �Q�[���������Ԃ�ݒ肷��֐��Q
	void SetDefaultGameTime();
	// 0 ~ 1
	void SetGameTime(float setTime);

	// �Q�b�^�[
	bool GetCanGameClear() const;
	bool GetCanGameOver() const;

	// �Q�[�������Ԃ��擾�A���̒l���ړ��x�N�g���ɂ����邱�ƂŃQ�[�������Ԃ��ς���Ă��i���Ƃ��΃X���[�ɂȂ��Ă��j
	// ���̎��Ԃɂ������ړ��ʂɂȂ�
	float GetGameTime() const;
private:

	GameManager() = default;
	~GameManager() = default;

	// ����R�s�[�̋֎~
	void operator=(const GameManager& obj) = delete;
	GameManager(const GameManager& obj) = delete;

	void ChangeGameTime();
};

