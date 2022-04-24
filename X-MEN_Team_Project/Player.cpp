#include "Player.h"

#include<Input.h>

#include"TestObject.h"

void Player::Move()
{
	// �ړ��x�N�g��
	MelLib::Vector3 moveVector;
	// �ړ����x
	static const float MOVE_SPEED = 0.3f;
	
	if (MelLib::Input::KeyState(DIK_LEFT))
	{
		moveVector.x -= MOVE_SPEED;
	}
	else if (MelLib::Input::KeyState(DIK_RIGHT))
	{
		moveVector.x += MOVE_SPEED;
	}
	else if (MelLib::Input::KeyState(DIK_UP))
	{
		moveVector.z += MOVE_SPEED;
	}
	else if (MelLib::Input::KeyState(DIK_DOWN))
	{
		moveVector.z -= MOVE_SPEED;
	}

	// ���Z
	// AddPosition�ASetPosition�͓����蔻����ꏏ�ɓ���
	AddPosition(moveVector);
}

Player::Player()
{
	// MelLib;;ModelObject�̔z��
	// �l�p�`���Z�b�g
	modelObjects["main"].Create(MelLib::ModelData::Get(MelLib::ShapeType3D::BOX));

	// �����ʒu��0,0,5��
	SetPosition(MelLib::Vector3(0, 0, 5));

	// �����蔻��̍쐬(��)
	// Player�̍��W���擾���A������Z�b�g
	sphereDatas["main"].resize(1);
	sphereDatas["main"][0].SetPosition(GetPosition());
	sphereDatas["main"][0].SetRadius(0.5f);
}

void Player::Update()
{
	Move();

	// SPACE�������烂�f���Ǘ��N���X����폜(���������)
	if(MelLib::Input::KeyTrigger(DIK_SPACE))
	{
		eraseManager = true;
	}

	modelObjects["main"].SetMulColor(MelLib::Color(255, 255, 255, 255));
}

void Player::Draw()
{
	// ModelObjects�ɒǉ�����Ă���ModelObject�����ׂĕ`��
	AllDraw();
}

void Player::Hit
(
	const GameObject& object, 
	const MelLib::ShapeType3D shapeType, 
	const std::string& shapeName, 
	const MelLib::ShapeType3D hitObjShapeType,
	const std::string& hitShapeName
)
{
	// �����ɓ����������̏������L�q
	// typeid�Ȃǂŏ����𕪂����肷��

	// �e�X�g�I�u�W�F�N�g�ƏՓ˂�����F�ύX
	if(typeid(object) == typeid(TestObject))
	{
		modelObjects["main"].SetMulColor(MelLib::Color(100, 100, 100, 255));
	}
}
