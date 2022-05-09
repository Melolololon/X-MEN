#pragma once
#include <GameObject.h>

class Ball    
	: public MelLib::GameObject
{
	using Vector3 = MelLib::Vector3;

//�萔
private:
	const float INIT_SPEED = 0.5f;

private:
	Vector3 velocity = { 0,0,0.1 };
	float speed = INIT_SPEED;
	bool isThrowed = false;

private:

	void Move();

public:
	Ball();
	~Ball();

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

	/// <summary>
	/// �{�[���𓊂��� �v���C���[����ĂԂ���
	/// </summary>
	/// <param name="initPos">�{�[���̏����ʒu �v���C���[�̈ʒu�����̂܂ܓn��</param>
	void ThrowBall(const Vector3& initPos);

	/// <summary>
	/// �{�[�����������Ă��邩��Ԃ�
	/// </summary>
	/// <returns>�{�[�����������Ă����true ����ȊO��false</returns>
	bool GetIsThrowed()const { return isThrowed; }

	/// <summary>
	/// �{�[���̈ʒu�����ߑł��ŃZ�b�g���� �v���C���[���{�[������������Ԏ��̒ǔ��Ɏg�p
	/// </summary>
	/// <param name="pos"></param>
	void SetBallPos(const Vector3& pos);
};