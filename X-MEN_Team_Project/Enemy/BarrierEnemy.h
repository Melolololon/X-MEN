#pragma once
#include"Enemy.h"
#include"../EnemyBarrier.h"

namespace BarrierEnemyStatus
{
	const float MAX_HP = 4;

	const float DISTANCE_TO_PLAYER = 500;

	const int CHANGE_POSE_FRAME = 120;
}

class BarrierEnemy : public Enemy
{
private:

	// �{�[���̕����x�N�g�� ����g�p���ă��f���̌�����ύX����
	//MelLib::Vector3 ballDir;

	// �o���A
	std::shared_ptr<EnemyBarrier> pBarrier;
	// ���ʃx�N�g��
	MelLib::Vector3 frontDir;

	std::array<MelLib::Vector3, BarrierEnemyStatus::CHANGE_POSE_FRAME> ballDir;

	//�o���A�\�[�g�pint
	int ballCurrentNum;//�ŐV�̃{�[���̏�������z��ԍ�
	int ballBeforeNum;//�o���A�ɓ���鎞�̃t���[���̔z��ԍ�
	bool firstCountflg;//�ŏ��A�z������؂�܂ł̊֐�

	std::shared_ptr<GameObject> refBallObject;
private:

	void Move() override;

	void ChangePose();

	// �w��t���[�����̔z��̐����p�@UPDATE�O�Ɏg�p
	void BallDirSort();

	// �ŏ���Update���ɂ̂݃{�[���̎Q�Ƃ��擾����
	void RefBallObject();

	void SetRandomPosition();
public:
	BarrierEnemy();
	~BarrierEnemy() {}

	// �X�V
	void Update()override;

	//�`��
	void Draw()override;

	/// <summary>
	/// �����������ɌĂ΂��֐�
	/// </summary>
	/// <param name="object">�Փ˂����I�u�W�F�N�g�̎Q��</param>
	/// <param name="shapeType">����ƏՓ˂��������̓����蔻��̌`��</param>
	/// <param name="shapeName">����ƏՓ˂��������̓����蔻��̖��O</param>
	/// <param name="hitObjShapeType">�Փ˂����I�u�W�F�N�g�̓����蔻��̌`��</param>
	/// <param name="hitShapeName">�Փ˂����I�u�W�F�N�g�̔���̖��O</param>
	void Hit
	(
		const GameObject& object,
		const MelLib::ShapeType3D shapeType,
		const std::string& shapeName,
		const MelLib::ShapeType3D hitObjShapeType,
		const std::string& hitShapeName
	)override;


	// �Z�b�^�[
	void SetBallDir(const MelLib::Vector3& pos);
	void SetBarrier(std::shared_ptr<EnemyBarrier> barrier);

};