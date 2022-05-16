#pragma once
#include <GameObject.h>
#include "Player.h"

enum class BallState
{
	NONE,
	// �v���C���[�������Ă���
	HOLD_PLAYER,
	// �G���E�E�E
	HOLD_ENEMY,
	// �v���C���[��������or���˕Ԃ���
	THROWING_PLAYER,
	// �G���E�E�E
	THROWING_ENEMY,
};

class Player;
class Ball
	: public MelLib::GameObject
{
	using Vector3 = MelLib::Vector3;

	//�萔
public:
	//�{�[���ɃZ�b�g����F
	static const MelLib::Color BALL_COLOR_RED;		//��
	static const MelLib::Color BALL_COLOR_BLUE;		//��
	static const MelLib::Color BALL_COLOR_YELLOW;	//��
	
private:
	//�{�[���̏����X�s�[�h
	const float INIT_THROW_SPEED = 0.5f;
	const float MAX_SPEED = 1.8f;
	const float BALL_ACCEL = 0.25F;

private:
	//�ړ�����
	Vector3 velocity = { 0,0,0 };
	//�{�[���̑���
	float speed = 0;
	//������ꂽ�ォ
	bool isThrowed = true;

	BallState throwingState;

private:

	/// <summary>
	/// �ʒu���̕ϓ��֘A
	/// </summary>
	void Move();

	/// <summary>
	/// ���f���ɐF��ݒ肷��
	/// </summary>
	/// <param name="color">�F���</param>
	void SetColor(const MelLib::Color& color);

	/// <summary>
	/// ���˂���Ƃ��̋��ʏ���
	/// </summary>
	/// <param name="otherNormal">�Փ˂����|���S���ʂ̖@���x�N�g��</param>
	/// <param name="isAddSpeed">���������邩</param>
	void Reflection(const Vector3& otherNormal, bool isAddSpeed);

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
	/// �{�[���𓊂���
	/// </summary>
	/// <param name="initVel">�{�[���̔��˕��� ����̈ړ�������n��</param>
	void ThrowBall(const Vector3& initVel);

	/// <summary>
	/// �{�[�����E�����Ƃ��̏���
	/// </summary>
	/// <param name="initPos">�{�[���̏����ʒu</param>
	/// <param name="initColor">�{�[���̏����ʒu �v���C���[�Ȃ�A�G�Ȃ��</param>
	void PickUp(const Vector3& ballPos, const MelLib::Color& initColor);

#pragma region Getter
	/// <summary>
	/// �{�[�����������Ă��邩��Ԃ�
	/// </summary>
	/// <returns>�{�[�����������Ă����true ����ȊO��false</returns>
	bool GetIsThrowed()const { return isThrowed; }

	/// <summary>
	/// �{�[���̌��݂̑�����Ԃ�
	/// </summary>
	/// <returns>����</returns>
	float GetSpeed()const { return speed; }

	/// <summary>
	/// �{�[���𔽎ˁA�����A��N���s�������̏�Ԃ�Ԃ�
	/// </summary>
	/// <returns>���</returns>
	BallState GetThrowingState()const { return throwingState; }

#pragma endregion

#pragma region Setter
	/// <summary>
	/// �{�[���𔽎ˁA�����A��N���s�������̏�Ԃ�ύX
	/// </summary>
	/// <param name="setState">�ύX��̏��</param>
	void SetThrowingState(BallState setState) { throwingState = setState; }
#pragma endregion
};