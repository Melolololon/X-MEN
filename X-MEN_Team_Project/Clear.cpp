#include "Clear.h"
#include "GamePlay.h"

#include<GameObjectManager.h>
#include<Input.h>

#include"TestObject.h"

void Clear::Initialize()
{
	// 初期化処理
	// 必ずコンストラクタではなくここに初期化処理を書く(設計上の都合で)

	// オブジェクトのメモリ確保
	//pPlayer = std::make_shared<Player>();
	
	// 管理クラスにオブジェクトを追加
	// ObjectManagerはshared_ptrのみ対応

	clearFont.Create(MelLib::Color(255,0,0,255));
	clearFont.SetScale(MelLib::Vector2(600,200));
	clearFont.SetPosition(MelLib::Vector2(300,300));

	// テストオブジェクト追加
	MelLib::GameObjectManager::GetInstance()->AddObject(std::make_shared<TestObject>(MelLib::Vector3(0, 0, 0)));

}

void Clear::Update()
{
	// マネージャーの更新
	// オブジェクトの更新処理、判定処理、削除処理が行われる
	MelLib::GameObjectManager::GetInstance()->Update();	

	// Aキーで現在のシーンを終了して次のシーンへ
	// 今は次のシーンに今と同じシーンをセットしているため、位置がリセットされるだけ
	if (MelLib::Input::KeyTrigger(DIK_SPACE)|| MelLib::Input::PadButtonTrigger(MelLib::PadButton::A))isEnd = true;
}

void Clear::Draw()
{
	clearFont.Draw();
	// 描画
	MelLib::GameObjectManager::GetInstance()->Draw();
}

void Clear::Finalize()
{
	// 終了処理

	// 全削除
	MelLib::GameObjectManager::GetInstance()->AllEraseObject();
}

MelLib::Scene* Clear::GetNextScene()
{
	// 次のシーンのポインタを返す
	// 例 return new Title();
	return new GamePlay();
}
