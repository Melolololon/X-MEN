#include "FieldObjectManager.h"
#include "FieldObjectWall.h"

#include<GameObjectManager.h>
#include <LibMath.h>
#include <Interpolation.h>
#include <Random.h>

void FieldObjectManager::AddWall(const MelLib::Vector3& pos, const MelLib::Vector3& size, const MelLib::Vector3& angle)
{
	auto fieldObjectWall = std::make_shared<FieldObjectWall>();

	fieldObjectWall.get()->SetCenterPosition(MelLib::Vector3());
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

void FieldObjectManager::AddWalls(const unsigned int VALUE, bool isRotate)
{
	MelLib::Vector3 firstPoint = MelLib::Vector3(0, 0, 0);
	MelLib::Vector3 secondPoint = MelLib::Vector3(0, 0, 0);

	MelLib::Vector3 pos = MelLib::Vector3(0, 0, 0);
	MelLib::Vector3 size = MelLib::Vector3(1, 1, 1);
	MelLib::Vector3 angle = MelLib::Vector3(0, 0, 0);

	// �x�N�g�����s��ŕϊ����郉���_��
	auto TransformVector = [](const MelLib::Vector3& v, const MelLib::Matrix& m)
	{
		float w = v.x * m.matrix[0][3] + v.y * m.matrix[1][3] + v.z * m.matrix[2][3] + m.matrix[3][3];

		MelLib::Vector3 result
		{
			(v.x * m.matrix[0][0] + v.y * m.matrix[1][0] + v.z * m.matrix[2][0] + m.matrix[3][0]) / w,
			(v.x * m.matrix[0][1] + v.y * m.matrix[1][1] + v.z * m.matrix[2][1] + m.matrix[3][1]) / w,
			(v.x * m.matrix[0][2] + v.y * m.matrix[1][2] + v.z * m.matrix[2][2] + m.matrix[3][2]) / w,
		};

		return result;
	};

	for (unsigned int i = 0; i < VALUE; ++i)
	{
		// ���݂̃|�C���g�Ǝ��̃|�C���g������W�A�p�x���v�Z���邽��
		// ���̃|�C���g���Ă�VALUE(���p�`����\���l)�𒴂��Ă���ꍇ�͍ŏ��̃|�C���g�Ƃ��Ĉ���
		unsigned int nextIndex = i + 1;
		if (nextIndex > VALUE)nextIndex = 0;

		const float DOWBLE_PI = 2.0f * MelLib::LibMath::GetFloatPI();
		const float MAX_ANGLE = 180.0f;

		firstPoint.x = std::sinf((DOWBLE_PI / VALUE) * i);
		firstPoint.y = 0;
		firstPoint.z = std::cosf((DOWBLE_PI / VALUE) * i);

		secondPoint.x = std::sinf((DOWBLE_PI / VALUE) * nextIndex);
		secondPoint.y = 0;
		secondPoint.z = std::cosf((DOWBLE_PI / VALUE) * nextIndex);

		// isRotate�̏ꍇ�p�x�𐳂�
		if (isRotate)
		{
			const float ANGLE = DOWBLE_PI / VALUE / 2;

			firstPoint = TransformVector(firstPoint, MelLib::Matrix::GetRotateYMatrix(ANGLE));
			secondPoint = TransformVector(secondPoint, MelLib::Matrix::GetRotateYMatrix(ANGLE));
		}

		const float HALF_LERP_POINT = 0.5f;
		pos = MelLib::Interpolation::Lerp(firstPoint, secondPoint, HALF_LERP_POINT);
		float distance = pos.Length();
		pos = pos.Normalize();

		angle.y = MAX_ANGLE / MelLib::LibMath::GetFloatPI() * std::atan2f(pos.x, pos.z);

		size = FieldObjectWallInfo::TOP_BOTTOM_SIZE * distance;
		pos *= FieldObjectWallInfo::TOP_BOTTOM_SIZE.x / 2 * distance;

		AddWall(pos, size, angle);
	}
}

void FieldObjectManager::LoomingUpdate()
{
	if (!isLooming)return;

	if (loomingTime > 1)
	{
		// �k���J�E���g���c���Ă��Ȃ��Ȃ�I��
		if (loomingCount <= 0)
		{
			isLooming = false;
		}

		loomingTime = 0;
		--loomingCount;
		return;
	}

	const float FRAME = 1.0f / 60.0f;
	loomingTime += FRAME / FieldObjectManagerInfo::MAX_LOOMING_TIME;
}

FieldObjectManager* FieldObjectManager::GetInstance()
{
	static FieldObjectManager instance;
	return &instance;
}

FieldObjectManager::FieldObjectManager()
	: isLooming(false)
	, loomingTime(0)
{
}

void FieldObjectManager::Initialize()
{
	// �ǃI�u�W�F�N�g���i�[����z��̂�����r���m��
	fieldObjects[FieldObjectType::FIELD_OBJECT_TYPE_WALL] = std::make_shared<std::vector<std::shared_ptr<FieldObject>>>();

	// ���̎�ނ̔z��̃������m�ۂ���{�������珑��
	std::srand(rand());

	//AddWalls(5,true);
	AddWalls(MelLib::Random::GetRandomNumberSetNumber({ 4,5,6 }), (bool)MelLib::Random::GetRandomNumberSetNumber({ 0,1 }));
}

void FieldObjectManager::Update()
{
	LoomingUpdate();
}

void FieldObjectManager::Finalize()
{
	fieldObjects.clear();
}

std::shared_ptr<std::vector<std::shared_ptr<FieldObject>>> FieldObjectManager::GetFieldObjects(FieldObjectType hash)
{
	return fieldObjects[hash];
}

void FieldObjectManager::Looming()
{
	isLooming = true;
	++loomingCount;
}

bool FieldObjectManager::IsLooming() const
{
	return isLooming;
}