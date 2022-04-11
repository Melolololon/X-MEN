#pragma once
#include<limits>
#include<vector>

//ポインタを配列にまとめられない(瞬間の判定をとれない)から、クラステンプレートやめた
//コンストラクタや関数などでパラメータを一気にセットできるようにする?

namespace MelLib
{
	//カウンタークラス
	class Counter
	{
	private:
		static std::vector<Counter*>pCounter;

		int count = 0;
		int preCount = -1;
		int resetMaxNum = INT_MAX;
		int resetMinNum = INT_MIN;
		int maxCount = INT_MAX;
		int minCount = INT_MIN;

		//次のフレームで自動的にリセットするかどうか
		bool countReset = false;

		void Update();

	public:

		Counter();
		Counter(const int num);
		~Counter();

		void operator++() { count++; }
		void operator--() { count--; }

		void IncrementCount() { count++; }
		void DecrementCount() { count--; }
		void AddCount(const int num) { count += num; }
		void SubCount(const int num) { count -= num; }
		void ResetMaxCount() { count = resetMaxNum; }
		void ResetMinCount() { count = resetMinNum; }
		void ResetCountZero() { count = 0; }

#pragma region セット
		void SetCount(const int num) { count = num; }
		void SetResetMaxNum(const int num) { resetMaxNum = num; }
		void SetResetMinNum(const int num) { resetMaxNum = num; }

		/// <summary>
		///	LibraryクラスのLoopStartProcess関数呼び出し時にカウントが最大または最小カウント数に達していた場合、
		/// 自動でリセットするかどうかのフラグをセットします。
		/// </summary>
		/// <param name="flag"></param>
		void SetResetFlag(const bool flag) { countReset = flag; }
#pragma endregion


#pragma region ゲット

		int GetCount() { return count; }
		bool GetCountMax() { return count >= maxCount; }
		bool GetCountMin() { return count <= minCount; }
#pragma endregion

		static void AllUpdate();
	};

	//カウンタークラス(float型)
	class CounterF
	{
	private:
		float count = 0;
		float preCount = -1;
		float resetNum = 0;
		float maxCount = FLT_MAX;
		float minCount = FLT_MIN;


	public:
		CounterF() {}
		CounterF(const float num) :count(num), preCount(count - 1) {}


		void operator++() { count++; }
		void operator--() { count--; }

		void IncrementCount() { count++; }
		void DecrementCount() { count--; }
		void AddCount(const float num) { count += num; }
		void SubCount(const float num) { count -= num; }
		void ResetCount() { count = resetNum; }
		void ResetCountZero() { count = 0; }

#pragma region セット
		void SetCount(const float num) { count = num; }
		void SetResetNum(const float num) { resetNum = num; }
#pragma endregion


#pragma region ゲット

		float GetCount() { return count; }

#pragma endregion

	};
}