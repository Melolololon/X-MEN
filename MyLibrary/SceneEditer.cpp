#include "SceneEditer.h"

#include"ImguiManager.h"
#include"GameObjectManager.h"
#include"SceneManager.h"
#include"Input.h"

#include<fstream>

void MelLib::SceneEditer::Save()
{
	if (!isEdit)return;

	// 2022_05_01
	// オブジェクトマネージャーに追加したオブジェクトをimguiでいじれるように

	std::string name = SceneManager::GetInstance()->GetCurrentSceneName();
	name += ".mlsce";
	std::ofstream file(name);

	// 書き出し


	file.close();
}

void MelLib::SceneEditer::SelectObjectUpdate()
{
	if (!isEdit)return;
	
	MelLib::Vector3 pos = pSelectObject->GetPosition();
	ImguiManager::GetInstance()->DrawSliderVector3("Position", pos, -1000, 1000);
	pSelectObject->SetPosition(pos);

	MelLib::Vector3 angle = pSelectObject->GetAngle();
	ImguiManager::GetInstance()->DrawSliderVector3("Angle", angle, 0, 359);
	pSelectObject->SetAngle(angle);

	MelLib::Vector3 scale = pSelectObject->GetScale();
	ImguiManager::GetInstance()->DrawSliderVector3("Scale", scale, 0, 359);
	pSelectObject->SetScale(scale);
}

MelLib::SceneEditer* MelLib::SceneEditer::GetInstance()
{
	static SceneEditer s;
	return &s;
}

void MelLib::SceneEditer::RegisterObject(const std::shared_ptr<MelLib::GameObject>& pObject , const std::string& objectType)
{
	if (!isEdit)return;
	
	// C++20のcontainsに置き換えできる
	if (pObjects.find(objectType) != pObjects.end()) 
	{
		std::string text = "シーンエディタには既に"+ objectType + "の名前で他のオブジェクトが登録されています。\n";
		OutputDebugStringA(text.c_str());
		return;
	}

	pObjects.try_emplace(objectType, std::vector<std::shared_ptr<MelLib::GameObject>>());
	pObjects[objectType].push_back(pObject);
	//test.push_back(pObject);
	objectOrderDatas.try_emplace(pObjects.size() - 1, objectType);

}

void MelLib::SceneEditer::Update()
{
	if (!isEdit)return;
	if (pObjects.size() == 0 || !ImguiManager::GetInstance()->GetReleaseDrawFrag())return;

#pragma region 選択


	ImguiManager::GetInstance()->BeginDrawWindow("Edit");

	// キーの数だけラジオボタン描画
	int count = 0;
	for (const auto& object : pObjects)
	{
		ImguiManager::GetInstance()->DrawRadioButton(object.first, selectType, count);
		count++;
	}

	// ラジオボタンで選択したオブジェクト一覧の参照
	std::vector < std::shared_ptr<MelLib::GameObject>>& refObjects = pObjects[objectOrderDatas[selectType]];

	// ここにスライダー作成処理
	int sliderNum = 0;
	ImguiManager::GetInstance()->DrawSliderInt("Object", sliderNum, 0, refObjects.size() - 1);

	// 選ばれたオブジェクトのポインタをpSelectObjectに代入
	pSelectObject = refObjects[sliderNum].get();

	

#pragma endregion

	SelectObjectUpdate();
	ImguiManager::GetInstance()->EndDrawWindow();

	// 追加
	if (Input::KeyTrigger(DIK_SPACE)) 
	{
		// GetNewPtr関数でメモリ確保
		std::shared_ptr<GameObject>pObject = pSelectObject->GetNewPtr();

		if (pObject) 
		{
			// 追加時に座標とかセットし直すのめんどいから
			// pSelectObjectを管理クラスに追加して
			// 戻り値のを新たにmapに入れてもいいかも
			// どっちみちセットしないといけないから
			pObject->SetPosition(pSelectObject->GetPosition());
			pObject->SetAngle(pSelectObject->GetAngle());
			pObject->SetScale(pSelectObject->GetScale());

			// 追加
			GameObjectManager::GetInstance()->AddObject(pObject);
		}
		else 
		{
			std::string text = typeid(*pObject).name();
			text += "のGetNewPtrがnullptrを返しました。オブジェクトが追加できません。\n";
			OutputDebugStringA(text.c_str());
		}
	}

	// 保存
	if ((Input::KeyState(DIK_LCONTROL) || (Input::KeyState(DIK_RCONTROL)) && Input::KeyTrigger(DIK_S)))Save();
}

void MelLib::SceneEditer::Draw()
{
	if (!isEdit)return;
	if (pObjects.size() == 0 || !ImguiManager::GetInstance()->GetReleaseDrawFrag())return;

	pSelectObject->Draw();
}

void MelLib::SceneEditer::SetEditFlag(const bool flag)
{
	if (!releaseEdit) return;
	isEdit = flag;
}
