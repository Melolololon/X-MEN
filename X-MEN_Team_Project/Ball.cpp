#include "Ball.h"
#include "FieldObjectManager.h"
#include "FieldObjectWall.h"
#include "Enemy/FollowEnemy.h"
#include "Enemy/BarrierEnemy.h"
#include "NormalBarrier.h"
#include "EnemyBarrier.h"
#include "Enemy/EnemyManager.h"
#include <Random.h>
#include <LibMath.h>
#include <ModelData.cpp>

const MelLib::Color Ball::BALL_COLOR_RED = { 255,64,64,255 };
const MelLib::Color Ball::BALL_COLOR_BLUE = { 64,64,255,255 };
const MelLib::Color Ball::BALL_COLOR_BLUE2 = { 60,20,195,128 };
const MelLib::Color Ball::BALL_COLOR_YELLOW = { 255,255,64,255 };

void Ball::Move()
{
	//�ʒu�X�V
	SetPosition(GetPosition() + velocity * speed);

	//�X�s�[�h�������炷
	speed -= BALL_FRICTION;
	if (speed < 0) { speed = 0; }
}

void Ball::SetColor(const MelLib::Color& color)
{
	modelObjects["main"].SetMulColor(color);
}

void Ball::Reflection(const Vector3& otherNormal, bool isAddSpeed)
{
	// ���������I�u�W�F�N�g�̋߂��̍��W���擾
	SetPosition(GetLerpExtrudePosition());

	//���˃x�N�g�����v�Z
	Vector3 reflectVel = (velocity - 2.0f * velocity.Dot(otherNormal) * otherNormal);

	//���f
	velocity = reflectVel;

	//�ʒu�X�V (�߂肱�ݖh�~�p)
	SetPosition(GetPosition() + velocity * speed);

	//����
	if (isAddSpeed == false) {
		return;
	}

	speed += BALL_ACCEL;

	if (speed > MAX_SPEED)
	{
		speed = MAX_SPEED;
	}
}

Ball::Ball()
{
	// MelLib;;ModelObject�̔z��
	MelLib::ModelData::Load("Resources/Model/Ball/Ball.obj", true, "objBall");
	modelObjects["main"].Create(MelLib::ModelData::Get("objBall"));
	modelObjects["main"].SetScale(scale);
	//�F�Z�b�g
	SetColor(BALL_COLOR_YELLOW);
	throwingState = BallState::NONE;

	// �����蔻��̍쐬(��)
	sphereDatas["main"].resize(1);
	sphereDatas["main"][0].SetPosition(GetPosition());
	sphereDatas["main"][0].SetRadius(scale / 2);

	sphereFrameHitCheckNum = 4;

	//collisionCheckDistance = MAX_SCALE;

	SetPosition({ 20,0,0 });
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
		AddScale();
		//��~
		if (speed <= 0) {
			////�F�Z�b�g
			//SetColor(BALL_COLOR_YELLOW);
			throwingState = BallState::NONE;
		}
	}
	else {
		//eraseManager = isPicked;
	}

	switch (throwingState)
	{
	case BallState::NONE:
		SetColor(BALL_COLOR_BLUE2);
		break;
	case BallState::HOLD_PLAYER:
		SetColor(BALL_COLOR_BLUE2);
		break;
	case BallState::HOLD_ENEMY:
		SetColor(BALL_COLOR_RED);
		break;
	case BallState::THROWING_PLAYER:
		SetColor(BALL_COLOR_BLUE2);
		break;
	case BallState::THROWING_ENEMY:
		SetColor(BALL_COLOR_RED);
		break;
	default:
		break;
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
		Vector3 otherNormal = GetSphereCalcResult().GetOBBHitSurfaceNormal();
		Reflection(otherNormal, false);
	}
	//�G�Ƃ̏Փ�
	else if (typeid(object) == typeid(FollowEnemy) ||
		typeid(object) == typeid(BarrierEnemy))
	{
		// �v���C���[���������蔽�˂������{�[���Ȃ�
		if (throwingState == BallState::THROWING_PLAYER)
		{
			//���ˋ��ʏ���
			Vector3 otherNormal = GetPosition() - object.GetPosition();
			otherNormal = otherNormal.Normalize();
			Reflection(otherNormal, true);

			throwingState = BallState::THROWING_ENEMY;
		}
	}
	//�v���C���[�Ƃ̏Փ�
	else if (typeid(object) == typeid(Player))
	{
		//�����Ă���Ƃ��͔���
		if (speed > 0) {

			// �G���������蔽�˂������{�[���Ȃ�
			if (throwingState == BallState::THROWING_ENEMY)
			{
				//���ˋ��ʏ���
				Vector3 otherNormal = GetSphereCalcResult().GetBoxHitSurfaceNormal();
				Reflection(otherNormal, false);
			}
		}
	}
	//�m�[�}���o���A�Ƃ̔���
	else if (typeid(object) == typeid(NormalBarrier))
	{

		//�o���A�W�J�����擾���邽�߂Ɍ^�ϊ�
		const NormalBarrier* other = static_cast<const NormalBarrier*>(&object);

		//�o���A�W�J���łȂ��Ȃ烊�^�[��
		if (other->GetIsOpen() == false) {
			return;
		}

		//���ˋ��ʏ���
		Vector3 otherNormal = GetSphereCalcResult().GetOBBHitSurfaceNormal();
		
		//�v���C���[�̃o���A�ɓ���������o���A�̕����ɂ��̂܂ܕԂ�
		velocity = MelLib::Quaternion(1,0,0,1).GetZXYRotateQuaternion({ 0,0,1 }, other->GetAngle()).ToVector3();
		//�ʒu�X�V (�߂肱�ݖh�~�p)
		SetPosition(GetPosition() + velocity * speed);

		//����
		speed += BALL_ACCEL;

		if (speed > MAX_SPEED)
		{
			speed = MAX_SPEED;
		}

		throwingState = BallState::THROWING_PLAYER;
	}
	//�G�l�~�[�o���A�Ƃ̔���
	else if (typeid(object) == typeid(EnemyBarrier))
	{
		//�o���A�W�J�����擾���邽�߂Ɍ^�ϊ�
		const EnemyBarrier* other = static_cast<const EnemyBarrier*>(&object);

		//�o���A�W�J���łȂ��Ȃ烊�^�[��
		if (other->GetIsOpen() == false) {
			return;
		}

		//���ˋ��ʏ���
		Vector3 otherNormal = GetSphereCalcResult().GetOBBHitSurfaceNormal();
		Reflection(otherNormal, true);
		throwingState = BallState::THROWING_ENEMY;
	}
}

void Ball::ThrowBall(const Vector3& initVel)
{
	//�X�s�[�h�������l��
	speed = INIT_THROW_SPEED;

	//�����Z�b�g
	velocity = initVel;

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

void Ball::AddScale()
{
	if (EnemyManager::GetInstance()->GetIsDeadFlame() == false) {
		return;
	}

	//�ЂƂ܂�5��G�|���ƍő�l�ɂȂ�悤��
	scale += (MAX_SCALE - INIT_SCALE) / 5;

	if (scale > MAX_SCALE) {
		scale = MAX_SCALE;
	}

	//�I�u�W�F�N�g�ɔ��f
	SetScale({ scale,scale,scale });

	EnemyManager::GetInstance()->SetIsDeadFlame(false);
}
