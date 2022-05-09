#include "Ball.h"
#include "FieldObjectManager.h"
#include "FieldObjectWall.h"
#include <Random.h>
#include <LibMath.h>


void Ball::Move()
{
	//�ʒu�X�V
	SetPosition(GetPosition() + velocity * speed);

	//�X�s�[�h�������炷
	speed -= 0.001f;
	if (speed < 0) { speed = 0; }
}

Ball::Ball()
{
	// MelLib;;ModelObject�̔z��
	// �l�p�`���Z�b�g
	modelObjects["main"].Create(MelLib::ModelData::Get(MelLib::ShapeType3D::BOX));

	//�F�Z�b�g (�b��ŉ��F)
	modelObjects["main"].SetMulColor(MelLib::Color(255, 255, 64, 255));

	// �����蔻��̍쐬(��)
	// Player�̍��W���擾���A������Z�b�g
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

		//��~�����玩���̎茳�ɖ߂�
		if (speed <= 0) {
			isThrowed = false;
		}
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

	//�ǂƂ̔���
	if (typeid(object) == typeid(FieldObjectWall))
	{
		//���˃x�N�g�����v�Z
		Vector3 otherNormal = GetSphereCalcResult().GetBoxHitSurfaceNormal();
		Vector3 reflectVel = (velocity - 2.0f * velocity.Dot(otherNormal) * otherNormal);

		//���f
		velocity = reflectVel;
	}
}

void Ball::ThrowBall(const Vector3& initPos)
{
	//XZ���ʂ̃����_���Ȋp�x�Ŏˏo
	float rand = MelLib::Random::GetRandomFloatNumberRangeSelect(0, 2 * MelLib::LibMath::GetFloatPI(), 2);
	float initX = sin(rand);
	float initZ = cos(rand);

	//�X�s�[�h�������l��
	speed = INIT_SPEED;

	//�ʒu�A�����Z�b�g
	SetBallPos(initPos);
	velocity = { initX, 0, initZ };

	//�ˏo�t���O��L����
	isThrowed = true;
}

void Ball::SetBallPos(const Vector3& pos)
{
	//�I�u�W�F�N�g�ɔ��f
	SetPosition(pos);
}
