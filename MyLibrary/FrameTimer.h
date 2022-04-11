#pragma once

#include<vector>

//演算子オーバーロードする?
//マネージャーでのUpdateとストップフラグ関数やめて、
//関数呼び出してる間ストップしない(Updateする)って仕様にする?


namespace MelLib
{
	//フレームタイマー(プログラム依存)
	class FrameTimer
	{
	private:
		//タイマーのポインタの配列(Update用)
		static std::vector<FrameTimer*>pTimers;

		//現在の時間
		int time = 0;
		//前フレームの時間
		int preTime = 0;
		//最大計測時間に達したときに現在の時間を上書きする時間
		int resetMaxTime = 0;
		//最小計測時間に達したときに現在の時間を上書きする時間
		int resetMinTime = 0;
		//最大計測時間
		int maxTime = INT_MAX;
		//最小計測時間
		int minTime = INT_MIN;
		//計測するかどうか
		bool isStop = true;
		//最大計測時間に達した瞬間かどうか
		bool timeMaxMoment = false;
		//最小計測時間に達した瞬間かどうか
		bool timeMinMoment = false;

		//時間を加算するか減算するか
		bool isDecrement = false;
		//リセットするかどうか
		bool resetFlag = false;

	private:
		void Update();
	public:

		FrameTimer();

		~FrameTimer();

	

		static void AllUpdate();

		/// <summary>
		/// 現在の時間を最大計測時間に達したときの時間にします。
		/// </summary>
		void ResetMaxTime() { time = resetMaxTime; }

		/// <summary>
		/// 現在の時間を最小計測時間に達したときの時間にします。
		/// </summary>
		void ResetMinTime() { time = resetMinTime; }

		/// <summary>
		/// 時間を0でリセットします。
		/// </summary>
		void ResetTimeZero() { time = 0; }

#pragma region ゲット


		/// <summary>
		/// タイマーが停止しているかを返します。
		/// </summary>
		/// <returns></returns>
		bool GetStopFlag()const { return isStop; }

		/// <summary>
		///	現在の時間を返します。
		/// </summary>
		/// <returns></returns>
		int GetNowTime() const { return time; }

		/// <summary>
		/// 最大計測時間を取得します。
		/// </summary>
		/// <returns></returns>
		int GetMaxTime() const { return maxTime; }

		/// <summary>
		/// 最小計測時間を取得します。
		/// </summary>
		/// <returns></returns>
		int GetMinTime() const { return minTime; }

		/// <summary>
		/// 最大計測時間に達したときに現在の時間を上書きする時間を取得します。
		/// </summary>
		/// <returns></returns>
		int GetResetMaxTime()const { return resetMaxTime; }

		/// <summary>
		/// 最小計測時間に達したときに現在の時間を上書きする時間を取得します。
		/// </summary>
		/// <returns></returns>
		int GetResetMinTime()const { return resetMinTime; }

		/// <summary>
		/// 最大計測時間と現在の時間が同じになった瞬間にtrueを返します。
		/// </summary>
		/// <returns></returns>
		bool GetSameAsMaxFlag()const { return timeMaxMoment; }

		/// <summary>
		/// 最小計測時間と現在の時間が同じになった瞬間にtrueを返します。
		/// </summary>
		/// <returns></returns>
		bool GetSameAsMinFlag() const { return timeMinMoment; }

		bool GetMaxJustFlag()const { return time == maxTime; }
		bool GetMinJustFlag()const { return time == minTime; }

		bool GetMaxOverFlag()const { return time >= maxTime; }
		bool GetMinOverFlag()const { return time <= minTime; }

		/// <summary>
		/// 時間がセットした数値の倍数だった場合、trueを返します。
		/// </summary>
		/// <param name="num">数値</param>
		/// <returns></returns>
		bool GetMultipleTimeFlag(const int num)const { return time % num == 0; }

		/// <summary>
		/// セットした時間ごとにフラグをtrueとfalseに切り替えて返します。falseからスタートします。
		/// </summary>
		/// <param name="num">数値</param>
		/// <returns></returns>
		int GetSetByNumberFlag(const int num) const { return time % (num * 2) >= num; }
#pragma endregion

#pragma region セット

		/// <summary>
		/// タイマーを停止するかどうかのフラグをセットします、
		/// </summary>
		/// <param name="flag"></param>
		void SetStopFlag(const bool flag) { isStop = flag; }

		/// <summary>
		/// 最大または最小計測時間に達したときに、時間を自動的にリセットするかどうかのフラグをセットします。
		/// </summary>
		/// <param name="flag"></param>
		void SetResetFlag(const bool flag) { resetFlag = flag; }

		/// <summary>
		/// 時間を加算していくか減算していくかを指定します。
		/// </summary>
		/// <param name="flag"></param>
		void SetDecrementFlag(const bool flag) { isDecrement = flag; }

		/// <summary>
		/// 最大計測時間に達したときに現在の時間をリセットするための時間を設定します。
		/// </summary>
		/// <param name="num"></param>
		void SetResetMaxTime(const int num) { resetMaxTime = num; }

		/// <summary>
		/// 最小計測時間に達したときに現在の時間をリセットするための時間を設定します。
		/// </summary>
		/// <param name="num"></param>
		void SetResetMinTime(const int num) { resetMinTime = num; }

		/// <summary>
		/// 最大計測時間を設定します。
		/// </summary>
		/// <param name="num"></param>
		void SetMaxTime(const int num) { maxTime = num; }

		/// <summary>
		/// 最小計測時間を設定します。
		/// </summary>
		/// <param name="num"></param>
		void SetMinTime(const int num) { minTime = num; }

		/// <summary>
		/// タイマーの現在の時間を設定します。
		/// </summary>
		/// <param name="num"></param>
		void SetNowTime(const int num) { time = num; }
#pragma endregion

	};

}