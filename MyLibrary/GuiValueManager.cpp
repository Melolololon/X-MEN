#include "GuiValueManager.h"
#include"ImguiManager.h"
#include"Random.h"
#include"StringSupport.h"
#include<fstream>
#include<filesystem>

const std::string MelLib::GuiValueManager::DATA_FORMAT = ".guid";
const std::unordered_map<std::string, char>MelLib::GuiValueManager::DATA_FORMAT_STR =
{
	{"int",'i'},
	{"bool",'b'},
	{"Vector3",'3'},
	{"float",'f'},
};

MelLib::GuiValueManager::GuiValueManager() 
{
	// �����œǂݍ��ނƊJ���̎��s����
	//Load();
}

void MelLib::GuiValueManager::AddCreateWindowName(const std::string& windowName)
{
	for (const auto& name : createWindowNames)
	{
		if (name == windowName)return;
	}
	createWindowNames.push_back(windowName);
}



void MelLib::GuiValueManager::Save(const std::string& windowName, const std::string& lavel, const char*& data, const type_info& type,const size_t dataSize, bool& refFlag)
{
	

	// �폜���ꂽGui�̃p�����[�^�͏����o���Ȃ��悤�ɂ���

#pragma region ��


	//bool fileOpen = true;
	//std::fstream file(EXPORT_PATH,std::ios_base::binary|std::ios_base::in);
	//if (!file)
	//{
	//	fileOpen = false;
	//}


	//std::string dataName = "[" + lavel + "]";

	//size_t fileSize = 0;
	//file.seekg(std::ios_base::end);
	//fileSize = file.tellg();
	//file.seekg(std::ios_base::beg);

	//// �����������O�̖��[�ʒu(]�̗�)
	//size_t dataEndPos = 0;

	//bool matchName = false;
	//while (file)
	//{
	//	// �m�F�I���������܂Ŋm�F�����甲����
	//	size_t currentPos = file.tellg();
	//	if (matchName || currentPos == fileSize)break;

	//	std::string str;
	//	char c = '_';
	//	file.read(&c, 1);

	//	// [��ǂݍ��񂾂����
	//	if (c == '[') 
	//	{

	//		str += c;

	//		// ]�܂Ŏ擾����B
	//		while (1) 
	//		{
	//			file.read(&c, 1);
	//			str += c;
	//			if (str[str.size() - 1] == ']')
	//			{
	//				// ���O����v�������d���[�v�𔲂���B
	//				if (str == dataName)
	//				{
	//					matchName = true;
	//					dataEndPos = file.tellg();
	//				}
	//				break;
	//			}
	//		}
	//	}
	//}
	//file.close();

	//std::fstream oFile(EXPORT_PATH,std::ios_base::binary | std::ios_base::out);
	//// ���݂�����㏑��
	//if (matchName) 
	//{
	//	// �V�[�N���ď�������
	//	//oFile.seekg(0, dataEndPos);
	//	oFile.write(data,dataSize);
	//}
	//else // ���O������������
	//{
	//	// �t�@�C�������݂��Ă����疖���ɃV�[�N
	//	if(fileOpen)oFile.seekg(std::ios_base::end);

	//	// ���O�ƃf�[�^����������
	//	oFile.write(dataName.c_str(), dataName.size());
	//	oFile.write(data, dataSize);

	//}

	//oFile.close();

#pragma endregion

	// �V�[�N���Ĉꕔ��������������ɂ́A��Ƀt�@�C�����J���Ă��������Ȃ�
	// �J�������Ə����邩��
	
	// ��U�E�B���h�E���S�������o���ł���������

	// char��128MAX������100�����ăI�[�o�[�t���[�����Ă�-1�ɂ͂Ȃ�Ȃ�����-1����؂�Ƃ��Ďg���Ă���
	// �I�[��-2
	
	std::string param;
	//param += -1;

	// ����΍��p����
	char ran = static_cast<char>(Random::GetRandomNumber(100) + 1);

	std::string addLavel = lavel;
	for (auto& c : addLavel) c += ran;
	param += addLavel;

	param += -1;
	param += ran;

	if (type == typeid(int))param += DATA_FORMAT_STR.at("int");
	else if (type == typeid(float))param += DATA_FORMAT_STR.at("float");
	else if (type == typeid(bool))param += DATA_FORMAT_STR.at("bool");
	else if (type == typeid(MelLib::Vector3))param += DATA_FORMAT_STR.at("Vector3");

	for (size_t i = 0; i < dataSize; i++)
	{
		param += data[i];
	}

	datas[windowName][lavel] = param;

	const std::string EXPORT_PATH = GuiOption::GetInstance()->GetGuiDataPath() + windowName + DATA_FORMAT;

	// �����o��
	std::ofstream file(EXPORT_PATH);

	int loopNum = 0; 
	for (const auto& d : datas[windowName])
	{
	/*	std::string lavel = d.first;
		for (auto& c : lavel) c += ran;*/

		//file.write(lavel.c_str(), lavel.size());
		file.write(d.second.c_str(), d.second.size());


		if (loopNum != datas[windowName].size() - 1) 
		{
			char kugiri = -1;
			file.write(&kugiri, 1);
		}
		else 
		{
			char kugiri = -2;
			file.write(&kugiri, 1);
		}
		loopNum++;
	}

	file.close();
	// Imgui�̕ύX�m�F�t���O��false��
	refFlag = false;
}

void MelLib::GuiValueManager::Load()
{



	std::string importPath = GuiOption::GetInstance()->GetGuiDataPath();

	if (importPath.size() == 0)importPath = std::filesystem::current_path().string();

	for (const auto& p : std::filesystem::directory_iterator(importPath))
	{
		std::string filePath = p.path().string();
		if (filePath.find(DATA_FORMAT) != std::string::npos) 
		{
			std::string fileName = StringSupport::ExtractionFileName(filePath);
			
			fileName.erase(fileName.begin() + (fileName.size() - DATA_FORMAT.size()), fileName.end());

			std::ifstream file(filePath);

			while (1) 
			{

				std::string lavel;
				char c = 0;
				while (1)
				{
					file.read(&c, 1);
					if (c == -1)break;
					lavel += c;
				}

				std::string param = lavel;
				param += -1;

				// �ݒ肵���������擾
				char randNum = 0;
				file.read(&randNum, 1);
				// ���Z���Ă����Ƃ������O�ɖ߂�
				for (auto& c : lavel)c -= randNum;
				param += randNum;


				// �^�̎擾
				char formatChar = 0;
				file.read(&formatChar, 1);
				param += formatChar;

				if (formatChar == DATA_FORMAT_STR.at("int")
					|| formatChar == DATA_FORMAT_STR.at("float"))
				{
					char value[4];
					file.read(value, 4);

					for (int i = 0; i < 4; i++)param += value[i];
					int z = 0;
				}
				else if (formatChar == DATA_FORMAT_STR.at("bool"))
				{
					char value;
					file.read(&value, 1);
					param += value;
				}
				else if (formatChar == DATA_FORMAT_STR.at("Vector3"))
				{
					char value[sizeof(Vector3)];
					file.read(value, sizeof(Vector3));
					for (int i = 0; i < sizeof(Vector3); i++)param += value[i];
				}

				datas[fileName][lavel] = param;

				// ��؂��-1���Ȃ������甲����
				char kugiri = 0;
				file.read(&kugiri, 1);
				if (kugiri == -2)break;
			}

			file.close();
		}
	}
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

void MelLib::GuiValueManager::Initialize()
{
	Load();
}

void MelLib::GuiValueManager::Update()
{
	// �����[�X���ɕ`�悵�Ȃ��ݒ肾������return
	if (!ImguiManager::GetInstance()->GetReleaseDrawFrag())return;

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
				Save(name, LAVEL, data, typeid(num),sizeof(num), changeFlag);
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
				Save(name, LAVEL, data, typeid(num), sizeof(num), changeFlag);
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
				Save(name, LAVEL, data, typeid(num), sizeof(num), changeFlag);
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
				Save(name, LAVEL, data, typeid(flag), sizeof(flag), changeFlag);
				guiBool = flag;
			}
		}

		ImguiManager::GetInstance()->EndDrawWindow();
	}
}

void MelLib::GuiValueManager::GetGuiData(int& refInt, const std::string& windowName, const std::string& lavel) const
{
	// ���݂��邩�m�F
	if (datas.find(windowName) == datas.end())return;
	if (datas.at(windowName).find(lavel) == datas.at(windowName).end())return;

	// ��������i�[
	std::string param = datas.at(windowName).at(lavel);
	char data[4];
	for (int i = 0; i < 4; i++)data[i] = param[param.size() - 4 + i];

	
	int* pValue = reinterpret_cast<int*>(data);
	refInt = *pValue;
}

void MelLib::GuiValueManager::GetGuiData(float& refFloat, const std::string& windowName, const std::string& lavel) const
{
	// ���݂��邩�m�F
	if (datas.find(windowName) == datas.end())return;
	if (datas.at(windowName).find(lavel) == datas.at(windowName).end())return;

	// ��������i�[
	std::string param = datas.at(windowName).at(lavel);
	char data[4];
	for (int i = 0; i < 4; i++)data[i] = param[param.size() - 4 + i];


	float* pValue = reinterpret_cast<float*>(data);
	refFloat = *pValue;
}

void MelLib::GuiValueManager::GetGuiData(bool& refFlag, const std::string& windowName, const std::string& lavel) const
{
	// ���݂��邩�m�F
	if (datas.find(windowName) == datas.end())return;
	if (datas.at(windowName).find(lavel) == datas.at(windowName).end())return;

	// ��������i�[
	std::string param = datas.at(windowName).at(lavel);
	char flag = param[param.size() - 1];
	refFlag = static_cast<bool>(flag);
}

void MelLib::GuiValueManager::GetGuiData(Vector3& refVectior3, const std::string& windowName, const std::string& lavel) const
{
	// ���݂��邩�m�F
	if (datas.find(windowName) == datas.end())return;
	if (datas.at(windowName).find(lavel) == datas.at(windowName).end())return;

	// ��������i�[
	std::string param = datas.at(windowName).at(lavel);
	char data[sizeof(Vector3)];
	for (int i = 0; i < sizeof(Vector3); i++)data[i] = param[param.size() - sizeof(Vector3) + i];

	Vector3* pValue = reinterpret_cast<Vector3*>(data);
	refVectior3 = *pValue;
}

