#pragma once
#include<array>
#include"Vector.h"
#include<DirectXMath.h>

namespace MelLib
{

	struct Matrix
	{
		Matrix();
		Matrix(const DirectX::XMMATRIX& m);

		std::array<std::array<float, 4>, 4> matrix =
		{
			{
				{0,0,0,0},
				{0,0,0,0},
				{0,0,0,0},
				{0,0,0,0}
			}
		};
		
		

		Matrix operator*(const Matrix& mat)const;
		void operator*=(const Matrix& mat);

		std::array<float, 4>& operator[](const unsigned int num);
		std::array<float, 4>operator[](const unsigned int num)const;

		/// <summary>
		/// XMMATRIXに変換します。
		/// </summary>
		/// <returns></returns>
		DirectX::XMMATRIX ToXMMATRIX();

		/// <summary>
		/// 零行列を取得します。
		/// </summary>
		/// <returns></returns>
		static Matrix GetZeroMatrix();

		/// <summary>
		/// 単位行列を取得します。
		/// </summary>
		/// <returns></returns>
		static Matrix GetIdentityMatrix();

		/// <summary>
		/// X軸基準で回転させる行列を取得します。
		/// </summary>
		/// <param name="angle">角度</param>
		/// <returns></returns>
		static Matrix GetRotateXMatrix(const float angle);

		/// <summary>
		/// Y軸基準で回転させる行列を取得します。
		/// </summary>
		/// <param name="angle">角度</param>
		/// <returns></returns>
		static Matrix GetRotateYMatrix(const float angle);

		/// <summary>
		/// Z軸基準で回転させる行列を取得します。
		/// </summary>
		/// <param name="angle">角度</param>
		/// <returns></returns>
		static Matrix GetRotateZMatrix(const float angle);

		/// <summary>
		/// 回転行列をZXYの順番で掛けて取得します。
		/// </summary>
		/// <param name="angle">角度</param>
		/// <returns>計算結果</returns>
		static Matrix GetRotateZXYMatrix(const Vector3& angle);

		/// <summary>
		/// 拡縮する行列を取得します。
		/// </summary>
		/// <param name="vector">拡縮度</param>
		/// <returns></returns>
		static Matrix GetScalingMatrix(const Vector3& vector);

		/// <summary>
		/// 平行移動する行列を取得します。
		/// </summary>
		/// <param name="vector">移動量</param>
		/// <returns></returns>
		static Matrix GetTranslationMatrix(const Vector3& vector);
	};
}



