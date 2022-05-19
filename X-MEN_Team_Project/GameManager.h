#pragma once

namespace GameManageData
{
	// �q�b�g�X�g�b�v����t���[����
	const int HITSTOP_FRAME = 20;
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

public:

	static GameManager* GetInstance();

	void Initialize();
	void Destroy();

	// �q�b�g�X�g�b�v���Ȃ�true
	bool IsHitStop();
	void Update();

	// �Z�b�^�[�@
	void SetCanGameClear(bool flg);
	void SetCanGameOver(bool flg);

	void SetHitStop(bool flg);

	// �Q�b�^�[
	bool GetCanGameClear() const;
	bool GetCanGameOver() const;

private:

	GameManager() = default;
	~GameManager() = default;

	// ����R�s�[�̋֎~
	void operator=(const GameManager& obj) = delete;
	GameManager(const GameManager& obj) = delete;

};

