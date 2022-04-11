#pragma once

namespace MelLib
{

	//数値をクランプしたり補間するするクラス
	//イージングとまとめる?イージング補間だし
	class Limit
	{
	public:

		/// <summary>
		/// numがnumMin未満だったらnumMinに、numMaxを越えたらumMaxにします。
		/// </summary>
		/// <typeparam name="T">数値の型</typeparam>
		/// <param name="num">数値</param>
		/// <param name="numMin">最低値</param>
		/// <param name="numMax">最高値</param>
		/// <returns></returns>
		template<class T>
		static T Clamp(T num, const T numMin, const T numMax)
		{
			if (num > numMax)num = numMax;
			else if (num < numMin)num = numMin;
			return num;
		}

		static float Smoothstep(float num, const float numMin, const float numMax)
		{
			num = Clamp((num - numMin) / (numMax - numMin), 0.0f, 1.0f);
			return num * num * (3 - 2 * num);
		}
	};

}