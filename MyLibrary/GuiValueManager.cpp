#include "GuiValueManager.h"
#include"ImguiManager.h"
#include<fstream>

void MelLib::GuiValueManager::AddCreateWindowName(const std::string& windowName)
{
	for (const auto& name : createWindowNames)
	{
		if (name == windowName)return;
	}
	createWindowNames.push_back(windowName);
}



void MelLib::GuiValueManager::Save(const std::string& windowName, const std::string& lavel, const char*& data, size_t dataSize, bool& refFlag)
{
	const std::string EXPORT_PATH = GuiOption::GetInstance()->GetGuiDataExportPath() + windowName + ".guid";

	std::ifstream file(EXPORT_PATH,std::ios_base::binary);
	if (!file) 
	{
		std::ofstream oFile(EXPORT_PATH);
		oFile.close();
		file.open(EXPORT_PATH, std::ios_base::binary);
	}

	std::string dataName = "[" + lavel + "]";

	size_t fileSize = 0;
	file.seekg(std::ios_base::end);
	fileSize = file.tellg();
	file.seekg(std::ios_base::beg);

	// �����������O�̖��[�ʒu(]�̗�)
	size_t dataEndPos = 0;

	bool matchName = false;
	while (1) 
	{
		// �m�F�I���������܂Ŋm�F�����甲����
		size_t currentPos = file.tellg();
		if (matchName || currentPos == fileSize)break;

		std::string str;
		char c = '_';
		file.read(&c, 1);

		// [��ǂݍ��񂾂����
		if (c == '[') 
		{
			

			// ]�܂Ŏ擾����B
			while (1) 
			{
				str += c;
				file.read(&c, 1);
				if (str[str.size() - 1] == ']')
				{
					// ���O����v�������d���[�v�𔲂���B
					if (str == dataName)
					{
						matchName = true;
						dataEndPos = file.tellg();
					}
					break;
				}
			}
		}
	}
	file.close();

	std::fstream oFile(EXPORT_PATH);
	// ���݂�����㏑��
	if (matchName) 
	{
		// �V�[�N���ď�������
		oFile.seekg(0, dataEndPos);
		oFile.write(data,dataSize);
	}
	else // ���O�����������疖���ɒǉ�
	{
		// �����ɃV�[�N
		oFile.seekg(std::ios_base::end);

		// ���O�ƃf�[�^����������
		oFile.write(dataName.c_str(), dataName.size());
		oFile.write(data, dataSize);
	}

	oFile.close();

	// Imgui�̕ύX�m�F�t���O��false��
	refFlag = false;
}


MelLib::GuiValueManager* MelLib::GuiValueManager::GetInstance()
{
	static GuiValueManager m;
	return &m;
}

void MelLib::GuiValueManager::AddGuiValue(GuiInt* pGuiValue, const std::string& windowName, const std::string& lavel)
{
	intValues[windowName].try_emplace(lavel, pGuiValue);
	AddCreateWindowName(windowName);
	
}

void MelLib::GuiValueManager::AddGuiValue(GuiFloat* pGuiValue, const std::string& windowName, const std::string& lavel)
{
	floatValues[windowName].try_emplace(lavel, pGuiValue);
	AddCreateWindowName(windowName);
}

void MelLib::GuiValueManager::AddGuiValue(GuiVector3* pGuiValue, const std::string& windowName, const std::string& lavel)
{
	vector3Values[windowName].try_emplace(lavel, pGuiValue);
	AddCreateWindowName(windowName);
}

void MelLib::GuiValueManager::AddGuiValue(GuiBool* pGuiValue, const std::string& windowName, const std::string& lavel)
{
	boolValues[windowName].try_emplace(lavel, pGuiValue);
	AddCreateWindowName(windowName);
}

void MelLib::GuiValueManager::EraseGuiValue(const type_info& type, const std::string& windowName, const std::string& lavel)
{
	if (type == typeid(int)) 
	{
		intValues[windowName].erase(lavel);
		if (intValues[windowName].size() == 0)intValues.erase(windowName);
	}
	else if (type == typeid(float))
	{
		floatValues[windowName].erase(lavel);
		if (floatValues[windowName].size() == 0)floatValues.erase(windowName);
	}
	else if (type == typeid(MelLib::Vector3))
	{
		vector3Values[windowName].erase(lavel);
		if (vector3Values[windowName].size() == 0)vector3Values.erase(windowName);
	}
	else if (type == typeid(bool))
	{
		boolValues[windowName].erase(lavel);
		if (boolValues[windowName].size() == 0)boolValues.erase(windowName);
	}
}

void MelLib::GuiValueManager::Update()
{
	// �O��ނ̔z�񌩂āAWindow�����ꏏ��������1�̃E�B���h�E�ɂ܂Ƃ߂�
	for (const auto& name : createWindowNames)
	{
		ImguiManager::GetInstance()->BeginDrawWindow(name);
		bool changeFlag = false;

		// int
		const std::unordered_map<std::string, GuiInt*>& refInts = intValues[name];
		for (auto& value : refInts) 
		{
			const std::string& LAVEL = value.first;
			GuiInt& guiInt = *value.second;
			int num = guiInt.GetValue();
			changeFlag = ImguiManager::GetInstance()->DrawSliderInt(LAVEL, num, guiInt.GetMinValue(), guiInt.GetMaxValue());
			
			if (changeFlag) 
			{
				const char* data = reinterpret_cast<char*>(&num);
				Save(name, LAVEL, data, sizeof(num), changeFlag);
				guiInt = num;
			}
		}

		// float 
		const std::unordered_map<std::string, GuiFloat*>& refFloats = floatValues[name];
		for (auto& value : refFloats)
		{
			const std::string& LAVEL = value.first;
			GuiFloat& guiFloat = *value.second;
			float num = guiFloat.GetValue();
			changeFlag = ImguiManager::GetInstance()->DrawSliderFloat(LAVEL, num, guiFloat.GetMinValue(), guiFloat.GetMaxValue());
			if (changeFlag)
			{
				const char* data = reinterpret_cast<char*>(&num);
				Save(name, LAVEL, data, sizeof(num), changeFlag);
				guiFloat = num;
			}
		}

		// Vector3
		const std::unordered_map<std::string, GuiVector3*>& refVector3 = vector3Values[name];
		for (auto& value : refVector3)
		{
			const std::string& LAVEL = value.first;
			GuiVector3& guiVector3 = *value.second;
			MelLib::Vector3 num = guiVector3.GetValue();
			changeFlag = ImguiManager::GetInstance()->DrawSliderVector3(LAVEL, num, guiVector3.GetMinValue(), guiVector3.GetMaxValue());
			if (changeFlag)
			{
				const char* data = reinterpret_cast<char*>(&num);
				Save(name, LAVEL, data, sizeof(num), changeFlag);
				guiVector3 = num;
			}
		}

		// bool
		const std::unordered_map<std::string, GuiBool*>& refBool = boolValues[name];
		for (auto& value : refBool)
		{
			const std::string& LAVEL = value.first;
			GuiBool& guiBool = *value.second;
			bool flag = guiBool.GetValue();
			changeFlag = ImguiManager::GetInstance()->DrawCheckBox(LAVEL, flag);
			if (changeFlag)
			{
				const char* data = reinterpret_cast<char*>(&flag);
				Save(name, LAVEL, data, sizeof(flag), changeFlag);
				guiBool = flag;
			}
		}

		ImguiManager::GetInstance()->EndDrawWindow();
	}
}
