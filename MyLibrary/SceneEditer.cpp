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
	// �I�u�W�F�N�g�}�l�[�W���[�ɒǉ������I�u�W�F�N�g��imgui�ł������悤��

	std::string name = SceneManager::GetInstance()->GetCurrentSceneName();
	name += ".mlsce";
	std::ofstream file(name);

	// �����o��


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
	
	// C++20��contains�ɒu�������ł���
	if (pObjects.find(objectType) != pObjects.end()) 
	{
		std::string text = "�V�[���G�f�B�^�ɂ͊���"+ objectType + "�̖��O�ő��̃I�u�W�F�N�g���o�^����Ă��܂��B\n";
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

#pragma region �I��


	ImguiManager::GetInstance()->BeginDrawWindow("Edit");

	// �L�[�̐��������W�I�{�^���`��
	int count = 0;
	for (const auto& object : pObjects)
	{
		ImguiManager::GetInstance()->DrawRadioButton(object.first, selectType, count);
		count++;
	}

	// ���W�I�{�^���őI�������I�u�W�F�N�g�ꗗ�̎Q��
	std::vector < std::shared_ptr<MelLib::GameObject>>& refObjects = pObjects[objectOrderDatas[selectType]];

	// �����ɃX���C�_�[�쐬����
	int sliderNum = 0;
	ImguiManager::GetInstance()->DrawSliderInt("Object", sliderNum, 0, refObjects.size() - 1);

	// �I�΂ꂽ�I�u�W�F�N�g�̃|�C���^��pSelectObject�ɑ��
	pSelectObject = refObjects[sliderNum].get();

	

#pragma endregion

	SelectObjectUpdate();
	ImguiManager::GetInstance()->EndDrawWindow();

	// �ǉ�
	if (Input::KeyTrigger(DIK_SPACE)) 
	{
		// GetNewPtr�֐��Ń������m��
		std::shared_ptr<GameObject>pObject = pSelectObject->GetNewPtr();

		if (pObject) 
		{
			// �ǉ����ɍ��W�Ƃ��Z�b�g�������̂߂�ǂ�����
			// pSelectObject���Ǘ��N���X�ɒǉ�����
			// �߂�l�̂�V����map�ɓ���Ă���������
			// �ǂ����݂��Z�b�g���Ȃ��Ƃ����Ȃ�����
			pObject->SetPosition(pSelectObject->GetPosition());
			pObject->SetAngle(pSelectObject->GetAngle());
			pObject->SetScale(pSelectObject->GetScale());

			// �ǉ�
			GameObjectManager::GetInstance()->AddObject(pObject);
		}
		else 
		{
			std::string text = typeid(*pObject).name();
			text += "��GetNewPtr��nullptr��Ԃ��܂����B�I�u�W�F�N�g���ǉ��ł��܂���B\n";
			OutputDebugStringA(text.c_str());
		}
	}

	// �ۑ�
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
