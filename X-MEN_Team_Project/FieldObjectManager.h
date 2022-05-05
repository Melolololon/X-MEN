#pragma once
#include "FieldObject.h"
#include <vector>

class FieldObjectManager
{
private:
	std::vector<std::shared_ptr<FieldObject>> fieldObjects;
private:
	// 壁を追加する
	void AddWall(const MelLib::Vector3& pos,const MelLib::Vector3& size);
	// 壁群を追加する
	void AddWalls();
public:
	FieldObjectManager();

	// メモリ確保など
	void Initialize();

	// 保持しているオブジェクトのポインタ配列をクリアしたり、追加した数を保持する変数を初期化する
	void Finalize();
};
