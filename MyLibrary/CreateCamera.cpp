//#include "CreateCamera.h"
//
//
//
//CreateCamera::CreateCamera(int winWidth, int winHeight)
//{
//	windowWidth = winWidth;
//	windowHeight = winHeight;
//}
//
//
//CreateCamera::~CreateCamera()
//{
//}
//
//DirectX::XMMATRIX CreateCamera::Get2DCameraMatrix()
//{
//	DirectX::XMMATRIX mapProjection = DirectX::XMMatrixOrthographicOffCenterLH
//	(
//		0.0f,
//		windowWidth,
//		windowHeight,
//		0.0f,
//		0.0f,
//		1.0f
//	);
//
//	return mapProjection;
//}
//
////引数をCameraDataという構造体にする
//DirectX::XMMATRIX CreateCamera::Get3DCameraMatrix(CameraData cameraData)
//{
//	if (cameraData.fovY <= 0.0f)
//	{
//		cameraData.fovY = 0.1f;
//	}
//	if (cameraData.nearNumber <= 0.0f)
//	{
//		cameraData.nearNumber = 0.0001f;
//	}
//	if (cameraData.farNumber <= cameraData.nearNumber)
//	{
//		cameraData.farNumber = cameraData.nearNumber + 0.0001f;
//	}
//
//
//
//	return  GetViewMatrix(cameraData) * GetProjectionMatrix(cameraData);
//}
//
//void CreateCamera::Get3DCameraPosition(CameraData cameraData, DirectX::XMFLOAT3& pos, DirectX::XMFLOAT3& target)
//{
//	if (cameraData.fovY <= 0.0f)
//	{
//		cameraData.fovY = 0.1f;
//	}
//	if (cameraData.nearNumber == 0.0f)
//	{
//		cameraData.nearNumber = 0.0001f;
//	}
//	if (cameraData.farNumber == 0.0f)
//	{
//		cameraData.farNumber = 0.0002f;
//	}
//
//
//	cameraData.nowEye.x -= cameraData.eye.x;
//	cameraData.nowEye.y -= cameraData.eye.y;
//	cameraData.nowEye.z -= cameraData.eye.z;
//
//	//カメラの座標を行列で回転させてeyeに代入
//	DirectX::XMMATRIX matrix = DirectX::XMMatrixTranslation
//	(
//		cameraData.eye.x,
//		cameraData.eye.y,
//		cameraData.eye.z
//	);
//
//	matrix *=
//		DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(cameraData.eyeAngle.z)) *
//		DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(cameraData.eyeAngle.x)) *
//		DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(cameraData.eyeAngle.y)) *
//		DirectX::XMMatrixTranslation
//		(
//			cameraData.nowEye.x,
//			cameraData.nowEye.y,
//			cameraData.nowEye.z
//		);
//
//	//行列を適応させたときの座標を取り出す
//	cameraData.eye.x = matrix.r[3].m128_f32[0];
//	cameraData.eye.y = matrix.r[3].m128_f32[1];
//	cameraData.eye.z = matrix.r[3].m128_f32[2];
//
//
//
//	cameraData.nowTarget.x -= cameraData.target.x;
//	cameraData.nowTarget.y -= cameraData.target.y;
//	cameraData.nowTarget.z -= cameraData.target.z;
//
//	matrix = DirectX::XMMatrixTranslation
//	(
//		cameraData.target.x,
//		cameraData.target.y,
//		cameraData.target.z
//	);
//	matrix *=
//		DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(cameraData.targetAngle.z)) *
//		DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(cameraData.targetAngle.x))*
//		DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(cameraData.targetAngle.y)) *
//		DirectX::XMMatrixTranslation
//		(
//			cameraData.nowTarget.x,
//			cameraData.nowTarget.y,
//			cameraData.nowTarget.z
//		);
//	cameraData.target.x = matrix.r[3].m128_f32[0];
//	cameraData.target.y = matrix.r[3].m128_f32[1];
//	cameraData.target.z = matrix.r[3].m128_f32[2];
//
//	//結果を代入
//	pos = cameraData.eye;
//	target = cameraData.target;
//}
//
//
//DirectX::XMMATRIX CreateCamera::GetViewMatrix(CameraData cameraData)
//{
//	DirectX::XMMATRIX matView;
//
//
//	cameraData.nowEye.x -= cameraData.eye.x;
//	cameraData.nowEye.y -= cameraData.eye.y;
//	cameraData.nowEye.z -= cameraData.eye.z;
//
//	//カメラの座標を行列で回転させてeyeに代入
//	DirectX::XMMATRIX matrix = DirectX::XMMatrixTranslation
//	(
//		cameraData.eye.x,
//		cameraData.eye.y,
//		cameraData.eye.z
//	);
//
//	matrix *=
//		DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(cameraData.eyeAngle.z)) *
//		DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(cameraData.eyeAngle.x)) *
//		DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(cameraData.eyeAngle.y)) *
//		DirectX::XMMatrixTranslation
//		(
//			cameraData.nowEye.x,
//			cameraData.nowEye.y,
//			cameraData.nowEye.z
//		);
//
//	//行列を適応させたときの座標を取り出す
//	cameraData.eye.x = matrix.r[3].m128_f32[0];
//	cameraData.eye.y = matrix.r[3].m128_f32[1];
//	cameraData.eye.z = matrix.r[3].m128_f32[2];
//
//
//
//	cameraData.nowTarget.x -= cameraData.target.x;
//	cameraData.nowTarget.y -= cameraData.target.y;
//	cameraData.nowTarget.z -= cameraData.target.z;
//
//	matrix = DirectX::XMMatrixTranslation
//	(
//		cameraData.target.x,
//		cameraData.target.y,
//		cameraData.target.z
//	);
//	matrix *=
//		DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(cameraData.targetAngle.z)) *
//		DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(cameraData.targetAngle.x))*
//		DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(cameraData.targetAngle.y)) *
//		DirectX::XMMatrixTranslation
//		(
//			cameraData.nowTarget.x,
//			cameraData.nowTarget.y,
//			cameraData.nowTarget.z
//		);
//	cameraData.target.x = matrix.r[3].m128_f32[0];
//	cameraData.target.y = matrix.r[3].m128_f32[1];
//	cameraData.target.z = matrix.r[3].m128_f32[2];
//
//
//	matView = DirectX::XMMatrixLookAtLH
//	(
//		DirectX::XMLoadFloat3(&cameraData.eye),
//		DirectX::XMLoadFloat3(&cameraData.target),
//		DirectX::XMLoadFloat3(&cameraData.up)
//	);
//
//	return matView;
//}
//
//DirectX::XMMATRIX CreateCamera::GetProjectionMatrix(CameraData cameraData)
//{
//	DirectX::XMMATRIX mapProjection = DirectX::XMMatrixPerspectiveFovLH
//	(
//		DirectX::XMConvertToRadians(cameraData.fovY), //画角
//		(float)windowWidth / (float)windowHeight,//アスペクト比
//		cameraData.nearNumber, cameraData.farNumber//描画範囲
//	);
//
//	return mapProjection;
//}
