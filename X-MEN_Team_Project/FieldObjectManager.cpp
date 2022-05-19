#include "FieldObjectManager.h"
#include "FieldObjectWall.h"

#include<GameObjectManager.h>
#include <LibMath.h>
#include <Interpolation.h>

void FieldObjectManager::AddWall(const MelLib::Vector3& pos, const MelLib::Vector3& size, const MelLib::Vector3& angle)
{
	auto fieldObjectWall = std::make_shared<FieldObjectWall>();

	fieldObjectWall.get()->SetScale(size);
	fieldObjectWall.get()->SetPosition(pos);
	fieldObjectWall.get()->SetAngle(angle);

	fieldObjects[FieldObjectType::FIELD_OBJECT_TYPE_WALL].get()->push_back(fieldObjectWall);

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

void FieldObjectManager::AddWalls(const unsigned int VALUE)
{
	MelLib::Vector3 firstPoint = MelLib::Vector3(0, 0, 0);
	MelLib::Vector3 secondPoint = MelLib::Vector3(0, 0, 0);

	MelLib::Vector3 pos = MelLib::Vector3(0, 0, 0);
	MelLib::Vector3 size = MelLib::Vector3(1, 1, 1);
	MelLib::Vector3 angle = MelLib::Vector3(0,0,0);


	for (int i = 0; i < VALUE; ++i)
	{
		// ���݂̃|�C���g�Ǝ��̃|�C���g������W�A�p�x���v�Z���邽��
		// ���̃|�C���g���Ă�VALUE(���p�`����\���l)�𒴂��Ă���ꍇ�͍ŏ��̃|�C���g�Ƃ��Ĉ���
		int nextIndex = i + 1;
		if (nextIndex > VALUE)nextIndex = 0;

		const float DOWBLE_PI = 2.0f * MelLib::LibMath::GetFloatPI();
		const float MAX_ANGLE = 180.0f;

		firstPoint.x = std::sinf((DOWBLE_PI / VALUE) * i);
		firstPoint.y = 0;
		firstPoint.z = std::cosf((DOWBLE_PI / VALUE) * i);

		secondPoint.x = std::sinf((DOWBLE_PI / VALUE) * nextIndex);
		secondPoint.y = 0;
		secondPoint.z = std::cosf((DOWBLE_PI / VALUE) * nextIndex);

		pos = MelLib::Interpolation::Lerp(firstPoint,secondPoint,0.5f);
		float distance = pos.Length();
		pos = pos.Normalize();
		angle.y = MAX_ANGLE / MelLib::LibMath::GetFloatPI() * std::atan2f(pos.x, pos.z);
		size = FieldObjectWallInfo::TOP_BOTTOM_SIZE * distance;
		pos *= FieldObjectWallInfo::TOP_BOTTOM_SIZE.x / 2;

		AddWall(pos, size, angle);
	}
}

FieldObjectManager* FieldObjectManager::GetInstance()
{
	static FieldObjectManager instance;
	return &instance;
}

FieldObjectManager::FieldObjectManager()
{
}

void FieldObjectManager::Initialize()
{
	// �ǃI�u�W�F�N�g���i�[����z��̂�����r���m��
	fieldObjects[FieldObjectType::FIELD_OBJECT_TYPE_WALL] = std::make_shared<std::vector<std::shared_ptr<FieldObject>>>();

	// ���̎�ނ̔z��̃������m�ۂ���{�������珑��

	//AddWalls();
	AddWalls(6);
}

void FieldObjectManager::Finalize()
{
	fieldObjects.clear();
}

std::shared_ptr<std::vector<std::shared_ptr<FieldObject>>> FieldObjectManager::GetFieldObjects(FieldObjectType hash)
{
	return fieldObjects[hash];
}
