#pragma once
#include<string>
#include<unordered_map>
#include<vector>

#include"Vector.h"

// struct�ł�����?
// �ݒ肵���l�������o����悤�ɁA�t�@�C���̓E�B���h�E���Ƃł���?
// �e�X�g�v���C���͕ϐ��̒l���ς���Ă��e���󂯂Ȃ��悤��
// �e�X�g�v���C���͕ϐ��̒l�ς����邯�ǁA�G�f�B�b�g�ɖ߂�����SET_VALUE�ɖ߂��悤�ɂ���
// �ǂݍ��񂾎��ɒl��v���Ȃ�������ǂݍ��񂾂ق���D��(Unity�Ɠ����ɂ���)

// �R���X�g���N�^�Ńf�[�^����������ǂݍ��ނ悤�ɂ���

namespace MelLib 
{
	class GuiOption
	{
	private:
		std::string exportPath;

		GuiOption(){}
		~GuiOption() {}
	public:
		GuiOption(GuiOption& o) = delete;
		GuiOption& operator=(GuiOption& o) = delete;
		static GuiOption* GetInstance();

		/// <summary>
		/// Gui�Őݒ肵���l�̏��̏o�͐��ݒ肵�܂��B
		/// </summary>
		/// <param name="path"></param>
		void SetGuiDataExportPath(const std::string& path) { exportPath = path; }
		std::string GetGuiDataExportPath() { return exportPath; }

	};

	// Unity��SerializeField�݂����Ȃ��
	class GuiInt
	{
	private:
		int value = 0;
		const int MIN_VALUE = 0;
		const int MAX_VALUE = 0;
		const int SET_VALUE = 0;
		const std::string WINDOW_NAME;
		const std::string LAVEL;
		
	public:
		
		/// <summary>
		/// 
		/// </summary>
		/// <param name="value"></param>
		/// <param name="windowName"></param>
		/// <param name="lavel"></param>
		/// <param name="minNumber"></param>
		/// <param name="maxNumber"></param>
		GuiInt(int value, const std::string& windowName, const std::string& lavel,int minNumber,int maxNumber);
		~GuiInt();
		
		void operator=(const int num) { this->value = num; }

		void operator++() { value++; }
		void operator--() { value--; }

		
		int GetValue()const { return value; }
		int GetMaxValue() const { return MAX_VALUE; }
		int GetMinValue()const { return MIN_VALUE; }

	};

	class GuiFloat
	{
	private:
		float value = 0.0f;
		const float MIN_VALUE = 0.0f;
		const float MAX_VALUE = 0.0f;
		const float SET_VALUE = 0.0f;
		const std::string WINDOW_NAME;
		const std::string LAVEL;

	public:

		/// <summary>
		/// 
		/// </summary>
		/// <param name="value"></param>
		/// <param name="windowName"></param>
		/// <param name="lavel"></param>
		/// <param name="minNumber"></param>
		/// <param name="maxNumber"></param>
		GuiFloat(float value, const std::string& windowName, const std::string& lavel, float minNumber, float maxNumber);
		~GuiFloat();
		void operator=(const float num) { this->value = num; }

		void operator++() { value++; }
		void operator--() { value--; }

		float GetValue()const { return value; }
		float GetMaxValue() const { return MAX_VALUE; }
		float GetMinValue()const { return MIN_VALUE; }

	};

	class GuiVector3
	{
	private:
		MelLib::Vector3 value = 0.0f;
		const float MIN_VALUE = 0.0f;
		const float MAX_VALUE = 0.0f;
		const MelLib::Vector3 SET_VALUE = 0.0f;
		const std::string WINDOW_NAME;
		const std::string LAVEL;

	public:

		/// <summary>
		/// 
		/// </summary>
		/// <param name="value"></param>
		/// <param name="windowName"></param>
		/// <param name="lavel"></param>
		/// <param name="minNumber"></param>
		/// <param name="maxNumber"></param>
		GuiVector3(MelLib::Vector3 value, const std::string& windowName, const std::string& lavel, float minNumber, float maxNumber);
		~GuiVector3();
		void operator=(const MelLib::Vector3& num) { this->value = num; }

	/*	void operator++() { value++; }
		void operator--() { value--; }*/

		MelLib::Vector3 GetValue()const { return value; }
		float GetMaxValue() const { return MAX_VALUE; }
		float GetMinValue()const { return MIN_VALUE; }

	};

	class GuiBool
	{
	private:
		bool value = false;
		const bool SET_VALUE = false;
		const std::string WINDOW_NAME;
		const std::string LAVEL;

	public:

		/// <summary>
		/// 
		/// </summary>
		/// <param name="value"></param>
		/// <param name="windowName"></param>
		/// <param name="lavel"></param>
		/// <param name="minNumber"></param>
		/// <param name="maxNumber"></param>
		GuiBool(bool value, const std::string& windowName, const std::string& lavel);
		~GuiBool();
		void operator=(const bool value) { this->value = value; }

		bool GetValue()const { return value; }

	};
}