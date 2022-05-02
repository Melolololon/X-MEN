#include "EnemyBarrier.h"

#include<Input.h>

#include"TestObject.h"

EnemyBarrier::EnemyBarrier()
{
	// MelLib;;ModelObject�̔z��
	// �l�p�`���Z�b�g
	modelObjects["main"].Create(MelLib::ModelData::Get(MelLib::ShapeType3D::BOX));
	//�����l
	SetPosition(MelLib::Vector3(0, 0, 5));
	SetScale(MelLib::Vector3(5, 5, 1));
	nowHp = hp;
	isOpen = false;

	// �����蔻��̍쐬(��)
	// EnemyBarrier�̍��W���擾���A������Z�b�g
	//sphereDatas["main"].resize(1);
	//sphereDatas["main"][0].SetPosition(GetPosition());
	//sphereDatas["main"][0].SetRadius(0.5f);
}

void EnemyBarrier::Update()
{
	//�W�J���Ȃ�
	if (isOpen)
	{
		//�ϋv�l��0�ɂȂ�����
		if (hp < 0)
		{
			isOpen = false;
			nowHp = hp;
		}
	}
	modelObjects["main"].SetMulColor(MelLib::Color(255, 150, 150, 255));
}

void EnemyBarrier::Draw()
{
	if (isOpen)
	{
		// ModelObjects�ɒǉ�����Ă���ModelObject�����ׂĕ`��
		AllDraw();
	}
}

void EnemyBarrier::Hit
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
}

