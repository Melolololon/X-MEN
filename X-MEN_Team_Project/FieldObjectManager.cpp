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
