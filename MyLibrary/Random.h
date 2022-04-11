#pragma once
#include"Vector.h"
#include<vector>

namespace MelLib
{

	//乱数クラス
	class Random
	{
	private:
		Random() {}
		~Random() {}

		using UINT = unsigned int;
	public:

		static void Initialize();

		/// <summary>
		/// シード値をセットします。
		/// </summary>
		/// <param name="seed"></param>
		static void SetSeed(const UINT seed);

		/// <summary>
		/// シード値に時間を使用するようにします。
		/// </summary>
		static void SetSeedTime();

		/// <summary>
		///	乱数を取得します。取得できる値はrand関数と同じです。
		/// </summary>
		/// <param name="num"></param>
		/// <returns></returns>
		static int GetRandomNumber(const int num);

		/// <summary>
		/// 指定した範囲内の数のどれかを取得します。
		/// </summary>
		/// <param name="start"></param>
		/// <param name="end"></param>
		/// <returns></returns>
		static int GetRandomNumberRangeSelect(const int start, const int end);

		/// <summary>
		/// numsの中からどれか1つをランダムで返します。
		/// </summary>
		/// <param name="nums"></param>
		/// <returns></returns>
		static int GetRandomNumberSetNumber(const std::vector<int>& nums);

		/// <summary>
		/// 小数の乱数を取得します。
		/// </summary>
		/// <param name="num"></param>
		/// <param name="digits">桁数(1から4)</param>
		/// <returns></returns>
		static float GetRandomFloatNumber(const float num, const int digits);

		/// <summary>
		/// 指定した範囲内の数のどれかを取得します。
		/// </summary>
		/// <param name="num"></param>
		/// <param name="digits">桁数(1から4)</param>
		/// <returns></returns>
		static float GetRandomFloatNumberRangeSelect(const float start, const float end, const int digits);

		/// <summary>
	/// numsの中からどれか1つをランダムで返します。
	/// </summary>
	/// <param name="nums"></param>
	/// <returns></returns>
		static float GetRandomNumberSetFloatNumber(const std::vector<float>& nums);

	

	};

}
