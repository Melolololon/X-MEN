#include "Clear.h"
#include "GamePlay.h"

#include<GameObjectManager.h>
#include<Input.h>

#include"TestObject.h"
#include"TextWrite.h"

void Clear::Initialize()
{
	// 初期化処理
	// 必ずコンストラクタではなくここに初期化処理を書く(設計上の都合で)
	
	// 管理クラスにオブジェクトを追加
	// ObjectManagerはshared_ptrのみ対応

	//Clearフォント
	MelLib::TextWrite::CreateFontData(L"Arial", 100, "Test");

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
	//Clearフォント
	MelLib::TextWrite::Draw(MelLib::Vector2(500, 300), MelLib::Color(255, 0, 0, 255), L"Clear", "Test");

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
