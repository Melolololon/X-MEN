#pragma once
#include"Enemy.h"

namespace BarrierEnemyStatus
{
	const float MAX_HP = 200;

	const float DISTANCE_TO_PLAYER = 500;
}

class BarrierEnemy : public Enemy
{
private:

	// �{�[���̕����Ɍ������߂̕����x�N�g�� ����g�p���ă��f���̌�����ύX����
	MelLib::Vector3 ballDir;

private:

	void Move() override;

public:
	BarrierEnemy();
	~BarrierEnemy(){}

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


};