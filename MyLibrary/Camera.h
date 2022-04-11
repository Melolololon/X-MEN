#pragma once
#include"Vector.h"
#include<vector>
#include<string>
#include<memory>
#include<unordered_map>

//#include"Input.h"

namespace MelLib
{
	class Camera
	{
	public:

		//回転させるときの基準となる位置
		enum class RotatePoint
		{
			ROTATE_POINT_CAMERA_POSITION,//カメラの座標を中心に注視点を回転
			ROTATE_POINT_TARGET_POSITION,//注視点の座標を中心にカメラ座標を回転
		};

	private:
		using UINT = unsigned int;

		static std::unordered_map<std::string, std::unique_ptr<Camera>>pCameras;
		static UINT createCount;
		static std::string mainCameraName;

		//回転基準
		RotatePoint rotatePoint = RotatePoint::ROTATE_POINT_CAMERA_POSITION;


		//画角
		float fovY = 60.0f;

		//最近点
		float nearNum = 0.01f;
		//最遠点
		float farNum = 1000.0f;

		//カメラ座標から注視点の距離
		float cameraToTargetDistance = 1.0f;

		//回転させるときの基準位置
		Vector3 rotatePointPosition = Vector3(0, 0, -10);
		Vector3 angle = 0;

		Vector3 cameraPosition = rotatePointPosition;
		Vector3 targetPosition = rotatePointPosition + Vector3(0, 0, cameraToTargetDistance);
		Vector3 upVector = Vector3(0, 1, 0);

		void CalcCameraData();
	public:
		Camera() {}
		~Camera() {}

		/// <summary>
		/// カメラを生成します。
		/// </summary>
		/// <param name="name"></param>
		static void Create(const std::string& name = "");

		/// <summary>
		/// カメラを削除します。
		/// </summary>
		/// <param name="name"></param>
		static void Delete(const std::string& name);

		/// <summary>
	/// カメラのポインタを取得します。
	/// </summary>
	/// <param name="name">カメラの名前(見入)</param>
	/// <returns></returns>
		static Camera* Get(const std::string& name = mainCameraName) { return pCameras[name].get(); }


#pragma region セット

		/// <summary>
		/// カメラを回転させるときに基準となる座標をセットします。
		/// </summary>
		/// <param name="position">回転させるときに基準となる座標</param>
		void SetRotateCriteriaPosition(const Vector3& position);


		/// <summary>
		/// 角度をセットします。角度が(0,0,0)の場合、カメラは0,0,1の方向を向きます。
		/// </summary>
		/// <param name="angle">カメラの角度</param>
		void SetAngle(const Vector3& angle);

		/// <summary>
		/// 画角をセットします。
		/// </summary>
		/// <param name="fovY">画角</param>
		void SetFovY(const float fovY) { this->fovY = fovY; }

		/// <summary>
		/// カメラからカメラの表示範囲の一番近い場所までの距離をセットします。
		/// </summary>
		/// <param name="num">near値</param>
		void SetNear(const float num)
		{
			nearNum = num;
			//Input::SetNear(nearNum);
		}

		/// <summary>
		/// カメラからカメラの表示範囲の一番遠い場所までの距離をセットします。
		/// </summary>
		/// <param name="num">far値</param>
		void SetFar(const float num)
		{
			farNum = num;
			//Input::SetFar(farNum);
		}

		/// <summary>
		/// カメラと注視点の距離をセットします。初期値は1.0fです。
		/// </summary>
		/// <param name="distance">カメラと注視点の距離</param>
		void SetCameraToTargetDistance(const float distance);

		/// <summary>
		/// カメラを回転させるとき、どこを基準に回転させるかを決めます。
		/// </summary>
		/// <param name="rotatePoint"></param>
		void SetRotatePoint(const RotatePoint rotatePoint);

#pragma endregion

#pragma region ゲット


		/// <summary>
		/// メインカメラ(ライブラリで用意されたカメラ)の名前を取得します。
		/// </summary>
		/// <returns></returns>
		static const std::string& GetMainCameraName() { return mainCameraName; }

		DirectX::XMMATRIX GetViewMatrix()const;
		DirectX::XMMATRIX GetProjectionMatrix()const;

		/// <summary>
		/// ビュー行列とプロジェクション行列を掛けた行列を取得します。
		/// </summary>
		/// <returns></returns>
		DirectX::XMMATRIX GetViewAndProjectionMat()const;

		/// <summary>
		/// カメラの座標を取得します。
		/// </summary>
		/// <returns>カメラの座標</returns>
		Vector3 GetCameraPosition()const { return cameraPosition; }

		/// <summary>
		/// 注視点の座標を取得します。
		/// </summary>
		/// <returns>注視点の座標</returns>
		Vector3 GetTargetPosition()const { return targetPosition; }

		/// <summary>
		/// 上ベクトルを取得します。
		/// </summary>
		/// <returns>上ベクトル</returns>
		Vector3 GetUpVector()const { return upVector; }

		Vector3 GetAngle()const { return angle; }

#pragma endregion
	};
}
