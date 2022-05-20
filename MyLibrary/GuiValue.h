#pragma once
#include<string>
#include<unordered_map>
#include<vector>

#include"Vector.h"

// structでもいい?
// 設定した値を書き出せるように、ファイルはウィンドウごとでいい?
// テストプレイ中は変数の値が変わっても影響受けないように
// テストプレイ中は変数の値変えられるけど、エディットに戻ったらSET_VALUEに戻すようにする
// 読み込んだ時に値一致しなかったら読み込んだほうを優先(Unityと同じにする)

// コンストラクタでデータがあったら読み込むようにする

namespace MelLib 
{
	class GuiOption
	{
	private:
		std::string inOutPath;

		GuiOption(){}
		~GuiOption() {}
	public:
		GuiOption(GuiOption& o) = delete;
		GuiOption& operator=(GuiOption& o) = delete;
		static GuiOption* GetInstance();

		/// <summary>
		/// Guiで設定した値の情報の入出力先を設定します。
		/// </summary>
		/// <param name="path"></param>
		void SetGuiDataPath(const std::string& path) { inOutPath = path; }
		std::string GetGuiDataPath() { return inOutPath; }

	};

	// UnityのSerializeFieldみたいなやつ
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
		GuiInt& operator=(GuiInt& value) 
		{
			*this = value;
			return value;
		}

		void operator++() { value++; }
		void operator--() { value--; }

		
		int GetValue()const { return value; }
		int GetMaxValue() const { return MAX_VALUE; }
		int GetMinValue()const { return MIN_VALUE; }

		/// <summary>
		/// 読み込まれた値をセットします。読み込んだ値をセットしたい場合や
		/// 初期化処理が始まる前にコンストラクタが呼び出されてしまう場合にお使いください。
		/// </summary>
		void SetLoadData();
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

		/// <summary>
		/// 読み込まれた値をセットします。読み込んだ値をセットしたい場合や
		/// 初期化処理が始まる前にコンストラクタが呼び出されてしまう場合にお使いください。
		/// </summary>
		void SetLoadData();
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
		GuiVector3(const MelLib::Vector3& value, const std::string& windowName, const std::string& lavel, float minNumber, float maxNumber);
		~GuiVector3();
		void operator=(const MelLib::Vector3& num) { this->value = num; }

	/*	void operator++() { value++; }
		void operator--() { value--; }*/

		MelLib::Vector3 GetValue()const { return value; }
		float GetMaxValue() const { return MAX_VALUE; }
		float GetMinValue()const { return MIN_VALUE; }
		
		/// <summary>
		/// 読み込まれた値をセットします。読み込んだ値をセットしたい場合や
		/// 初期化処理が始まる前にコンストラクタが呼び出されてしまう場合にお使いください。
		/// </summary>
		void SetLoadData();
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

		/// <summary>
		/// 読み込まれた値をセットします。読み込んだ値をセットしたい場合や
		/// 初期化処理が始まる前にコンストラクタが呼び出されてしまう場合にお使いください。
		/// </summary>
		void SetLoadData();
	};
}