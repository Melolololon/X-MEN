#pragma once
#include<string>
#include<unordered_map>
#include<vector>
#include<typeinfo>

#include"GuiValue.h"
namespace MelLib
{
	class GuiValueManager
	{
	private:
		GuiValueManager() {}
		~GuiValueManager() {}

		// createWindowNames�ɓn���ꂽ���O���Ȃ������ꍇ�ǉ�
		void AddCreateWindowName(const std::string& windowName);
	private:
		// bool�^�����

		std::unordered_map<std::string, std::unordered_map<std::string, GuiInt*>>intValues;
		std::unordered_map<std::string, std::unordered_map<std::string, GuiFloat*>>floatValues;
		std::unordered_map<std::string, std::unordered_map<std::string, GuiVector3*>>vector3Values;
		std::unordered_map<std::string, std::unordered_map<std::string, GuiBool*>>boolValues;

		std::vector<std::string>createWindowNames;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="windowName">�E�B���h�E��</param>
		/// <param name="lavel">���x��</param>
		/// <param name="data">char*�ɕϊ������f�[�^�̃|�C���^�̎Q��</param>
		/// <param name="dataSize">�f�[�^�T�C�Y</param>
		/// <param name="refFlag">changeFlag(�߂��p)</param>
		void Save(const std::string& windowName, const std::string& lavel, const char*& data, size_t dataSize,bool& refFlag);

		

	public:
		GuiValueManager(GuiValueManager& m) = delete;
		GuiValueManager& operator=(GuiValueManager& m) = delete;
		static GuiValueManager* GetInstance();

		void AddGuiValue(GuiInt* pGuiValue, const std::string& windowName,const std::string& lavel);
		void AddGuiValue(GuiFloat* pGuiValue, const std::string& windowName,const std::string& lavel);
		void AddGuiValue(GuiVector3* pGuiValue, const std::string& windowName,const std::string& lavel);
		void AddGuiValue(GuiBool* pGuiValue, const std::string& windowName,const std::string& lavel);

		void EraseGuiValue(const type_info& type,const std::string& windowName, const std::string& lavel);

		void Update();

		
	};

}