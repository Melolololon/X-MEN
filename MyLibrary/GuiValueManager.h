#pragma once
#include<string>
#include<unordered_map>
#include<vector>
#include<typeinfo>

#include"GuiValue.h"
namespace MelLib
{
	// .guid 仕様
	// ファイル名　ウィンドウ名
	// 
	// 内部(ラベル分存在)
	// ラベル
	// 区切り-1
	// 名前復元用乱数
	// 値の型
	// 値
	// 終了判別値(-1がまだ続く。-2が末尾)
	//


	class GuiValueManager
	{
	private:

		GuiValueManager();
		~GuiValueManager() {}

		// createWindowNamesに渡された名前がなかった場合追加
		void AddCreateWindowName(const std::string& windowName);


		/// <summary>
		/// 
		/// </summary>
		/// <param name="windowName">ウィンドウ名</param>
		/// <param name="lavel">ラベル</param>
		/// <param name="data">char*に変換したデータのポインタの参照</param>
		/// <param name="dataSize">データサイズ</param>
		/// <param name="refFlag">changeFlag(戻す用)</param>
		void Save(const std::string& windowName, const std::string& lavel, const char*& data, const type_info& type,size_t dataSize,bool& refFlag);

		void Load();

	private:

		static const std::string DATA_FORMAT;
		static const std::unordered_map<std::string, char>DATA_FORMAT_STR;

		std::unordered_map<std::string, std::unordered_map<std::string, GuiInt*>>intValues;
		std::unordered_map<std::string, std::unordered_map<std::string, GuiFloat*>>floatValues;
		std::unordered_map<std::string, std::unordered_map<std::string, GuiVector3*>>vector3Values;
		std::unordered_map<std::string, std::unordered_map<std::string, GuiBool*>>boolValues;

		std::vector<std::string>createWindowNames;

		// std::unordered_map < Window名, std::unordered_map<ラベル名, データ>>
		//　読み込んだデータ
		std::unordered_map < std::string, std::unordered_map<std::string, std::string>>datas;

	public:
		GuiValueManager(GuiValueManager& m) = delete;
		GuiValueManager& operator=(GuiValueManager& m) = delete;
		static GuiValueManager* GetInstance();

		void AddGuiValue(GuiInt* pGuiValue, const std::string& windowName,const std::string& lavel);
		void AddGuiValue(GuiFloat* pGuiValue, const std::string& windowName,const std::string& lavel);
		void AddGuiValue(GuiVector3* pGuiValue, const std::string& windowName,const std::string& lavel);
		void AddGuiValue(GuiBool* pGuiValue, const std::string& windowName,const std::string& lavel);

		void EraseGuiValue(const type_info& type,const std::string& windowName, const std::string& lavel);

		void Initialize();
		void Update();

		
		void GetGuiData(int& refInt, const std::string& windowName, const std::string& lavel)const;
		void GetGuiData(float& refFloat,const std::string& windowName, const std::string& lavel)const;
		void GetGuiData(bool& refFlag, const std::string& windowName, const std::string& lavel)const;
		void GetGuiData(Vector3& refVectior3,const std::string& windowName, const std::string& lavel)const;
	};

}