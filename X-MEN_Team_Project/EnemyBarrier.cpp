#include "EnemyBarrier.h"

#include<Input.h>

EnemyBarrier::EnemyBarrier()
{
	// MelLib;;ModelObject�̔z��
	// �l�p�`���Z�b�g
	modelObjects["main"].Create(MelLib::ModelData::Get(MelLib::ShapeType3D::BOX));
	//�����l
	const MelLib::Vector3 INIT_POS = { 0,0,5 };
	const MelLib::Vector3 INIT_SCALE = { 5,5,1 };
	SetPosition(INIT_POS);
	SetScale(INIT_SCALE);
	nowHp = aaaa::HP;
	isOpen = false;

	// �����蔻��̍쐬(OBB)
	obbDatas["main"].resize(1);
	obbDatas["main"][0].SetPosition(GetPosition());
	obbDatas["main"][0].SetSize(INIT_SCALE);
	obbDatas["main"][0].SetAngle(GetAngle());
}

void EnemyBarrier::Update()
{
	//�W�J���Ȃ�
	if (isOpen)
	{
		//�ϋv�l��0�ɂȂ�����
		if (nowHp < 0)
		{
			isOpen = false;
		}
	}
	modelObjects["main"].SetMulColor(MelLib::Color(255, 150, 150, 255));
}

void EnemyBarrier::Draw()
{
	//�W�J���̂ݕ`��
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

void EnemyBarrier::SetBarrierPosition(MelLib::Vector3 positon, MelLib::Vector3 move)
{
	//�W�J���Ȃ�
	if (isOpen)
	{
		//�̂̌�����move���狁�߂�
		float direction = atan2f(move.x, move.z);
		//�o���A��W�J������W
		MelLib::Vector3 barrierPosition;
		//������position�𒆐S�ɔ��aradius�����ꂽ���W�����߂�
		barrierPosition = MelLib::Vector3(positon.x + (sinf(direction) * aaaa::RADIUS), positon.y, positon.z + (cosf(direction) * aaaa::RADIUS));
		SetPosition(barrierPosition);

		//OBB�������C�u�����ŏ���ɔ��f����Ȃ����ۂ��̂łƂ肠�����蓮
		obbDatas["main"][0].SetPosition(GetPosition());

		//�x���@���ʓx�@
		SetAngle(MelLib::Vector3(0.0f, direction * 57.32484076433121f, 0.0f));

		//OBB�������C�u�����ŏ���ɔ��f����Ȃ����ۂ��̂łƂ肠�����蓮
		obbDatas["main"][0].SetAngle(GetAngle());
	}
}

void EnemyBarrier::OpenBarrier()
{
	isOpen = true;
	nowHp = aaaa::HP;
}
