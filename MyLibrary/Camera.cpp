#include "Camera.h"
#include"Library.h"
#include"LibMath.h"
#include"Input.h"

using namespace MelLib;
std::unordered_map<std::string, std::unique_ptr<Camera>>Camera::pCameras;
UINT Camera::createCount = 0;
std::string Camera::mainCameraName = "";

void Camera::CalcCameraData()
{
	//ベクトルをカメラの角度に応じて回転させる関数
	auto RotateVectorCameraAngle = [&](Vector3& vector)
	{
		vector = LibMath::RotateVector3(vector, Vector3(0, 0, -1), angle.z);
		vector = LibMath::RotateVector3(vector, Vector3(-1, 0, 0), angle.x);
		vector = LibMath::RotateVector3(vector, Vector3(0, -1, 0), angle.y);
	}; 


	//このアルゴリズムで、カメラを90度回転(Xの-方向に向いてる)ときに、FPSからTPS切り替えると、
	//カメラがプレイヤーの後ろに行って、スカイリムのカメラ切り替えみたいになる
	if (rotatePoint == RotatePoint::ROTATE_POINT_CAMERA_POSITION)
	{
		targetPosition = Vector3(0, 0, cameraToTargetDistance);
		RotateVectorCameraAngle(targetPosition);
		targetPosition += rotatePointPosition;

		cameraPosition = rotatePointPosition;
	}
	else
	{
		cameraPosition = Vector3(0, 0, -cameraToTargetDistance);
		RotateVectorCameraAngle(cameraPosition);
		cameraPosition += rotatePointPosition;

		targetPosition = rotatePointPosition;
	}

	upVector = Vector3(0, 1, 0);
	RotateVectorCameraAngle(upVector);

}

void Camera::Create(const std::string& name)
{
	std::string key = name;
	if (key == "")key = "Camera_" + std::to_string(createCount);
	createCount++;

	pCameras.emplace(key, std::make_unique<Camera>());

	if (mainCameraName == "")mainCameraName = name;

}

void Camera::Delete(const std::string& name)
{
	pCameras.erase(name);
}

void Camera::SetRotateCriteriaPosition(const Vector3& position)
{
	this->rotatePointPosition = position;
	CalcCameraData();
	//仮
	Input::SetViewMatrix(GetViewMatrix());
	Input::SetProjectionMatrix(GetProjectionMatrix());

}

void Camera::SetAngle(const Vector3& angle)
{
	this->angle = angle;
	CalcCameraData();
	//仮
	Input::SetViewMatrix(GetViewMatrix());
	Input::SetProjectionMatrix(GetProjectionMatrix());
}

void Camera::SetCameraToTargetDistance(const float distance)
{
	cameraToTargetDistance = distance;
	CalcCameraData();
}

void Camera::SetRotatePoint(const RotatePoint rotatePoint)
{
	this->rotatePoint = rotatePoint;
	CalcCameraData();
}

DirectX::XMMATRIX Camera::GetViewMatrix() const
{
	DirectX::XMMATRIX matView = DirectX::XMMatrixLookAtLH
	(
		DirectX::XMLoadFloat3(&cameraPosition.ToXMFLOAT3()),
		DirectX::XMLoadFloat3(&targetPosition.ToXMFLOAT3()),
		DirectX::XMLoadFloat3(&upVector.ToXMFLOAT3())
	);
	return matView;
}

DirectX::XMMATRIX Camera::GetProjectionMatrix() const
{

	DirectX::XMMATRIX mapProjection = DirectX::XMMatrixPerspectiveFovLH
	(
		DirectX::XMConvertToRadians(fovY), //画角
		(float)Library::GetWindowWidth() / (float)Library::GetWindowHeight(),//アスペクト比
		nearNum, farNum//描画範囲
	);
	return mapProjection;
}

DirectX::XMMATRIX Camera::GetViewAndProjectionMat()const
{
	return GetViewMatrix() * GetProjectionMatrix();
}
