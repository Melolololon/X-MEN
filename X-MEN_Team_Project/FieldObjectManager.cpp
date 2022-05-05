#include "FieldObjectManager.h"
#include "FieldObjectWall.h"

#include<GameObjectManager.h>

void FieldObjectManager::AddWall(const MelLib::Vector3& pos, const MelLib::Vector3& size)
{
	auto fieldObjectWall = std::make_shared<FieldObjectWall>();

	fieldObjectWall.get()->SetScale(size);
	fieldObjectWall.get()->SetPosition(pos);

	fieldObjects.push_back(fieldObjectWall);
	MelLib::GameObjectManager::GetInstance()->AddObject(fieldObjectWall);
}

void FieldObjectManager::AddWalls()
{
	// �e�|�W�V�����v�Z
	// ��p
	// z���E���p�̃T�C�Yz�̔����{�㉺�p��z�̃T�C�Y�̔����𑫂�
	const MelLib::Vector3 TOP_POSITION = { 0,0,FieldObjectWallInfo::RIGHT_LEFT_SIZE.z / 2 + FieldObjectWallInfo::TOP_BOTTOM_SIZE.z / 2 };

	// ���p
	// ��p��z���}�C�i�X�ɂȂ邾��
	const MelLib::Vector3 BOTTOM_POSITION = { TOP_POSITION.x,TOP_POSITION.y,-TOP_POSITION.z };

	// �E�p
	// x���㉺�p�̃T�C�Yx�̔����{���E�p��x�̃T�C�Y�̔����𑫂�
	const MelLib::Vector3 RIGHT_POSITION = { FieldObjectWallInfo::TOP_BOTTOM_SIZE.x / 2 + FieldObjectWallInfo::RIGHT_LEFT_SIZE.x / 2,0,0 };

	// ���p
	const MelLib::Vector3 LEFT_POSITION = { -RIGHT_POSITION.x,RIGHT_POSITION.y,RIGHT_POSITION.z };

	// ��ǉ�
	AddWall(TOP_POSITION, FieldObjectWallInfo::TOP_BOTTOM_SIZE);
	// ���ǉ�
	AddWall(BOTTOM_POSITION, FieldObjectWallInfo::TOP_BOTTOM_SIZE);
	// �E�ǉ�
	AddWall(RIGHT_POSITION, FieldObjectWallInfo::RIGHT_LEFT_SIZE);
	// ���ǉ�
	AddWall(LEFT_POSITION, FieldObjectWallInfo::RIGHT_LEFT_SIZE);
}

FieldObjectManager::FieldObjectManager()
{
}

void FieldObjectManager::Initialize()
{
	AddWalls();
}

void FieldObjectManager::Finalize()
{
	fieldObjects.clear();
}
