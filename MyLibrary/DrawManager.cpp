#include "DrawManager.h"

ID3D12Device* MelLib::DrawManager::device;
std::vector<ID3D12GraphicsCommandList*>MelLib::DrawManager::cmdLists;
ComPtr<ID3D12RootSignature>MelLib::DrawManager::rootSignature;

MelLib::DrawManager* MelLib::DrawManager::GetInstance()
{
	static DrawManager d;
	return &d;
}

void MelLib::DrawManager::SetDraw(Sprite* sprite)
{
	//pSpriteDatas.emplace();
}

void MelLib::DrawManager::SetDraw(ModelObject* model)
{
	preDrawType = DrawType::MODEL;

	pModelObjects.push_back(model);
}

void MelLib::DrawManager::Draw()
{
	//if (currentDrawType != preDrawType) 
	//{
	//	switch (currentDrawType)
	//	{
	//	case MelLib::DrawManager::DrawType::MODEL:
	//		cmdLists[0]->SetGraphicsRootSignature(rootSignature.Get());
	//		cmdLists[0]->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//		break;
	//	case MelLib::DrawManager::DrawType::SPRITE:
	//		break;
	//	default:
	//		break;
	//	}
	//}

	//if (currentModelData != preModelData) 
	//{	
	//	//モデル特有バッファセット
	//	if (modelConstBufferData.bufferType == ConstBufferData::BufferType::BUFFER_TYPE_EACH_MODEL)
	//		cmdLists[0]->SetGraphicsRootConstantBufferView(MODEL_BUFFER_REGISTER, modelConstBuffer[0]->GetGPUVirtualAddress());

	//	//ユーザーモデルバッファセット
	//	if (userConstBufferData.bufferType == ConstBufferData::BufferType::BUFFER_TYPE_EACH_MODEL)
	//		cmdLists[0]->SetGraphicsRootConstantBufferView(MODEL_BUFFER_REGISTER, userConstBuffer[0]->GetGPUVirtualAddress());

	//}

}
