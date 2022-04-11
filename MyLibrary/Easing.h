#pragma once
#include"Vector.h"

namespace MelLib
{
	/*class Easing
	{
	private:
		Easing() {}
		~Easing() {}

		static Vector3 CalcEasing(const Vector3& startPos, const Vector3& endPos, const float t);

	public:
		static Vector3 EaseIn
		(
			const Vector3& startPos,
			const Vector3& endPos,
			const float time
		);

		static Vector3 EaseOut
		(
			const Vector3& startPos,
			const Vector3& endPos,
			const float time
		);

		static Vector3 EaseInOut
		(
			const Vector3& startPos,
			const Vector3& endPos,
			const float time
		);



	};*/

	//イージングの情報、Tを持たせたクラス
	template<class V>
	class Easing
	{
	private:
		V value = 0.0f;
		V start = 0.0f;
		V end = 1.0f;

		//パーセント
		float par = 0.0f;
		//関数呼び出し時の加算値
		float addPar = 0.0f;

		void CalcEasing(const V startPos, const V endPos, const float t)
		{
			value = startPos * (1.0f - t) + endPos * t;
		}

		void Clamp()
		{
			if (par >= 100.0f)par = 100.0f;
			else if (par <= 0.0f)par = 0.0f;
		}
	public:
		Easing() {}
		Easing(V start, V end) :start(start), end(end) {}
		Easing(V start, V end, const float addPar) :start(start), end(end), addPar(addPar) {}
		Easing(V start, V end, const float addPar, const float par) :start(start), end(end), par(par), addPar(addPar) {}

		V Lerp()
		{
			par += addPar;
			float max1Par = (par / 100.0f);
			CalcEasing(start, end, max1Par);
			return value;
		}

		V EaseIn()
		{
			par += addPar;
			float max1Par = (par / 100.0f);
			float t = (max1Par * max1Par);
			CalcEasing(start, end, t);
			return value;
		}

		V EaseOut()
		{
			par += addPar;
			float max1Par = (par / 100.0f);
			float t = max1Par * (2 - max1Par);
			CalcEasing(start, end, t);
			return value;
		}

		V EaseInOut()
		{
			par += addPar;
			float max1Par = (par / 100.0f);
			float t = max1Par * max1Par * (3 - 2 * max1Par);
			CalcEasing(start, end, t);
			return value;
		}

		/// <summary>
		/// 範囲の開始をセットします。
		/// </summary>
		/// <param name="start"></param>
		void SetStart(const V start) { this->start = start; }

		/// <summary>
		/// 範囲の終了をセットします。
		/// </summary>
		/// <param name="end"></param>
		void SetEnd(const V end) { this->end = end; }

		/// <summary>
		/// startからendの範囲どの辺かを%で設定します。
		/// </summary>
		/// <param name="par"></param>
		void SetPar(const float par) { this->par = par; }

		/// <summary>
		/// EaseInなどのvalueに代入する関数を呼び出した時に加算する値を設定します。
		/// </summary>
		/// <param name="par"></param>
		void SetAddPar(const float par) { addPar = par; }

		V GetValue() { return value; }
		V GetStart() { return start; }
		V GetEnd() { return end; }
		float GetPar() { return par; }
		float GetAddPar() { return addPar; }
	};
}
