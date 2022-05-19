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
	// 各ポジション計算
	// 上用
	// zを右左用のサイズzの半分＋上下用のzのサイズの半分を足す
	const MelLib::Vector3 TOP_POSITION = { 0,0,FieldObjectWallInfo::RIGHT_LEFT_SIZE.z / 2 + FieldObjectWallInfo::TOP_BOTTOM_SIZE.z / 2 };

	// 下用
	// 上用のzがマイナスになるだけ
	const MelLib::Vector3 BOTTOM_POSITION = { TOP_POSITION.x,TOP_POSITION.y,-TOP_POSITION.z };

	// 右用
	// xを上下用のサイズxの半分＋左右用のxのサイズの半分を足す
	const MelLib::Vector3 RIGHT_POSITION = { FieldObjectWallInfo::TOP_BOTTOM_SIZE.x / 2 + FieldObjectWallInfo::RIGHT_LEFT_SIZE.x / 2,0,0 };

	// 左用
	const MelLib::Vector3 LEFT_POSITION = { -RIGHT_POSITION.x,RIGHT_POSITION.y,RIGHT_POSITION.z };

	// 上追加
	AddWall(TOP_POSITION, FieldObjectWallInfo::TOP_BOTTOM_SIZE);
	// 下追加
	AddWall(BOTTOM_POSITION, FieldObjectWallInfo::TOP_BOTTOM_SIZE);
	// 右追加
	AddWall(RIGHT_POSITION, FieldObjectWallInfo::RIGHT_LEFT_SIZE);
	// 左追加
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
		// 現在のポイントと次のポイントから座標、角度を計算するため
		// 次のポイントしてがVALUE(何角形かを表す値)を超えている場合は最初のポイントとして扱う
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
	// 壁オブジェクトを格納する配列のっメモrを確保
	fieldObjects[FieldObjectType::FIELD_OBJECT_TYPE_WALL] = std::make_shared<std::vector<std::shared_ptr<FieldObject>>>();

	// 他の種類の配列のメモリ確保も基本ここから書く

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
