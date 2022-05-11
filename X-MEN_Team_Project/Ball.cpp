#include "Ball.h"
#include "FieldObjectManager.h"
#include "FieldObjectWall.h"
#include "Enemy/FollowEnemy.h"
#include "Enemy/BarrierEnemy.h"
#include "NormalBarrier.h"
#include "EnemyBarrier.h"
#include <Random.h>
#include <LibMath.h>

const MelLib::Color Ball::BALL_COLOR_RED = { 255,64,64,255 };
const MelLib::Color Ball::BALL_COLOR_BLUE = { 64,64,255,255 };
const MelLib::Color Ball::BALL_COLOR_YELLOW = { 255,255,64,0 };

void Ball::Move()
{
	//�ʒu�X�V
	SetPosition(GetPosition() + velocity * speed);

	//�X�s�[�h�������炷
	speed -= 0.001f;
	if (speed < 0) { speed = 0; }
}

void Ball::SetColor(const MelLib::Color& color)
{
	modelObjects["main"].SetMulColor(color);
}

void Ball::Reflection(const Vector3& otherNormal)
{
	//���˃x�N�g�����v�Z
	Vector3 reflectVel = (velocity - 2.0f * velocity.Dot(otherNormal) * otherNormal);

	//���f
	velocity = reflectVel;

	//speed��1.5�{�� (��U�p�~)
	//speed *= 1.5f;
}

Ball::Ball()
{
	// MelLib;;ModelObject�̔z��
	// �l�p�`���Z�b�g
	modelObjects["main"].Create(MelLib::ModelData::Get(MelLib::ShapeType3D::BOX));

	//�F�Z�b�g
	SetColor(BALL_COLOR_YELLOW);

	// �����蔻��̍쐬(��)
	sphereDatas["main"].resize(1);
	sphereDatas["main"][0].SetPosition(GetPosition());
	sphereDatas["main"][0].SetRadius(0.5f);
}

Ball::~Ball()
{
	// �Ǘ��N���X����폜
	eraseManager = true;
}

void Ball::Update()
{
	//�������Ă����瓮����
	if (isThrowed) {
		Move();

		//��~
		if (speed <= 0) {
			//�F�Z�b�g
			SetColor(BALL_COLOR_YELLOW);
		}
	}
	else {
		//eraseManager = isPicked;
	}
}

void Ball::Draw()
{
	AllDraw();
}

void Ball::Hit(const GameObject& object, const MelLib::ShapeType3D shapeType, const std::string& shapeName, const MelLib::ShapeType3D hitObjShapeType, const std::string& hitShapeName)
{
	//�������Ă��Ȃ���ΏՓˏ������������Ȃ�
	if (isThrowed == false) {
		return;
	}

	//�ǂƂ̏Փ�
	if (typeid(object) == typeid(FieldObjectWall))
	{
		//���ˋ��ʏ���
		Vector3 otherNormal = GetSphereCalcResult().GetBoxHitSurfaceNormal();
		Reflection(otherNormal);

		//�F�Z�b�g
		if (speed > 0) {
			SetColor(BALL_COLOR_BLUE);
		}
	}
	//�G�Ƃ̏Փ�
	else if (typeid(object) == typeid(FollowEnemy) ||
		typeid(object) == typeid(BarrierEnemy))
	{
		//���ˋ��ʏ���
		Vector3 otherNormal = GetSphereCalcResult().GetBoxHitSurfaceNormal();
		Reflection(otherNormal);

		//�ԐF�Z�b�g
		if (speed > 0) {
			SetColor(BALL_COLOR_RED);
		}
	}
	//�v���C���[�Ƃ̏Փ�
	else if (typeid(object) == typeid(Player))
	{
		//�����Ă���Ƃ��͔���
		if (speed > 0) {

			//���ˋ��ʏ���
			Vector3 otherNormal = GetSphereCalcResult().GetBoxHitSurfaceNormal();
			Reflection(otherNormal);

			//�F�Z�b�g
			SetColor(BALL_COLOR_BLUE);
		}
		//��~���Ă���Ȃ炱��ball�͍폜�A���ʒu��Player���ŐV����ball���擾���Ȃ���
		else {
			// �Ǘ��N���X����폜
			eraseManager = true;
		}
	}
	//�m�[�}���o���A�Ƃ̔���
	else if (typeid(object) == typeid(NormalBarrier))
	{
		//���ˋ��ʏ���
		Vector3 otherNormal = GetSphereCalcResult().GetOBBHitSurfaceNormal();
		Reflection(otherNormal);

		//�F�Z�b�g
		if (speed > 0) {
			SetColor(BALL_COLOR_BLUE);
		}
	}
	//�G�l�~�[�o���A�Ƃ̔���
	else if (typeid(object) == typeid(EnemyBarrier))
	{
		//���ˋ��ʏ���
		Vector3 otherNormal = GetSphereCalcResult().GetOBBHitSurfaceNormal();
		Reflection(otherNormal);

		//�ԐF�Z�b�g
		if (speed > 0) {
			SetColor(BALL_COLOR_RED);
		}
	}
}

void Ball::ThrowBall(const Vector3& initVel)
{
	//�X�s�[�h�������l��
	speed = INIT_THROW_SPEED;

	//�����Z�b�g
	velocity = initVel;

	//�ʒu���ړ������փI�u�W�F�N�g�̑傫���������炷 (�������u�ԏՓ˂���̂�h��)
	SetPosition(GetPosition() + velocity * GetScale());

	//�ˏo�t���O��L����
	isThrowed = true;
}

void Ball::PickUp(const Vector3& ballPos, const MelLib::Color& initColor)
{
	//�ʒu�Z�b�g
	SetPosition(ballPos);

	//�F�Z�b�g
	SetColor(initColor);

	//�������t���O�I�t
	isThrowed = false;
}