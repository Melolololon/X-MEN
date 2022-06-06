#pragma once
#include <GameObject.h>
#include "Player.h"
#include "Dome.h"

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

//�{�[���̋O�Օ\���p
class BallTrajectory
	: public MelLib::GameObject
{
	using Vector3 = MelLib::Vector3;

public:
	static const Vector3 TRAJECTORY_SCALE;

private:
	MelLib::Color color;
	bool isDisp = true;
public:

	//�`��
	void Draw()override;

	//���f������
	void CreateModel(MelLib::ModelData* pModelData);

	//�F�Z�b�g
	void SetColor(const MelLib::Color& color);

	//�F�Q�b�g
	const MelLib::Color& GetColor() { return color; }

	//�`�悷�邩�̃t���O�Z�b�g
	void SetIsDisp(bool isDisp) { this->isDisp = isDisp; }

	//�`�悷�邩�̃t���O�Q�b�g
	bool GetIsDisp() { return isDisp; }

};

//�{�[���{��
class Ball
	: public MelLib::GameObject
{
	using Vector3 = MelLib::Vector3;

	//�萔
public:
	//�{�[���ɃZ�b�g����F
	static const MelLib::Color BALL_COLOR_RED;		//��
	static const MelLib::Color BALL_COLOR_BLUE;		//��
	static const MelLib::Color BALL_COLOR_BLUE2;	//��2
	static const MelLib::Color BALL_COLOR_YELLOW;	//��

private:
	//�{�[���̏����X�s�[�h
	const float INIT_THROW_SPEED = 0.5f;
	const float INIT_SCALE = 4.0f;
	const float MAX_SCALE = 10.0f;
	const float BALL_ACCEL = 1.5F;
	const float BALL_ACCEL_DOME = 1.0F;
	const float BALL_MAX_SPEED_NORMAL = 3.0f;
	const float BALL_MAX_SPEED_DOME = 8.0f;
	const float BALL_FRICTION = 0.0025f;

private:
	//�ړ�����
	Vector3 velocity = { 0,0,0 };
	//�{�[���̑���
	float speed = 0;
	//�ő�X�s�[�h
	float maxSpeed = 3.0f;
	//�{�[���̑傫��
	float scale = INIT_SCALE;
	//������ꂽ�ォ
	bool isThrowed = true;

	BallState throwingState;

	//�O�Օ\���p�I�u�W�F�N�g
	std::shared_ptr<BallTrajectory> pBallTrajectories[60];

	//�h�[���̃|�C���^
	Dome* pDome;

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

	/// <summary>
	/// BallState�����Ԃɍ��킹���F�擾
	/// </summary>
	/// <param name="ballState">BallState</param>
	const MelLib::Color GetColorFromBallState(const BallState& ballState);

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

	/// <summary>
	/// �O�ՍX�V
	/// </summary>
	void UpdateTrajectories();

	/// <summary>
	/// �O�Օ`��
	/// </summary>
	void DrawTrajectories();

	/// <summary>
	/// �h�[���Ƃ̓����蔻��
	/// </summary>
	/// <returns>���˂����t���[����true�A����ȊO��false</returns>
	bool UpdateBallToDome();

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

	void SetPDome(Dome* pDome) { this->pDome = pDome; }
#pragma endregion
};