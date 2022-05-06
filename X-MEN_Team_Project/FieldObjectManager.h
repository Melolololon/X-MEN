#pragma once
#include "FieldObject.h"
#include <vector>

class FieldObjectManager
{
private:
	std::unordered_map<FieldObjectType, std::vector<std::shared_ptr<FieldObject>>> fieldObjects;
private:
	// 壁を追加する
	void AddWall(const MelLib::Vector3& pos,const MelLib::Vector3& size);
	// 壁群を追加する
	void AddWalls();
public:
	static FieldObjectManager* GetInstance();

	// メモリ確保など
	void Initialize();

	// 保持しているオブジェクトのポインタ配列をクリアしたり、追加した数を保持する変数を初期化する
	void Finalize();

	// 引数のハッシュ値（フィールドオブジェクトのタイプ）をもとに配列の参照を返す
	// 型や引数に必要なenum classが長いため auto& や GetInstanceを先にしておくなどすると可読性上がります
	// auto& だけの 例) auto& fieldObjects = FieldObjectManager::GetInstance()->GetFieldObjects(FieldObjectType::FIELD_OBJECT_TYPE_WALL);
	std::vector<std::shared_ptr<FieldObject>>& GetFieldObjects(FieldObjectType hash);

private:
	// シングルトン用
	FieldObjectManager();
	FieldObjectManager(const FieldObjectManager&) = delete;
	~FieldObjectManager() = default;
	FieldObjectManager& operator=(const FieldObjectManager&) = delete;
};
