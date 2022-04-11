//#pragma once
//#include<DirectXMath.h>
//#include"DirectXStruct.h"
//
//
//namespace melLib
//{
//class CreateCamera
//{
//private:
//	int windowWidth;
//	int windowHeight;
//
//
//public:
//	CreateCamera(int winWidth,int winHeight);
//	~CreateCamera();
//	
//	DirectX::XMMATRIX Get2DCameraMatrix();
//	DirectX::XMMATRIX Get3DCameraMatrix(CameraData cameraData);
//
//	/// <summary>
//	/// 回転行列を適応させたカメラ座標を返します
//	/// </summary>
//	/// <param name="cameraData"></param>
//	/// <returns></returns>
//	void Get3DCameraPosition(CameraData cameraData,DirectX::XMFLOAT3& pos, DirectX::XMFLOAT3& target);
//
//	DirectX::XMMATRIX GetViewMatrix(CameraData cameraData);
//	DirectX::XMMATRIX GetProjectionMatrix(CameraData cameraData);
//};
//
