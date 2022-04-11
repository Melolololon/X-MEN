#pragma once
#include<vector>
#include"Vector.h"

namespace MelLib
{
	//ベジェ、スプラインなどの曲線クラス
	class Curve
	{

	private:
		//曲線上のどこにいるか(0から1)
		float t = 0.0f;
		//点の座標
		std::vector<Vector3> points;
		//t >= 1の時にtを0にするかどうか
		bool isMaxNumberReset = false;

		/// <summary>
		/// 座標計算
		/// </summary>
		Vector3 CalcPosition();

	public:
		Curve() {}
		~Curve() {}

		/// <summary>
		/// 始点終点制御点をセットします。
		/// </summary>
		/// <param name="points">点の配列</param>
		void SetPoints(const std::vector<Vector2>& points);


		/// <summary>
		/// 始点終点制御点をセットします。
		/// </summary>
		/// <param name="points">点の配列</param>
		void SetPoints(const std::vector<Vector3>& points);

		/// <summary>
		/// t(曲線上の位置を表す値(0から1))に加算する数値。
		/// </summary>
		/// <param name="num"></param>
		void AddNumber(const float num);

		/// <summary>
		/// 曲線上の座標を取得します。
		/// </summary>
		/// <returns>Vector2型の座標</returns>
		Vector2 GetVector2Position();

		/// <summary>
		/// 曲線上の座標を取得します。
		/// </summary>
		/// <returns>Vector3型の座標</returns>
		Vector3 GetVector3Position();

		/// <summary>
		/// 終点にたどり着いたときに始点戻るかどうかをセットします。
		/// </summary>
		/// <param name="flag"></param>
		void SetIsMaxNumberReset(const bool flag) { isMaxNumberReset = flag; }
	};
}

