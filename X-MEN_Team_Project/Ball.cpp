#include "Ball.h"
#include "FieldObjectManager.h"
#include "FieldObjectWall.h"
#include "Enemy/FollowEnemy.h"
#include "Enemy/BarrierEnemy.h"
#include "NormalBarrier.h"
#include "EnemyBarrier.h"
#include "GameManager.h"
#include <Random.h>
#include <LibMath.h>
#include <GameObjectManager.h>
#include "Enemy/EnemyManager.h"
#include <Random.h>
#include <LibMath.h>
#include <ModelData.cpp>

const MelLib::Color Ball::BALL_COLOR_RED = { 255,64,64,255 };
const MelLib::Color Ball::BALL_COLOR_BLUE = { 64,64,255,255 };
const MelLib::Color Ball::BALL_COLOR_BLUE2 = { 60,20,195,128 };
const MelLib::Color Ball::BALL_COLOR_YELLOW = { 255,255,64,255 };

const MelLib::Vector3 BallTrajectory::TRAJECTORY_SCALE = { 1.25,1.25,1.25 };

void Ball::Move()
{
	//�ʒu�X�V
	SetPosition(GetPosition() + velocity * speed * GameManager::GetInstance()->GetGameTime());

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

const MelLib::Color Ball::GetColorFromBallState(const BallState& ballState)
{
	MelLib::Color result;
	switch (throwingState)
	{
	case BallState::NONE:
		result = BALL_COLOR_BLUE2;
		break;
	case BallState::HOLD_PLAYER:
		result = BALL_COLOR_BLUE2;
		break;
	case BallState::HOLD_ENEMY:
		result = BALL_COLOR_RED;
		break;
	case BallState::THROWING_PLAYER:
		result = BALL_COLOR_BLUE2;
		break;
	case BallState::THROWING_ENEMY:
		result = BALL_COLOR_RED;
		break;
	default:
		result = BALL_COLOR_BLUE2;
		break;
	}

	return result;
}

Ball::Ball()
{
	// MelLib;;ModelObject�̔z��
	MelLib::ModelData::Load("Resources/Model/Ball/Ball.obj", true, "objBall");
	modelObjects["main"].Create(MelLib::ModelData::Get("objBall"));
	modelObjects["main"].SetScale(scale);
	throwingState = BallState::NONE;
	SetColor(GetColorFromBallState(throwingState));

	// �����蔻��̍쐬(��)
	sphereDatas["main"].resize(1);
	sphereDatas["main"][0].SetPosition(GetPosition());
	sphereDatas["main"][0].SetRadius(scale / 2);

	sphereFrameHitCheckNum = 4;
	SetPosition({ 0,0,-10 });

	//�O�ՃI�u�W�F�N�g����
	for (auto& v : pBallTrajectories) {
		v = std::make_shared<BallTrajectory>();
		//�����ɕR�Â��Ă��郂�f�������������Ă���
		v->CreateModel(modelObjects["main"].GetPModelData());
		//�����ʒu�������̈ʒu
		v->SetPosition(GetPosition());
		//�X�P�[���Z�b�g
		v->SetScale(BallTrajectory::TRAJECTORY_SCALE);
		//�F�Z�b�g
		v->SetMulColor(GetColorFromBallState(throwingState));
		//�S�Ĕ�\��
		v->SetIsDisp(false);
	}
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

	//�O�ՍX�V
	UpdateTrajectories();

	//�������Ă����Ԃ���F�Z�b�g
	SetColor(GetColorFromBallState(throwingState));
}

void Ball::Draw()
{
	//�{�[���{�̕`��
	AllDraw();
	////�܂��O�Օ`��
	//DrawTrajectories();
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
		//const Enemy* other = static_cast<const Enemy*>(&object);
		//if (other->GetHP() > 0.0f)
		//{
			// �v���C���[���������蔽�˂������{�[���Ȃ�
			if (throwingState == BallState::THROWING_PLAYER ||
				throwingState == BallState::THROWING_ENEMY)
			{
				//���ˋ��ʏ���
				Vector3 otherNormal = GetPosition() - object.GetPosition();
				otherNormal = otherNormal.Normalize();
				Reflection(otherNormal, true);

				velocity.y = 0;
				SetPosition({ GetPosition().x, 0, GetPosition().z });

				throwingState = BallState::THROWING_ENEMY;
			}
		//}
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
		velocity = MelLib::Quaternion(1, 0, 0, 1).GetZXYRotateQuaternion({ 0,0,1 }, other->GetAngle()).ToVector3();
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

	//�O�ՃI�u�W�F�N�g�̏���S�č��̃{�[���̏���
	for (auto& v : pBallTrajectories) {
		//�����̈ʒu
		v->SetPosition(GetPosition());
		//�X�P�[���Z�b�g
		v->SetScale(BallTrajectory::TRAJECTORY_SCALE);
		//�F�Z�b�g
		v->SetMulColor(GetColorFromBallState(throwingState));
		//�S�ĕ\��
		v->SetIsDisp(true);
	}
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

void Ball::UpdateTrajectories()
{
	//��Ԍ��̃I�u�W�F�N�g����{�̂Ɍ������ď����R�s�[���Ă���
	for (int i = _countof(pBallTrajectories) - 1; i >= 0; i--) {
		if (i > 0) {
			pBallTrajectories[i]->SetPosition(pBallTrajectories[i - 1]->GetPosition());	//���W
			//pBallTrajectories[i]->SetScale(pBallTrajectories[i - 1]->GetScale());		//�X�P�[��
			MelLib::Color color = pBallTrajectories[i - 1]->GetColor();
			color.a = 128 - (128 * ((float)i / _countof(pBallTrajectories)));
			color.a *= speed >= 1 ? 1 : speed;
			pBallTrajectories[i]->SetColor(color);										//�F
			pBallTrajectories[i]->SetIsDisp(pBallTrajectories[i - 1]->GetIsDisp());		//�\�����邩
		}
		else {
			//��Ԗ{�̂ɋ߂��I�u�W�F�N�g�͖{�̂̏��R�s�[
			pBallTrajectories[i]->SetPosition(GetPosition());							//���W
			//pBallTrajectories[i]->SetScale(BallTrajectory::TRAJECTORY_SCALE);						//�X�P�[��
			MelLib::Color color = GetColorFromBallState(throwingState);
			color.a = 128;
			pBallTrajectories[i]->SetColor(color);		//�F
			pBallTrajectories[i]->SetIsDisp(isThrowed);		//�\�����邩
		}
	}
}

void Ball::DrawTrajectories()
{
	for (int i = 0; i < _countof(pBallTrajectories); i++) {
		if (/*i % 8 == 0 &&*/											//8��1�`��
			pBallTrajectories[i]->GetIsDisp() &&				//�\���t���O�`�F�b�N
			MelLib::LibMath::CalcDistance3D(pBallTrajectories[i]->GetPosition(), GetPosition()) >= 1)//�߂����Ȃ�������
		{
			pBallTrajectories[i]->Draw();
		}
	}
}

void BallTrajectory::Draw()
{
	AllDraw();
}

void BallTrajectory::CreateModel(MelLib::ModelData* pModelData)
{
	modelObjects["main"].Create(pModelData);
}

void BallTrajectory::SetColor(const MelLib::Color& color)
{
	this->color = color;
	modelObjects["main"].SetMulColor(color);
}