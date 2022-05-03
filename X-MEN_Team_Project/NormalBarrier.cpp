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
	SetScale(MelLib::Vector3(5, 5, 1));
	time = 0;
	isOpen = false;
	//sphereDatas["main"][0].SetRadius(0.5f);

	// �����蔻��̍쐬(��)
	// NormalBarrier�̍��W���擾���A������Z�b�g
	//sphereDatas["main"].resize(1);
	//sphereDatas["main"][0].SetPosition(GetPosition());
};

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
	//����e�X�g***Player�Ɠ�������������Ă悵***
	if (MelLib::Input::KeyState(DIK_SPACE))
	{
		isOpen = true;
	}
	//���W0,0,0�𒆐S�ɔ��aradius���ꂽ�Ƃ���ɏ���W�J***Player�ɏ�����������Ă悵***
	SetBarrierPosition(MelLib::Vector3(), MelLib::Vector3(1,0,-1));
	

	modelObjects["main"].SetMulColor(MelLib::Color(150, 150, 255, 255));
}

void NormalBarrier::Draw()
{
	//�W�J���̂ݕ`��
	if (isOpen)
	{
		// ModelObjects�ɒǉ�����Ă���ModelObject�����ׂĕ`��
		AllDraw();
	}
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

void NormalBarrier::SetBarrierPosition(MelLib::Vector3 positon, MelLib::Vector3 move)
{
	//�W�J���Ȃ�
	if (isOpen)
	{
		//�̂̌�����move���狁�߂�
		float direction = atan2f(move.x, move.z);
		//�o���A��W�J������W
		MelLib::Vector3 barrierPosition;
		//������position�𒆐S�ɔ��aradius�����ꂽ���W�����߂�
		barrierPosition= MelLib::Vector3(positon.x + (sinf(direction) * radius), positon.y, positon.z + (cosf(direction) * radius));
		SetPosition(barrierPosition);
		//�x���@���ʓx�@
		SetAngle(MelLib::Vector3(0.0f, direction * 57.32484076433121f, 0.0f));

	}
}
