#include "Player.h"

#include<Input.h>

void Player::Move()
{
	// �ړ��x�N�g��
	MelLib::Vector3 moveVector;
	// �ړ����x
	static const float MOVE_SPEED = 1.0f;
	
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
	AddPosition(moveVector);
}

Player::Player()
{
	// MelLib;;ModelObject�̔z��
	// �l�p�`���Z�b�g
	modelObjects["main"].Create(MelLib::ModelData::Get(MelLib::ShapeType3D::BOX));

	SetPosition(MelLib::Vector3(0, 5, 0));
}

void Player::Update()
{
	Move();

	// SPACE�������烂�f���Ǘ��N���X����폜(���������)
	if(MelLib::Input::KeyTrigger(DIK_SPACE))
	{
		eraseManager = true;
	}
}

void Player::Draw()
{
	// ModelObjects�ɒǉ�����Ă���ModelObject�����ׂĕ`��
	AllDraw();
}

void Player::Hit
(
	const GameObject& object, 
	const MelLib::ShapeType3D& shapeType, 
	const std::string& shapeName, 
	const MelLib::ShapeType3D& hitObjShapeType, 
	const std::string& hitShapeName
)
{
	// �����ɓ����������̏������L�q
	// typeid�Ȃǂŏ����𕪂����肷��
}
