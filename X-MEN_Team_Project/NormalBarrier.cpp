#include "NormalBarrier.h"

#include<Input.h>

#include"TestObject.h"


NormalBarrier::NormalBarrier()
{
	// MelLib;;ModelObject�̔z��
	// �l�p�`���Z�b�g
	modelObjects["main"].Create(MelLib::ModelData::Get(MelLib::ShapeType3D::BOX));
	//�����l
	SetPosition(MelLib::Vector3(0, 0, 5));
	SetScale(MelLib::Vector3(5, 5, 5));
	time = 0;
	isOpen = false;

	// �����蔻��̍쐬(��)
	// NormalBarrier�̍��W���擾���A������Z�b�g
	//sphereDatas["main"].resize(1);
	//sphereDatas["main"][0].SetPosition(GetPosition());
	//sphereDatas["main"][0].SetRadius(0.5f);
}

void NormalBarrier::Update()
{
	//�W�J���Ȃ�
	if (isOpen)
	{
		//�W�J���ԃJ�E���g
		time++;
		//�w�肵���W�J�̎��Ԍo�߂�����
		if (time > openTime)
		{
			isOpen = false;
			time = 0;
		}
	}
	modelObjects["main"].SetMulColor(MelLib::Color(150, 150, 255, 255));
}

void NormalBarrier::Draw()
{
	// ModelObjects�ɒǉ�����Ă���ModelObject�����ׂĕ`��
	AllDraw();
}

void NormalBarrier::Hit
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

void NormalBarrier::SetIsOpen(bool flag)
{
	isOpen = flag;
}
