#include"ModelObject.h"
#include"CreateBuffer.h"
#include"FbxLoader.h"
#include"DirectionalLight.h"
#include"Matrix.h"
#include"CollisionType.h"
#include"LibMath.h"
#include"Values.h"
#include"Collision.h"

using namespace MelLib;

std::unordered_map<std::string, std::unique_ptr<ModelObject>>ModelObject::pModelObjects;


ID3D12Device* ModelObject::device;
std::vector<ID3D12GraphicsCommandList*>ModelObject::cmdLists;
ComPtr<ID3D12RootSignature> ModelObject::rootSignature;

ModelObject::ModelObject(ModelData* pModelData, ConstBufferData* userConstBufferData)
{
	Create(pModelData, userConstBufferData);
}

void ModelObject::CreateConstBuffer()
{
	ConstBufferData::BufferType modelConstBufferType = modelConstBufferData.bufferType;
	ConstBufferData::BufferType userConstBufferType = userConstBufferData.bufferType;

#pragma region リサイズ

	constBuffer.resize(modelFileObjectNum);


	if (modelConstBufferType == ConstBufferData::BufferType::BUFFER_TYPE_EACH_MODEL)
	{
		modelConstBuffer.resize(1);
	}
	else if (modelConstBufferType == ConstBufferData::BufferType::BUFFER_TYPE_EACH_MODEL_OBJECT)
	{
		modelConstBuffer.resize(modelFileObjectNum);
	}


	if (userConstBufferType == ConstBufferData::BufferType::BUFFER_TYPE_EACH_MODEL)
	{
		userConstBuffer.resize(1);
	}
	else if (userConstBufferType == ConstBufferData::BufferType::BUFFER_TYPE_EACH_MODEL_OBJECT)
	{
		userConstBuffer.resize(modelFileObjectNum);
	}

#pragma endregion


	auto CreateBuffer = [&]
	(
		ID3D12Resource** pBuffer,
		const size_t& dataSize
		)
	{
		CreateBuffer::GetInstance()->CreateConstBuffer
		(
			CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
			dataSize,
			pBuffer
		);
	};


	//モデル
	if (modelConstBufferType == ConstBufferData::BufferType::BUFFER_TYPE_EACH_MODEL)
	{
		CreateBuffer
		(
			&modelConstBuffer[0],
			modelConstBufferData.bufferDataSize
		);
	}

	//ユーザー
	if (userConstBufferType == ConstBufferData::BufferType::BUFFER_TYPE_EACH_MODEL)
	{
		CreateBuffer
		(
			&userConstBuffer[0],
			userConstBufferData.bufferDataSize
		);
	}

	//モデル内のオブジェクト分ループ
	for (int i = 0; i < modelFileObjectNum; i++)
	{
#pragma region 生成


		//メイン
		CreateBuffer
		(
			&constBuffer[i],
			sizeof(ModelConstBufferData)
		);


		//モデル
		if (modelConstBufferType == ConstBufferData::BufferType::BUFFER_TYPE_EACH_MODEL_OBJECT)
		{
			CreateBuffer
			(
				&modelConstBuffer[i],
				modelConstBufferData.bufferDataSize
			);
		}

		//ユーザー
		if (userConstBufferType == ConstBufferData::BufferType::BUFFER_TYPE_EACH_MODEL_OBJECT)
		{
			CreateBuffer
			(
				&userConstBuffer[i],
				userConstBufferData.bufferDataSize
			);
		}
#pragma endregion

#pragma region 初期化

		ModelConstBufferData* constBufferData;
		constBuffer[i]->Map(0, nullptr, (void**)&constBufferData);

		//ライト
		for (int j = 0, size = DirectionalLight::LIGTH_MAX; j < size; j++)
		{
			constBufferData->light[j] = DirectX::XMFLOAT4(0, 0, 0, 0);
			constBufferData->lightColor[j] = DirectX::XMFLOAT4(0, 0, 0, 0);
		}

		constBuffer[i]->Unmap(0, nullptr);
#pragma endregion
	}


}

void ModelObject::DrawCommonProcessing(const std::string& rtName)
{
	if (!pModelData)return;
	MapConstData(RenderTarget::Get(rtName)->GetCamera());

	ModelConstBufferData* constBufferData;
	constBuffer[0]->Map(0, nullptr, (void**)&constBufferData);
	int z = 0;

	SetCmdList();
}

void ModelObject::MapConstData(const Camera* camera)
{
	


	std::vector<DirectX::XMMATRIX>meshGlobalTransforms = pModelData->GetMeshGlobalTransforms();

	ModelConstBufferData* constBufferData;
	for (int i = 0; i < objectNames.size(); i++)
	{
		std::string objectName = objectNames[i];
#pragma region 基本的な情報のマップ

		constBuffer[i]->Map(0, nullptr, (void**)&constBufferData);
		constBufferData->addColor = modelConstDatas[objectName].addColor;
		constBufferData->subColor = modelConstDatas[objectName].subColor;
		constBufferData->mulColor = modelConstDatas[objectName].mulColor;
		constBufferData->ex = modelConstDatas[objectName].pushPolygonNum;

		std::vector<DirectionalLight*> pLights = DirectionalLight::GetAll();
		for (int i = 0, size = pLights.size(); i < size; i++)
		{
			Vector3 lightDir = pLights[i]->GetDirection();
			constBufferData->light[i] = DirectX::XMFLOAT4(lightDir.x, lightDir.y, lightDir.z, 0);
			Color lightCor = pLights[i]->GetColor();
			constBufferData->lightColor[i] = DirectX::XMFLOAT4
			(
				(float)lightCor.r / 255.0f,
				(float)lightCor.g / 255.0f,
				(float)lightCor.b / 255.0f,
				(float)lightCor.a / 255.0f
			);


		}

		Vector3 cameraPos = camera->GetCameraPosition();
		constBufferData->cameraPos = DirectX::XMFLOAT4(cameraPos.x, cameraPos.y, cameraPos.z, 0);

#pragma region 行列計算
		DirectX::XMMATRIX matWorld = DirectX::XMMatrixIdentity();

		// FBXモデルの場合、メッシュの行列を掛ける
		if (pModelData->GetModelFormat() == ModelData::ModelFormat::MODEL_FORMAT_FBX)
		{
			matWorld *= meshGlobalTransforms[i];
		}

		matWorld *= DirectX::XMMatrixScaling
		(
			modelConstDatas[objectName].scale.x,
			modelConstDatas[objectName].scale.y,
			modelConstDatas[objectName].scale.z
		);
		matWorld *= DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(modelConstDatas[objectName].angle.z));
		matWorld *= DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(modelConstDatas[objectName].angle.x));
		matWorld *= DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(modelConstDatas[objectName].angle.y));

		constBufferData->normalMat = matWorld;

		matWorld *= DirectX::XMMatrixTranslation
		(
			modelConstDatas[objectName].position.x,
			modelConstDatas[objectName].position.y,
			modelConstDatas[objectName].position.z
		);

		constBufferData->worldMat = matWorld;
		constBufferData->mat = camera->GetViewAndProjectionMat();

#pragma endregion


#pragma region レンダリング方向
		//上下左右前後
		std::array<float, 6>dirPos = pModelData->GetDirectionMaxPosition()[i];

		//仮に前から奧
		constBufferData->tex3DStart = dirPos[4];
		constBufferData->tex3DEnd = dirPos[5];
#pragma endregion


		constBuffer[i]->Unmap(0, nullptr);
	
#pragma endregion


	}

	constBuffer[0]->Map(0, nullptr, (void**)&constBufferData);
	constBuffer[0]->Unmap(0, nullptr);

	for (int i = 0; i < modelConstBuffer.size(); i++)
	{
		constBuffer[0]->Map(0, nullptr, (void**)&constBufferData);
		constBuffer[0]->Unmap(0, nullptr);

		std::string objectName = objectNames[i];

#pragma region ボーンのマップ

		int boneNum = pModelData->GetBoneNumber();

		if (boneNum == 0
			/*|| pModelData->GetModelFormat() != ModelData::ModelFormat::MODEL_FORMAT_OBJ*/
			/*&& i != 0*/)
		{
			continue;
		}


		// モデルのオブジェクトごとに生成するようにする
		SkinConstBufferData* skinConstData;

		modelConstBuffer[i]->Map(0, nullptr, (void**)&skinConstData);

		if (pModelData->GetModelFormat() == ModelData::ModelFormat::MODEL_FORMAT_OBJ)
		{
			std::vector<Vector3>objBonePositions = pModelData->GetObjBonePosition();

			DirectX::XMMATRIX boneMat = DirectX::XMMatrixIdentity();
			DirectX::XMFLOAT3 boneScale;
			DirectX::XMFLOAT3 boneAngle;
			DirectX::XMFLOAT3 boneMoveVector;

			//親ボーンの行列乗算
			int parentBoneNum = 0;
			int bone = 0;

			DirectX::XMFLOAT3 bonePos = { 0,0,0 };


			for (UINT j = 0; j < boneNum; j++)
			{
				boneMat = DirectX::XMMatrixIdentity();

				//ボーンから頂点の距離分移動
				bonePos = objBonePositions[j].ToXMFLOAT3();
				boneMat *= DirectX::XMMatrixTranslation(-bonePos.x, -bonePos.y, -bonePos.z);

				boneScale = boneDatas[j].scale;
				boneMat *= DirectX::XMMatrixScaling(boneScale.x, boneScale.y, boneScale.z);

				boneAngle = boneDatas[j].angle;
				boneMat *= DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(boneAngle.z));
				boneMat *= DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(boneAngle.x));
				boneMat *= DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(boneAngle.y));

				boneMoveVector = boneDatas[j].moveVector;

				//モデル自体のスケールの乗算により、ボーンの平行移動の値にスケールが乗算されるため、
				//割って増減を抑えている
				//ボーン→行列の順番で掛けるからモデルの倍率が影響しちゃう
				boneMat *= DirectX::XMMatrixTranslation
				(
					boneMoveVector.x / modelConstDatas[objectName].scale.x,
					boneMoveVector.y / modelConstDatas[objectName].scale.y,
					boneMoveVector.z / modelConstDatas[objectName].scale.z
				);

				//回転させたら戻す
				boneMat *= DirectX::XMMatrixTranslation(bonePos.x, bonePos.y, bonePos.z);

#pragma region 親の処理


				DirectX::XMFLOAT3 boneAngleImpact = { 0.0f,0.0f,0.0f };
				DirectX::XMFLOAT3 boneScaleImpact = { 1.0f,1.0f,1.0f };
				DirectX::XMFLOAT3 boneMoveVectorImpact = { 0.0f,0.0f,0.0f };
				boneAngleImpact = parentBoneDatas[j].angleImpact;
				boneScaleImpact = parentBoneDatas[j].scaleImpact;
				boneMoveVectorImpact = parentBoneDatas[j].moveVectorImpact;

				//親のボーン番号代入
				parentBoneNum = parentBoneDatas[j].parentBoneNum;

				//これを使って影響度とか加算してく
				//子　→　親の順で入れる
				std::vector<int>parentNums;
				parentNums.reserve(50);

				//こちらは親　→　子
				std::vector<DirectX::XMMATRIX>parentMat;
				parentMat.reserve(50);


				//先にすべての親を取得&影響度取得
				while (1)
				{
					//-1だったら(親がセットされてなかったら)抜ける
					if (parentBoneNum == -1)break;
					parentNums.push_back(parentBoneNum);

					//親のボーン番号代入
					parentBoneNum = parentBoneDatas[parentBoneNum].parentBoneNum;

				}
				if (parentNums.size() != 0)
				{

					DirectX::XMMATRIX mulMat = DirectX::XMMatrixIdentity();

					//親数
					const int maxParentSize = static_cast<int>(parentNums.size());

					DirectX::XMFLOAT3 pAngle = { 0,0,0 };
					DirectX::XMFLOAT3 pScale = { 1,1,1 };
					DirectX::XMFLOAT3 pMoveVector = { 0,0,0 };
					DirectX::XMFLOAT3 pPos = { 0,0,0 };

					DirectX::XMFLOAT3 pAngleImpact = { 1,1,1 };
					DirectX::XMFLOAT3 pScaleImpact = { 1,1,1 };
					DirectX::XMFLOAT3 pMoveVectorImpact = { 1,1,1 };

					mulMat = DirectX::XMMatrixIdentity();

					//最後にある親のボーンを基準に回すので、入れる
					pPos.x = objBonePositions[parentNums[maxParentSize - 1]].x;
					pPos.y = objBonePositions[parentNums[maxParentSize - 1]].y;
					pPos.z = objBonePositions[parentNums[maxParentSize - 1]].z;

					int impactIndex = j;
					for (auto& pNum : parentNums)
					{
						pAngle.x += boneDatas[pNum].angle.x;
						pAngle.y += boneDatas[pNum].angle.y;
						pAngle.z += boneDatas[pNum].angle.z;

						pScale.x *= boneDatas[pNum].scale.x;
						pScale.y *= boneDatas[pNum].scale.y;
						pScale.z *= boneDatas[pNum].scale.z;

						pMoveVector.x += boneDatas[pNum].moveVector.x;
						pMoveVector.y += boneDatas[pNum].moveVector.y;
						pMoveVector.z += boneDatas[pNum].moveVector.z;



						pAngleImpact.x *= parentBoneDatas[impactIndex].angleImpact.x;
						pAngleImpact.y *= parentBoneDatas[impactIndex].angleImpact.y;
						pAngleImpact.z *= parentBoneDatas[impactIndex].angleImpact.z;
						pScaleImpact.x *= parentBoneDatas[impactIndex].scaleImpact.x;
						pScaleImpact.y *= parentBoneDatas[impactIndex].scaleImpact.y;
						pScaleImpact.z *= parentBoneDatas[impactIndex].scaleImpact.z;
						pMoveVectorImpact.x *= parentBoneDatas[impactIndex].moveVectorImpact.x;
						pMoveVectorImpact.y *= parentBoneDatas[impactIndex].moveVectorImpact.y;
						pMoveVectorImpact.z *= parentBoneDatas[impactIndex].moveVectorImpact.z;

						impactIndex = pNum;
					}

					pAngle.x *= pAngleImpact.x;
					pAngle.y *= pAngleImpact.y;
					pAngle.z *= pAngleImpact.z;

					pScale.x *= pScaleImpact.x;
					pScale.y *= pScaleImpact.y;
					pScale.z *= pScaleImpact.z;

					pMoveVector.x *= pMoveVectorImpact.x;
					pMoveVector.y *= pMoveVectorImpact.y;
					pMoveVector.z *= pMoveVectorImpact.z;


					//ボーンから頂点の距離分移動
					mulMat *= DirectX::XMMatrixTranslation(-pPos.x, -pPos.y, -pPos.z);

					mulMat *= DirectX::XMMatrixScaling(pScale.x, pScale.y, pScale.z);

					mulMat *= DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(pAngle.z));
					mulMat *= DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(pAngle.x));
					mulMat *= DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(pAngle.y));


					//親はまとめて掛けているので、モデルと自分の拡縮だけ掛ければいい
					//modelConstDatas[i][0].scale に boneScale掛ける必要あるか要確認
					mulMat *= DirectX::XMMatrixTranslation
					(
						pMoveVector.x / (modelConstDatas[objectName].scale.x * boneScale.x),
						pMoveVector.y / (modelConstDatas[objectName].scale.y * boneScale.y),
						pMoveVector.z / (modelConstDatas[objectName].scale.z * boneScale.z)
					);

					//回転させたら戻す
					mulMat *= DirectX::XMMatrixTranslation(pPos.x, pPos.y, pPos.z);


					boneMat *= mulMat;
				}
#pragma endregion

				skinConstData->bones[j + 1] = boneMat;

			}


		}
		else if (pModelData->GetModelFormat() == ModelData::ModelFormat::MODEL_FORMAT_FBX)
		{
			pModelData->SetFbxAnimStack(fbxAnimationData.currentAnimationName);

			std::vector<ModelData::FbxBone> bones = pModelData->GetFbxBone();

			for (int j = 0; j < boneNum; j++)
			{
				if (boneNum >= BONE_MAX)
				{
					std::string outputStr = pModelData->GetModelPath() + "のボーン数がボーン最大数" + std::to_string(BONE_MAX) + "を超えています。\n";
					OutputDebugStringA(outputStr.c_str());

					outputStr = "モデルのボーン数 : " + std::to_string(boneNum) + "\n";
					OutputDebugStringA(outputStr.c_str());
					break;
				}

				//bones[j].fbxCluster.

				//変換
				DirectX::XMMATRIX matCurrentPose;
				FbxAMatrix fbxCurrentPose =
					bones[j].fbxCluster->GetLink()->EvaluateGlobalTransform(fbxAnimationData.currentTime);
				FbxLoader::GetInstance()->ConvertMatrixFromFbx(&matCurrentPose, fbxCurrentPose);

				//乗算
				//skinConstData->bones[j] = bones[j].invInitialPose * matCurrentPose;

				//DirectX::XMMATRIX meshGlobalTransform = pModelData->GetMeshGlobalTransform(i);
				DirectX::XMMATRIX meshGlobalTransform = meshGlobalTransforms[i];
				DirectX::XMMATRIX invMeshGlobalTransform = DirectX::XMMatrixInverse(nullptr, meshGlobalTransform);

				skinConstData->bones[j] =
					meshGlobalTransform *
					bones[j].invInitialPose *
					matCurrentPose *
					invMeshGlobalTransform;


			}
		}

		modelConstBuffer[i]->Unmap(0, nullptr);
#pragma endregion

	}
}

void ModelObject::SetCmdList()
{

	cmdLists[0]->SetGraphicsRootSignature(rootSignature.Get());
	//cmdLists[0]->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST);
	cmdLists[0]->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);



	//モデル特有バッファセット
	if (modelConstBufferData.bufferType == ConstBufferData::BufferType::BUFFER_TYPE_EACH_MODEL)
		cmdLists[0]->SetGraphicsRootConstantBufferView(MODEL_BUFFER_REGISTER, modelConstBuffer[0]->GetGPUVirtualAddress());

	//ユーザーモデルバッファセット
	if (userConstBufferData.bufferType == ConstBufferData::BufferType::BUFFER_TYPE_EACH_MODEL)
		cmdLists[0]->SetGraphicsRootConstantBufferView(MODEL_BUFFER_REGISTER, userConstBuffer[0]->GetGPUVirtualAddress());



	const std::vector<VertexBufferSet>& vertexBufferSets = pModelData->GetVertexBufferSet();
	const std::vector<IndexBufferSet>& indexBufferSets = pModelData->GetIndexBufferSet();


	//モデルのオブジェクトごとをセット
	//頂点バッファ分ループ
	for (int i = 0; i < modelFileObjectNum; i++)
	{
		/*if(modelFileObjectNum >= 2)
		{
			i = modelFileObjectNum - 1;
		}*/

		std::string objectName = objectNames[i];

		//cmdLists[0]->SetPipelineState(pPipeline[i]->GetPipelineState().Get());
		cmdLists[0]->SetPipelineState(materials[objectName]->GetPPipelineState()->GetPipelineState().Get());


		cmdLists[0]->IASetVertexBuffers(0, 1, &vertexBufferSets[i].vertexBufferView);
		cmdLists[0]->IASetIndexBuffer(&indexBufferSets[i].indexBufferView);

#pragma region テクスチャ

		ID3D12DescriptorHeap* textureDescHeap = materials[objectName]->GetPTextureHeap();
		std::vector<ID3D12DescriptorHeap*> ppHeaps;
		ppHeaps.push_back(textureDescHeap);
		cmdLists[0]->SetDescriptorHeaps(1, &ppHeaps[0]);


		D3D12_GPU_DESCRIPTOR_HANDLE gpuDescHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE
		(
			materials[objectName]->GetPTextureHeap()->GetGPUDescriptorHandleForHeapStart(),
			0,
			device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
		);
		cmdLists[0]->SetGraphicsRootDescriptorTable(TEXURE_ROOTPARAM_NUM, gpuDescHandle);
#pragma endregion

#pragma region 定数


		//if(modelFileObjectNum >= 2)//定数バッファ
		//	cmdLists[0]->SetGraphicsRootConstantBufferView(CONST_BUFFER_REGISTER, constBuffer[1]->GetGPUVirtualAddress());
		//else
		//定数バッファ
		cmdLists[0]->SetGraphicsRootConstantBufferView(CONST_BUFFER_REGISTER, constBuffer[i]->GetGPUVirtualAddress());

		/*ModelConstBufferData* constBufferData;
		constBuffer[0]->Map(0, nullptr, (void**)&constBufferData);*/

		//マテリアルバッファ
		cmdLists[0]->SetGraphicsRootConstantBufferView
		(MATERIAL_BUFFER_REGISTER, materials[objectName]->GetPConstBuffer(Material::MaterialConstBufferType::MATERIAL_DATA)->GetGPUVirtualAddress());

		//Colorマテリアル
		cmdLists[0]->SetGraphicsRootConstantBufferView
		(COLOR_MATERIAL_BUFFER_REGISTER, materials[objectName]->GetPConstBuffer(Material::MaterialConstBufferType::COLOR)->GetGPUVirtualAddress());

		//モデルバッファ
		if (modelConstBufferData.bufferType == ConstBufferData::BufferType::BUFFER_TYPE_EACH_MODEL_OBJECT)
			cmdLists[0]->SetGraphicsRootConstantBufferView(MODEL_BUFFER_REGISTER, modelConstBuffer[i]->GetGPUVirtualAddress());

		//ユーザー定数バッファ
		if (userConstBufferData.bufferType == ConstBufferData::BufferType::BUFFER_TYPE_EACH_MODEL_OBJECT)
			cmdLists[0]->SetGraphicsRootConstantBufferView(MODEL_BUFFER_REGISTER, userConstBuffer[i]->GetGPUVirtualAddress());


#pragma endregion


		//描画
		cmdLists[0]->DrawIndexedInstanced(static_cast<UINT>(pModelData->GetIndices()[i].size()), 1, 0, 0, 0);

	}

}

void MelLib::ModelObject::Update()
{
	FbxAnimation();
}

void ModelObject::Draw(const std::string& rtName)
{
	RenderTarget::ChangeCurrentRenderTarget(RenderTarget::Get(rtName));

	//FbxAnimation();

	DrawCommonProcessing(rtName);
}

bool MelLib::ModelObject::MeshCat(const PlaneData& plane, ModelData*& pFront, ModelData*& pBack, const bool createCrossSection)
{
	// 平面を回転させるとき、ZXYじゃなくてYXZでやったほうがいい?



	//緊急(解決優先度高めの現在の問題点)
	//1.そもそも四角形でも斬り方によっては5角形になるので、どっちみち面の再形成は必要
	//やり方は、斬られた辺の自分側(反対側じゃない方)の頂点と衝突点で多角形の三角形化を行う
	//斬られた辺に関わる部分だけ行えばよい

	//やること
	//1.当たった辺だけじゃなく、全頂点に対して表裏判定を行う。
	//(反対側の頂点をすべて消さないといけないため)

	//2.断面に使用する衝突点と入れ替える頂点かどうかの判断。
	//使うか使わないかだけわかればいい。
	//やり方は、ポートフォリオに書いてある
	//判断できたら、配列の何番目かとかの数値を取得しといて、断面の形成時に取得してインデックスとして使えるようにしとく
	//正直入れ替えずに消し飛ばして、後から追加してもいい

	//3.消し飛ばしたら、頂点だけではなくインデックスも変更する。
	//反対側の頂点が関係してるインデックスのセット(三角形を形成する添え字3つ)を消し飛ばす。
	//平面をまたいでる部分は消しちゃダメ。

	//現在の問題点
	//1.断面を形成するのに使う頂点とそうじゃない頂点があるが、断面を形成するのに使う頂点をどう取り出すか。
	//法線を元の頂点のやつを使いまわそうと思ったが、断面のように面を形成してない可能性があるので無理だった。
	//(四角形は断面と同じインデックスの面が元からあるからできる)
	//そもそも断面のように面を形成してない場合は、その頂点を追加しないといけない
	//そもそも切断してなくなる頂点を入れ替えずに消す?
	//結局断面を形成するのに使うか使わないかを判断しないといけない。
	//結局三角形を斬るときと同じように元からある面を形成しなおさないといけない?

	//2,斬った時に3つ以上分かれる場合はどうするか(wの真ん中斬った時など5個に分かれる)


	//思いついたこと
	//1.3角形も斬った時に面が消えるから、入れ替えればできる?
	//結局四角形に変えないといけないから無理。
	//多角形を三角形の集合体にするやつで面を形成しなおせばできる?

	//この処理はBoxとBoardで行う

	//とりあえず当たり判定処理をすっ飛ばして、
	//インデックスのセットと断面の描画を行う(断面用のテクスチャは割り当てない)


	//スケールだけではなく、回転と平行移動も考慮愛ないといけない
	//平行移動は問題ない。回転は、平面を回転させればいい

	//頂点に裏表の情報を持たせる?
	//シェーダーで分割する?

	//シェーダーで分割して、ストリーム出力で、インデックスも受け取れれば順序の問題は解決。
	//受け取れそうな気はするけど。そうしないと分割しても扱えないし


	//衝突点を求める
	//その位置を元に、シェーダーで点を生成。
	//ストリームで分割した頂点を出力
	//出力した頂点の表裏判定
	//2つのバッファに分ける

	//分割する三角形かどうか判別しないといけないため、
	//結局頂点に情報を追加しないといけない?
	//それとも、シェーダーで求められる?

	//ストリーム出力だとバッファに書き込むから、頂点データ取得と操作ができない?それともテクスチャバッファみたいに読み取れる?

	//ジオメトリシェーダーでやる処理をこちらで実装すれば楽に分割できる?


	//先に三角形ごとに分けて処理したほうがやりやすいかも
	//タプルに、インデックスを参考にValue3で頂点、インデックスの添え字(インデックス書き換え時にアクセスする用)


	//最初の段階で、隣を求めたほうがいい
	//既存の点(元々の三角形の点)と衝突点で距離を求め、その情報を元に、順序を決める

	//それでも無理だから、辺ごとに衝突点格納して、
	//三角形の座標どっちか、その座標が使われてる辺の衝突点、もう一個の衝突点、残りの座標という順序を使えばよい


	if (catFrontModelData || catBackModelData)return false;

	//三角形内部に頂点があるかないかの確認実装しといてね

	//法線0だったら切断できないため、false
	if (plane.GetNormal() == 0.0f)return false;

	// 平面情報(回転適応のため、作り直し)
	PlaneData rotPlane;
	rotPlane.SetPosition(plane.GetPosition());
	rotPlane.SetNormal(LibMath::RotateZXYVector3(plane.GetNormal(),
		DirectX::XMFLOAT3(-modelConstDatas[0].angle.x, -modelConstDatas[0].angle.y, -modelConstDatas[0].angle.z)).Normalize());

	//モデルの頂点を三角形ごとにまとめたもの
	struct ModelTri
	{
		//頂点
		Value3<FbxVertex> vertData;

		//表裏判定結果
		Value3<char>vertFB;

		//三角形の辺の情報
		Value3<Segment3DData>segmentData;

		//平面との衝突判定
		Value3<bool>hitResult;

		//衝突点の頂点情報
		Value3<FbxVertex>hitPosVert;

		//インデックスの配列のインデックス
		Value3<int> indicesIndex = 0;
	};


	std::vector<std::vector<FbxVertex>>vertPos = pModelData->GetVertices();
	std::vector<std::vector<USHORT>>indices = pModelData->GetIndices();

	std::vector<ModelTri>modelTri(indices[0].size() / 3);

	//三角形ごとにデータ格納
	for (int i = 0, size = indices[0].size(); i < size; i += 3)
	{
		int triIndex = 0;
		if (i == 0)triIndex = 0;
		else triIndex = i / 3;

		//頂点情報
		modelTri[triIndex].vertData.v1 = vertPos[0][indices[0][i]];
		modelTri[triIndex].vertData.v2 = vertPos[0][indices[0][i + 1]];
		modelTri[triIndex].vertData.v3 = vertPos[0][indices[0][i + 2]];

		//拡縮、平行移動適応
		//このままだとモデルデータの頂点も倍率かかった状態になるので、注意
		modelTri[triIndex].vertData.v1.pos.x *= modelConstDatas[0].scale.x;
		modelTri[triIndex].vertData.v1.pos.y *= modelConstDatas[0].scale.y;
		modelTri[triIndex].vertData.v1.pos.z *= modelConstDatas[0].scale.z;
		modelTri[triIndex].vertData.v2.pos.x *= modelConstDatas[0].scale.x;
		modelTri[triIndex].vertData.v2.pos.y *= modelConstDatas[0].scale.y;
		modelTri[triIndex].vertData.v2.pos.z *= modelConstDatas[0].scale.z;
		modelTri[triIndex].vertData.v3.pos.x *= modelConstDatas[0].scale.x;
		modelTri[triIndex].vertData.v3.pos.y *= modelConstDatas[0].scale.y;
		modelTri[triIndex].vertData.v3.pos.z *= modelConstDatas[0].scale.z;

		modelTri[triIndex].vertData.v1.pos.x += modelConstDatas[0].position.x;
		modelTri[triIndex].vertData.v1.pos.y += modelConstDatas[0].position.y;
		modelTri[triIndex].vertData.v1.pos.z += modelConstDatas[0].position.z;
		modelTri[triIndex].vertData.v2.pos.x += modelConstDatas[0].position.x;
		modelTri[triIndex].vertData.v2.pos.y += modelConstDatas[0].position.y;
		modelTri[triIndex].vertData.v2.pos.z += modelConstDatas[0].position.z;
		modelTri[triIndex].vertData.v3.pos.x += modelConstDatas[0].position.x;
		modelTri[triIndex].vertData.v3.pos.y += modelConstDatas[0].position.y;
		modelTri[triIndex].vertData.v3.pos.z += modelConstDatas[0].position.z;

		//辺情報
		modelTri[triIndex].segmentData.v1.SetPosition
		(Value2<Vector3>(modelTri[triIndex].vertData.v1.pos, modelTri[triIndex].vertData.v2.pos));
		modelTri[triIndex].segmentData.v2.SetPosition
		(Value2<Vector3>(modelTri[triIndex].vertData.v2.pos, modelTri[triIndex].vertData.v3.pos));
		modelTri[triIndex].segmentData.v3.SetPosition
		(Value2<Vector3>(modelTri[triIndex].vertData.v3.pos, modelTri[triIndex].vertData.v1.pos));

		//インデックスの配列のインデックス
		modelTri[triIndex].indicesIndex.v1 = i;
		modelTri[triIndex].indicesIndex.v2 = i + 1;
		modelTri[triIndex].indicesIndex.v3 = i + 2;

		//裏表判定結果
		modelTri[triIndex].vertFB.v1 = LibMath::PointPlaneFrontBackCheck(modelTri[triIndex].vertData.v1.pos, rotPlane);
		modelTri[triIndex].vertFB.v2 = LibMath::PointPlaneFrontBackCheck(modelTri[triIndex].vertData.v2.pos, rotPlane);
		modelTri[triIndex].vertFB.v3 = LibMath::PointPlaneFrontBackCheck(modelTri[triIndex].vertData.v3.pos, rotPlane);

		//辺の衝突確認、衝突点取得
		Segment3DCalcResult res;
		modelTri[triIndex].hitResult.v1
			= Collision::PlaneAndSegment3D(rotPlane, modelTri[triIndex].segmentData.v1, &res);
		modelTri[triIndex].hitPosVert.v1.pos = res.planeHitPos.ToXMFLOAT3();
		modelTri[triIndex].hitPosVert.v1.normal = modelTri[triIndex].vertData.v1.normal;
		//modelTri[triIndex].hitPosVert.v1.uv = modelTri[triIndex].vertData.v1.normal;

		modelTri[triIndex].hitResult.v2
			= Collision::PlaneAndSegment3D(rotPlane, modelTri[triIndex].segmentData.v2, &res);
		modelTri[triIndex].hitPosVert.v2.pos = res.planeHitPos.ToXMFLOAT3();
		modelTri[triIndex].hitPosVert.v2.normal = modelTri[triIndex].vertData.v2.normal;
		//modelTri[triIndex].hitPosVert.v2.uv = modelTri[triIndex].vertData.v2.normal;

		modelTri[triIndex].hitResult.v3
			= Collision::PlaneAndSegment3D(rotPlane, modelTri[triIndex].segmentData.v3, &res);
		modelTri[triIndex].hitPosVert.v3.pos = res.planeHitPos.ToXMFLOAT3();
		modelTri[triIndex].hitPosVert.v3.normal = modelTri[triIndex].vertData.v3.normal;
		//modelTri[triIndex].hitPosVert.v3.uv = modelTri[triIndex].vertData.v3.normal;
	}

	//全頂点をまとめる配列。表と裏で分ける
	std::vector<FbxVertex>frontVertices;
	std::vector<FbxVertex>backVertices;
	std::vector<USHORT>frontIndices;
	std::vector<USHORT>backIndices;



	//三角形を見ていって、格納していく
	USHORT frontIndex = 0;
	USHORT backIndex = 0;

	for (const auto& tri : modelTri)
	{
		//ヒットしてるのに表裏に分かれてないということは頂点にかすってるだけなので、分割しない
		if (tri.hitResult.v1 || tri.hitResult.v2 || tri.hitResult.v3)
		{

			if (tri.vertFB.v1 != -1 && tri.vertFB.v2 != -1 && tri.vertFB.v3 != -1
				|| tri.vertFB.v1 != 1 && tri.vertFB.v2 != 1 && tri.vertFB.v3 != 1)
			{
				if (tri.vertFB.v1 == 1 || tri.vertFB.v2 == 1 || tri.vertFB.v3 == 1)
				{
					frontVertices.push_back(tri.vertData.v1);
					frontVertices.push_back(tri.vertData.v2);
					frontVertices.push_back(tri.vertData.v3);

					frontIndices.push_back(frontIndex);
					frontIndices.push_back(frontIndex + 1);
					frontIndices.push_back(frontIndex + 2);

					frontIndex += 3;
				}
				else if (tri.vertFB.v1 == -1 || tri.vertFB.v2 == -1 || tri.vertFB.v3 == -1)
				{
					backVertices.push_back(tri.vertData.v1);
					backVertices.push_back(tri.vertData.v2);
					backVertices.push_back(tri.vertData.v3);

					backIndices.push_back(backIndex);
					backIndices.push_back(backIndex + 1);
					backIndices.push_back(backIndex + 2);

					backIndex += 3;
				}
				continue;
			}
		}


		//一時的に格納するための配列
		std::vector<FbxVertex>fVert;
		std::vector<FbxVertex>bVert;


		//片方に偏ってたら(斬られてなかったら)インデックスを通常通り格納して次へ
		if (!tri.hitResult.v1 && !tri.hitResult.v2 && !tri.hitResult.v3)
		{
			if (tri.vertFB.v1 == 1)
			{
				frontVertices.push_back(tri.vertData.v1);
				frontVertices.push_back(tri.vertData.v2);
				frontVertices.push_back(tri.vertData.v3);

				frontIndices.push_back(frontIndex);
				frontIndices.push_back(frontIndex + 1);
				frontIndices.push_back(frontIndex + 2);

				frontIndex += 3;
			}
			else
			{
				backVertices.push_back(tri.vertData.v1);
				backVertices.push_back(tri.vertData.v2);
				backVertices.push_back(tri.vertData.v3);

				backIndices.push_back(backIndex);
				backIndices.push_back(backIndex + 1);
				backIndices.push_back(backIndex + 2);

				backIndex += 3;
			}
			continue;
		}

		//偏ってなかったら、表裏ごとに格納

		//順序決める


		//頂点のインデックス(frontInd、backIndに格納する値)
		std::vector<USHORT>fVertInd;
		std::vector<USHORT>bVertInd;


		//三角形の点がどちらに偏ってるかを調べる
		if (tri.vertFB.v1 == 1)
		{
			fVert.push_back(tri.vertData.v1);
		}
		else if (tri.vertFB.v1 == -1)
		{
			bVert.push_back(tri.vertData.v1);
		}
		if (tri.vertFB.v2 == 1)
		{
			fVert.push_back(tri.vertData.v2);
		}
		else if (tri.vertFB.v2 == -1)
		{
			bVert.push_back(tri.vertData.v2);
		}
		if (tri.vertFB.v3 == 1)
		{
			fVert.push_back(tri.vertData.v3);
		}
		else if (tri.vertFB.v3 == -1)
		{
			bVert.push_back(tri.vertData.v3);

		}


		bool addVert = true;
		//片方の場合(多角形の面形成いらない場合)
		if (fVert.size() == 1)
		{
			frontVertices.push_back(fVert[0]);

			if (tri.hitResult.v1)
			{
				if (Vector3(fVert[0].pos) != Vector3(tri.hitPosVert.v1.pos))
				{
					frontVertices.push_back(tri.hitPosVert.v1);
					fVert.push_back(tri.hitPosVert.v1);
				}
			}
			if (tri.hitResult.v2)
			{
				for (int i = 0; i < fVert.size(); i++)
				{
					if (Vector3(fVert[i].pos) == Vector3(tri.hitPosVert.v2.pos))
					{
						addVert = false;
						break;
					}
				}

				if (addVert)
				{
					frontVertices.push_back(tri.hitPosVert.v2);
					fVert.push_back(tri.hitPosVert.v2);
				}
			}

			addVert = true;
			if (tri.hitResult.v3)
			{
				for (int i = 0; i < fVert.size(); i++)
				{
					if (Vector3(fVert[i].pos) == Vector3(tri.hitPosVert.v3.pos))
					{
						addVert = false;
						break;
					}
				}
				if (addVert)
				{
					frontVertices.push_back(tri.hitPosVert.v3);
					fVert.push_back(tri.hitPosVert.v3);
				}
			}

			Vector3 cross;
			cross = LibMath::CalcNormal(fVert[0].pos, fVert[1].pos, fVert[2].pos);
			if (Vector3(tri.vertData.v1.normal) == cross)
			{
				frontIndices.push_back(frontIndex);
				frontIndices.push_back(frontIndex + 1);
				frontIndices.push_back(frontIndex + 2);
			}
			else//法線が逆だったら、並びを逆にして格納
			{
				frontIndices.push_back(frontIndex + 2);
				frontIndices.push_back(frontIndex + 1);
				frontIndices.push_back(frontIndex);
			}

			frontIndex += 3;
		}
		else
		{
			frontVertices.push_back(fVert[0]);

			int fVertVNum = 0;
			//fVert[0]が含まれててかつ線分が平面に当たってたら衝突点を格納
			if (tri.hitResult.v1)
			{
				if (tri.segmentData.v1.GetPosition().v1 == fVert[0].pos
					|| tri.segmentData.v1.GetPosition().v2 == fVert[0].pos)
				{
					//fVert0Seg = 
					fVertVNum = 1;
					frontVertices.push_back(tri.hitPosVert.v1);
				}
			}
			if (tri.hitResult.v2)
			{
				if (tri.segmentData.v2.GetPosition().v1 == fVert[0].pos
					|| tri.segmentData.v2.GetPosition().v2 == fVert[0].pos)
				{
					fVertVNum = 2;
					frontVertices.push_back(tri.hitPosVert.v2);
				}
			}
			if (tri.hitResult.v3)
			{
				if (tri.segmentData.v3.GetPosition().v1 == fVert[0].pos
					|| tri.segmentData.v3.GetPosition().v2 == fVert[0].pos)
				{
					fVertVNum = 3;
					frontVertices.push_back(tri.hitPosVert.v3);
				}
			}

			//もう一個の衝突点と残りの頂点を格納
			if (tri.hitResult.v1 && fVertVNum != 1)
			{
				frontVertices.push_back(tri.hitPosVert.v1);
			}
			else if (tri.hitResult.v2 && fVertVNum != 2)
			{
				frontVertices.push_back(tri.hitPosVert.v2);
			}
			else if (tri.hitResult.v3 && fVertVNum != 3)
			{
				frontVertices.push_back(tri.hitPosVert.v3);
			}

			frontVertices.push_back(fVert[1]);



			//三角形を形成

			//fVertをクリアして、今回追加した三角形を追加
			fVert.clear();
			fVert.resize(4);
			for (int i = frontIndex; i < frontIndex + 4; i++)
			{
				fVert[i - frontIndex] = frontVertices[i];
			}

			//n多角形の三角形分割を利用してインデックスを求める
			//原点から一番遠い頂点
			FbxVertex farVertex;

			//一番遠い頂点の座標
			float farVertDis = 0.0f;

			//farVertexの添え字(形成後の削除用)
			int farVertIndex = 0;

			std::vector<int>skipVertIndices;

			int vertCount = fVert.size();
			//表
			while (1)
			{
				farVertDis = 0.0f;
				//三角形内部に頂点があるかないかの確認実装しといてね

				//一番遠い頂点を求める
				for (int i = 0, size = fVert.size(); i < size; i++)
				{
					bool skip = false;
					for (const auto& ind : skipVertIndices)
					{
						if (i == ind)skip = true;
					}
					if (skip)continue;


					float dis = Vector3(fVert[i].pos).Length();
					if (farVertDis <= dis)
					{
						farVertex = fVert[i];
						farVertDis = dis;
						farVertIndex = i;
					}
				}

				//10/11 ここ書き換えないといけない(距離じゃなくて配列のインデックスから求める)
				//隣の頂点を求める
				int farAddIndex = farVertIndex + 1;
				/*	if (farAddIndex >= fVert.size())farAddIndex = 0;
					for (const auto& ind : skipVertIndices)
					{
						if (ind == farAddIndex)
						{
							farAddIndex++;
							break;
						}
					}*/

				if (farAddIndex >= fVert.size())farAddIndex = 0;

				bool whileEnd = false;
				while (!whileEnd && skipVertIndices.size() != 0)
				{
					for (const auto& ind : skipVertIndices)
					{
						if (ind == farAddIndex || farAddIndex == farVertIndex)
						{
							farAddIndex++;
							if (farAddIndex >= fVert.size())farAddIndex = 0;
						}
						else
						{
							whileEnd = true;
						}
					}
				}





				int farSubIndex = farVertIndex - 1;
				/*	if (farSubIndex <= 0)farSubIndex = fVert.size() - 1;
					for (const auto& ind : skipVertIndices)
					{
						if (ind == farSubIndex)
						{
							farSubIndex--;
							break;
						}
					}*/
				if (farSubIndex < 0)farSubIndex = 3;
				whileEnd = false;
				while (!whileEnd && skipVertIndices.size() != 0)
				{
					for (const auto& ind : skipVertIndices)
					{
						if (ind == farSubIndex || farSubIndex == farVertIndex || farSubIndex == farAddIndex)
						{
							farSubIndex--;
							if (farSubIndex < 0)farSubIndex = 3;
						}
						else
						{
							whileEnd = true;
						}
					}
				}

				//三角形を形成

				//求めた外積と、面の外積が一致したら計算終了
				//三角形の外積
				Vector3 cross;

				//sub,far,add
				cross = LibMath::CalcNormal(fVert[farSubIndex].pos, farVertex.pos, fVert[farAddIndex].pos);
				if (Vector3(farVertex.normal) == cross)
				{
					frontIndices.push_back(frontIndex + farSubIndex);
					frontIndices.push_back(frontIndex + farVertIndex);
					frontIndices.push_back(frontIndex + farAddIndex);
				}
				else//法線が逆だったら、逆にして格納 2,1,far
				{
					frontIndices.push_back(frontIndex + farAddIndex);
					frontIndices.push_back(frontIndex + farVertIndex);
					frontIndices.push_back(frontIndex + farSubIndex);
				}

				//原点から一番遠い点を削除
				//fVert.erase(fVert.begin() + farVertIndex);
				vertCount--;
				skipVertIndices.push_back(farVertIndex);

				//形成不可になったら終了
				//if (fVert.size() == 2)break;
				if (vertCount == 2)break;

			}
			frontIndex += 4;

		}


		if (bVert.size() == 1)
		{
			backVertices.push_back(bVert[0]);

			bool addVert = true;
			if (tri.hitResult.v1)
			{
				if (Vector3(bVert[0].pos) != Vector3(tri.hitPosVert.v2.pos))
				{
					backVertices.push_back(tri.hitPosVert.v1);
					bVert.push_back(tri.hitPosVert.v1);
				}
			}
			if (tri.hitResult.v2)
			{
				for (int i = 0; i < bVert.size(); i++)
				{
					//これだと、衝突点が他の三角形で含まれたら、絶対に入らなくなって頂点足りなくてエラー出る
					//backVerticesじゃなくてbVertでいける?
					if (Vector3(bVert[i].pos) == Vector3(tri.hitPosVert.v2.pos))
					{
						addVert = false;
						break;
					}
				}
				if (addVert)
				{
					backVertices.push_back(tri.hitPosVert.v2);
					bVert.push_back(tri.hitPosVert.v2);
				}
			}

			addVert = true;
			if (tri.hitResult.v3)
			{
				for (int i = 0; i < bVert.size(); i++)
				{
					if (Vector3(bVert[i].pos) == Vector3(tri.hitPosVert.v3.pos))
					{
						addVert = false;
						break;
					}
				}
				if (addVert)
				{
					backVertices.push_back(tri.hitPosVert.v3);
					bVert.push_back(tri.hitPosVert.v3);
				}
			}

			Vector3 cross;
			cross = LibMath::CalcNormal(bVert[0].pos, bVert[1].pos, bVert[2].pos);
			if (Vector3(tri.vertData.v1.normal) == cross)
			{
				backIndices.push_back(backIndex);
				backIndices.push_back(backIndex + 1);
				backIndices.push_back(backIndex + 2);
			}
			else//法線が逆だったら、並びを逆にして格納
			{
				backIndices.push_back(backIndex + 2);
				backIndices.push_back(backIndex + 1);
				backIndices.push_back(backIndex);
			}

			backIndex += 3;
		}
		else
		{
			backVertices.push_back(bVert[0]);

			int bVertVNum = 0;
			//bVert[0]が含まれててかつ線分が平面に当たってたら衝突点を格納
			if (tri.hitResult.v1)
			{
				if (tri.segmentData.v1.GetPosition().v1 == bVert[0].pos
					|| tri.segmentData.v1.GetPosition().v2 == bVert[0].pos)
				{
					//bVert0Seg = 
					bVertVNum = 1;
					backVertices.push_back(tri.hitPosVert.v1);
				}
			}
			if (tri.hitResult.v2)
			{
				if (tri.segmentData.v2.GetPosition().v1 == bVert[0].pos
					|| tri.segmentData.v2.GetPosition().v2 == bVert[0].pos)
				{
					bVertVNum = 2;
					backVertices.push_back(tri.hitPosVert.v2);
				}
			}
			if (tri.hitResult.v3)
			{
				if (tri.segmentData.v3.GetPosition().v1 == bVert[0].pos
					|| tri.segmentData.v3.GetPosition().v2 == bVert[0].pos)
				{
					bVertVNum = 3;
					backVertices.push_back(tri.hitPosVert.v3);
				}
			}

			//もう一個の衝突点と残りの頂点を格納
			if (tri.hitResult.v1 && bVertVNum != 1)
			{
				backVertices.push_back(tri.hitPosVert.v1);
			}
			else if (tri.hitResult.v2 && bVertVNum != 2)
			{
				backVertices.push_back(tri.hitPosVert.v2);
			}
			else if (tri.hitResult.v3 && bVertVNum != 3)
			{
				backVertices.push_back(tri.hitPosVert.v3);
			}

			backVertices.push_back(bVert[1]);



			//三角形を形成

			//bVertをクリアして、今回追加した三角形を追加
			bVert.clear();
			bVert.resize(4);
			for (int i = backIndex; i < backIndex + 4; i++)
			{
				bVert[i - backIndex] = backVertices[i];
			}

			//n多角形の三角形分割を利用してインデックスを求める
			//原点から一番遠い頂点
			FbxVertex farVertex;

			//一番遠い頂点の座標
			float farVertDis = 0.0f;

			//farVertexの添え字(形成後の削除用)
			int farVertIndex = 0;

			std::vector<int>skipVertIndices;
			int vertCount = bVert.size();
			//裏
			while (1)
			{
				farVertDis = 0.0f;
				//三角形内部に頂点があるかないかの確認実装しといてね

				//一番遠い頂点を求める
				for (int i = 0, size = bVert.size(); i < size; i++)
				{
					bool skip = false;
					for (const auto& ind : skipVertIndices)
					{
						if (i == ind)skip = true;
					}
					if (skip)continue;

					float dis = Vector3(bVert[i].pos).Length();
					if (farVertDis <= dis)
					{
						farVertex = bVert[i];
						farVertDis = dis;
						farVertIndex = i;

					}
				}

				//10/11 ここ書き換えないといけない(距離じゃなくて配列のインデックスから求める)
				//隣の頂点を求める
				int farAddIndex = farVertIndex + 1;
				if (farAddIndex >= bVert.size())farAddIndex = 0;

				/*for (const auto& ind : skipVertIndices)
				{
					if (ind == farAddIndex)
					{
						farAddIndex++;

						if (farAddIndex == farVertIndex)farAddIndex++;
					}
				}*/

				bool whileEnd = false;
				while (!whileEnd && skipVertIndices.size() != 0)
				{
					for (const auto& ind : skipVertIndices)
					{
						if (ind == farAddIndex || farAddIndex == farVertIndex)
						{
							farAddIndex++;
							if (farAddIndex >= bVert.size())farAddIndex = 0;
						}
						else
						{
							whileEnd = true;
						}
					}
				}



				int farSubIndex = farVertIndex - 1;
				if (farSubIndex < 0)farSubIndex = 3;
				/*for (const auto& ind : skipVertIndices)
				{
					if (ind == farSubIndex)
					{
						farSubIndex--;

						if (farSubIndex == farVertIndex || farSubIndex == farAddIndex)farSubIndex--;
					}
				}*/
				whileEnd = false;
				while (!whileEnd && skipVertIndices.size() != 0)
				{
					for (const auto& ind : skipVertIndices)
					{
						if (ind == farSubIndex || farSubIndex == farVertIndex || farSubIndex == farAddIndex)
						{
							farSubIndex--;
							if (farSubIndex < 0)farSubIndex = 3;
						}
						else
						{
							whileEnd = true;
						}
					}
				}


				//三角形を形成

				//求めた外積と、面の外積が一致したら計算終了
				//三角形の外積
				Vector3 cross;

				//sub,far,add
				cross = LibMath::CalcNormal(bVert[farSubIndex].pos, farVertex.pos, bVert[farAddIndex].pos);
				if (Vector3(farVertex.normal) == cross)
				{
					backIndices.push_back(backIndex + farSubIndex);
					backIndices.push_back(backIndex + farVertIndex);
					backIndices.push_back(backIndex + farAddIndex);
				}
				else//法線が逆だったら、逆にして格納 2,1,far
				{
					backIndices.push_back(backIndex + farAddIndex);
					backIndices.push_back(backIndex + farVertIndex);
					backIndices.push_back(backIndex + farSubIndex);
				}

				//原点から一番遠い点を削除
				//bVert.erase(bVert.begin() + farVertIndex);
				vertCount--;
				skipVertIndices.push_back(farVertIndex);


				//形成不可になったら終了
				//if (bVert.size() == 2)break;
				if (vertCount == 2)break;

			}
			backIndex += 4;

		}

	}

	//完全にどっちかに偏っていたら切れてないので、false
	if (frontVertices.size() == 0 || backVertices.size() == 0)return false;

	//ここで、断面の情報をセットしていく

	if (!pFront && !catFrontModelData)
	{
		//頂点とインデックスを元にバッファを作成&更新(Map)
		//バッファ作成よりモデルデータを作成する感じにする?
		std::vector<std::vector<FbxVertex>> vert(1, frontVertices);
		std::vector<std::vector<USHORT>> ind(1, frontIndices);
		catFrontModelData = std::make_unique<ModelData>();
		catFrontModelData->Create(vert, ind);
		pFront = catFrontModelData.get();
	}
	if (!pBack && !catBackModelData)
	{
		std::vector<std::vector<FbxVertex>> vert(1, backVertices);
		std::vector<std::vector<USHORT>> ind(1, backIndices);
		catBackModelData = std::make_unique<ModelData>();
		catBackModelData->Create(vert, ind);
		pBack = catBackModelData.get();
	}

	return true;



}

void ModelObject::SetPosition(const Vector3& position, const std::string& name)
{
	if (name == "")
	{
		for (auto& data : modelConstDatas)
		{
			data.second.position = position.ToXMFLOAT3();
		}
	}
	else
	{
		modelConstDatas[name].position = position.ToXMFLOAT3();
	}
}

void ModelObject::SetScale(const Vector3& scale, const std::string& name)
{
	if (name == "")
	{
		for (auto& data : modelConstDatas)
		{
			data.second.scale = scale.ToXMFLOAT3();
			if (data.second.scale.x == 0.0f)data.second.scale.x = 0.0001f;
			if (data.second.scale.y == 0.0f)data.second.scale.y = 0.0001f;
			if (data.second.scale.z == 0.0f)data.second.scale.z = 0.0001f;
		}
	}
	else
	{
		modelConstDatas[name].scale = scale.ToXMFLOAT3();
		if (modelConstDatas[name].scale.x == 0.0f)modelConstDatas[name].scale.x = 0.0001f;
		if (modelConstDatas[name].scale.y == 0.0f)modelConstDatas[name].scale.y = 0.0001f;
		if (modelConstDatas[name].scale.z == 0.0f)modelConstDatas[name].scale.z = 0.0001f;
	}


}

void ModelObject::SetAngle(const Vector3& angle, const std::string& name)
{
	if (name == "")
	{
		for (auto& data : modelConstDatas)
		{
			data.second.angle = angle.ToXMFLOAT3();
		}
	}
	else
	{
		modelConstDatas[name].angle = angle.ToXMFLOAT3();
	}


}

void MelLib::ModelObject::SetAddColor(const Color& color, const std::string& name)
{

	Value4<float> colorValue4(color.ToFloat());
	DirectX::XMFLOAT4 colorXMFLOAT4(colorValue4.v1, colorValue4.v2, colorValue4.v3, colorValue4.v4);

	if (name == "")
	{
		for (auto& data : modelConstDatas)
		{
			data.second.addColor = colorXMFLOAT4;
		}
	}
	else
	{
		modelConstDatas[name].addColor = colorXMFLOAT4;
	}
}

void MelLib::ModelObject::SetSubColor(const Color& color, const std::string& name)
{
	Value4<float> colorValue4(color.ToFloat());
	DirectX::XMFLOAT4 colorXMFLOAT4(colorValue4.v1, colorValue4.v2, colorValue4.v3, colorValue4.v4);

	if (name == "")
	{
		for (auto& data : modelConstDatas)
		{
			data.second.subColor = colorXMFLOAT4;
		}
	}
	else
	{
		modelConstDatas[name].subColor = colorXMFLOAT4;
	}
}

void MelLib::ModelObject::SetMulColor(const Color& color, const std::string& name)
{
	Value4<float> colorValue4(color.ToFloat());
	DirectX::XMFLOAT4 colorXMFLOAT4(colorValue4.v1, colorValue4.v2, colorValue4.v3, colorValue4.v4);

	if (name == "")
	{
		for (auto& data : modelConstDatas)
		{
			data.second.mulColor = colorXMFLOAT4;
		}
	}
	else
	{
		modelConstDatas[name].mulColor = colorXMFLOAT4;
	}
}


MelLib::ModelObject::ModelObject(ModelObject& obj)
{
	Create(obj.pModelData, nullptr);
	modelConstDatas = obj.modelConstDatas;

	if (obj.catFrontModelData)
	{
		catFrontModelData = std::make_unique<ModelData>();
		*catFrontModelData = *obj.catFrontModelData;
	}
	if (obj.catBackModelData)
	{
		catBackModelData = std::make_unique<ModelData>();
		*catBackModelData = *obj.catBackModelData;
	}
}

ModelObject& MelLib::ModelObject::operator=(ModelObject& obj)
{
	Create(obj.pModelData, nullptr);
	modelConstDatas = obj.modelConstDatas;

	if (obj.catFrontModelData)
	{
		catFrontModelData = std::make_unique<ModelData>();
		*catFrontModelData = *obj.catFrontModelData;
	}
	if (obj.catBackModelData)
	{
		catBackModelData = std::make_unique<ModelData>();
		*catBackModelData = *obj.catBackModelData;
	}
	return *this;
}

bool ModelObject::Initialize(ID3D12Device* dev, const std::vector<ID3D12GraphicsCommandList*>& cmdList)
{
	device = dev;
	cmdLists = cmdList;


#pragma region ディスクリプタレンジ_ルートパラメーター
	CD3DX12_DESCRIPTOR_RANGE descRangeSRV;
	descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);

	CD3DX12_ROOT_PARAMETER rootparam[6] = {};

	//行列や色など
	rootparam[CONST_BUFFER_REGISTER].InitAsConstantBufferView(CONST_BUFFER_REGISTER, 0, D3D12_SHADER_VISIBILITY_ALL);
	//ユーザー定数
	rootparam[USER_BUFFER_REGISTER].InitAsConstantBufferView(USER_BUFFER_REGISTER, 0, D3D12_SHADER_VISIBILITY_ALL);
	//マテリアル
	rootparam[MATERIAL_BUFFER_REGISTER].InitAsConstantBufferView(MATERIAL_BUFFER_REGISTER, 0, D3D12_SHADER_VISIBILITY_PIXEL);
	//PBRマテリアル
	rootparam[COLOR_MATERIAL_BUFFER_REGISTER].InitAsConstantBufferView(COLOR_MATERIAL_BUFFER_REGISTER, 0, D3D12_SHADER_VISIBILITY_PIXEL);
	//モデルごと
	rootparam[MODEL_BUFFER_REGISTER].InitAsConstantBufferView(MODEL_BUFFER_REGISTER, 0, D3D12_SHADER_VISIBILITY_ALL);

	//テクスチャ
	rootparam[TEXURE_ROOTPARAM_NUM].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_PIXEL);

#pragma endregion

#pragma region ルートシグネチャ


	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	rootSignatureDesc.pParameters = rootparam;
	rootSignatureDesc.NumParameters = _countof(rootparam);


	D3D12_STATIC_SAMPLER_DESC samplerDesc{};
	samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_OPAQUE_BLACK;
	samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_POINT;
	samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;
	samplerDesc.MinLOD = 0.0f;
	samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	rootSignatureDesc.pStaticSamplers = &samplerDesc;
	rootSignatureDesc.NumStaticSamplers = 1;

	ComPtr<ID3DBlob>rootSigBlob;
	ComPtr<ID3DBlob>errorBlob;

	auto result = D3D12SerializeRootSignature(
		&rootSignatureDesc,
		D3D_ROOT_SIGNATURE_VERSION_1_0,
		&rootSigBlob, &errorBlob);


	if (FAILED(result))
	{
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";

		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	result = device->CreateRootSignature(
		0,
		rootSigBlob->GetBufferPointer(),
		rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(&rootSignature));

#pragma endregion

	PipelineState::SetModelRootSignature(rootSignature.Get());




}

void MelLib::ModelObject::SetMaterial(Material* mtl, const std::string& name)
{
	if (!mtl)
	{
#ifdef _DEBUG
		OutputDebugString(L"マテリアルのセットに失敗しました。マテリアルがnullptrです。\n");
#endif // _DEBUG
		return;
	}
	if (name == "")
	{
		for (auto& material : materials)
		{
			material.second = mtl;
		}
	}
	else
	{
		materials[name] = mtl;
	}



}

Material* MelLib::ModelObject::GetPMaterial(const std::string& name)
{
	if (name == "")
	{
		return materials[objectNames[0]];
	}
	return materials[name];
}

Vector3 MelLib::ModelObject::GetPosition(const std::string& name) const
{
	if (name == "")
	{
		return modelConstDatas.at(objectNames[0]).position;
	}
	return modelConstDatas.at(name).position;
}
Vector3 MelLib::ModelObject::GetAngle(const std::string& name) const
{
	if (name == "")
	{
		return modelConstDatas.at(objectNames[0]).angle;
	}
	return modelConstDatas.at(name).angle;
}

Vector3 MelLib::ModelObject::GetScale(const std::string& name) const
{
	if (name == "")
	{
		return modelConstDatas.at(objectNames[0]).scale;
	}
	return modelConstDatas.at(name).scale;
}



bool ModelObject::Create(ModelData* pModelData, ConstBufferData* userConstBufferData, const std::string& name)
{
	if (!pModelData)
	{
#ifdef _DEBUG

		OutputDebugStringA(name.data());
		OutputDebugStringW(L"の生成に失敗しました。\n");
#endif // _DEBUG
		return false;
	}

	pModelObjects.emplace(name, std::make_unique<ModelObject>(pModelData, userConstBufferData));


	return true;
}

void ModelObject::Delete(const std::string& name)
{
	pModelObjects.erase(name);
}


void ModelObject::SetCurrentFream(const UINT fream)
{
	FbxTime setTime = fbxAnimationData.animationTimes.startTime * fream;

	if (setTime > fbxAnimationData.animationTimes.endTime) {
		setTime = fbxAnimationData.animationTimes.endTime;
	}

	fbxAnimationData.currentTime = setTime;
}



void ModelObject::FbxAnimation()
{
	// こいつDrawで呼ばないようにする

	if (!isAnimation || pModelData->GetModelFormat() != ModelData::ModelFormat::MODEL_FORMAT_FBX)return;


	//タイムを進める
	if(animationReverse)fbxAnimationData.currentTime += fbxAnimationData.animationTimes.frameTime * -fbxAnimationData.timeMag;
	else fbxAnimationData.currentTime += fbxAnimationData.animationTimes.frameTime * fbxAnimationData.timeMag;

	animationEnd = false;


	/*if (fbxAnimationData.currentTime > fbxAnimationData.animationTimes.endTime) 
	{

		if (animationEndStop) 
		{
			fbxAnimationData.currentTime = fbxAnimationData.animationTimes.endTime;
			return;
		}

		fbxAnimationData.currentTime = fbxAnimationData.animationTimes.startTime;

	}
	else
		if (fbxAnimationData.currentTime < fbxAnimationData.animationTimes.startTime) {

			if (animationEndStop) {
				fbxAnimationData.currentTime = fbxAnimationData.animationTimes.startTime;
				return;
			}

			fbxAnimationData.currentTime = fbxAnimationData.animationTimes.endTime;

		}*/


	if (fbxAnimationData.currentTime >= fbxAnimationData.animationTimes.endTime
		&& !animationReverse
		|| fbxAnimationData.currentTime <= fbxAnimationData.animationTimes.startTime
		&& animationReverse)
	{
		animationEnd = true;
	}

	if(animationReverse)
	{
		if (fbxAnimationData.currentTime < fbxAnimationData.animationTimes.startTime)
		{

			if (animationEndStop)
			{
				fbxAnimationData.currentTime = fbxAnimationData.animationTimes.startTime;

			}
			else 
			{
				fbxAnimationData.currentTime = fbxAnimationData.animationTimes.endTime;
			}
		}
		else if (fbxAnimationData.currentTime > fbxAnimationData.animationTimes.endTime)
		{
			// 途中で再生逆にしても戻すための処理
			fbxAnimationData.currentTime = fbxAnimationData.animationTimes.endTime;
		}
	}
	else
	{
		if (fbxAnimationData.currentTime > fbxAnimationData.animationTimes.endTime)
		{

			if (animationEndStop)
			{
				fbxAnimationData.currentTime = fbxAnimationData.animationTimes.endTime;
				
			}
			else
			{
				fbxAnimationData.currentTime = fbxAnimationData.animationTimes.startTime;
			}
		}
		else if (fbxAnimationData.currentTime < fbxAnimationData.animationTimes.startTime)
		{
			// 途中で再生逆にしても戻すための処理
			fbxAnimationData.currentTime = fbxAnimationData.animationTimes.startTime;
		}
	}


}


bool ModelObject::Create(ModelData* pModelData, ConstBufferData* userConstBufferData)
{
	if (!pModelData)
	{
#ifdef _DEBUG
		OutputDebugStringW(L"pModelDataがnullptrです。生成に失敗しました\n");
#endif // _DEBUG
		return false;
	}

	// オブジェクト名取得
	objectNames = pModelData->GetObjectNames();

#pragma region データセット
	if (userConstBufferData)this->userConstBufferData = *userConstBufferData;

	//objのボーンのMoveVectorの割る処理のために、オブジェクトごとにバッファ作成
	if (pModelData->GetModelFormat() == ModelData::ModelFormat::MODEL_FORMAT_OBJ)
	{
		modelConstBufferData.bufferType = ConstBufferData::BufferType::BUFFER_TYPE_EACH_MODEL_OBJECT;
	}
	else
	{
		modelConstBufferData.bufferType = ConstBufferData::BufferType::BUFFER_TYPE_EACH_MODEL;
	}
	modelConstBufferData.bufferDataSize = sizeof(SkinConstBufferData);

	modelFileObjectNum = pModelData->GetModelFileObjectNumber();
	this->pModelData = pModelData;

#pragma endregion

	CreateConstBuffer();

	for (const auto& objectName : objectNames)
	{
		modelConstDatas.try_emplace(objectName);
	}

	//マテリアル取得
	std::vector<ADSAMaterial*>modelDataMtl = pModelData->GetPMaterial();

	size_t size = modelDataMtl.size();

	for (int i = 0; i < size; i++)
	{
		materials[objectNames[i]] = modelDataMtl[i];
	}

#pragma region アニメーション関係


	boneDatas.resize(pModelData->GetBoneNumber());
	parentBoneDatas.resize(pModelData->GetBoneNumber());
	SkinConstBufferData* skinConstData = nullptr;

	for (auto& buffer : modelConstBuffer)
	{
		buffer->Map(0, nullptr, (void**)&skinConstData);
		for (int i = 0; i < BONE_MAX; i++)
		{
			skinConstData->bones[i] = DirectX::XMMatrixIdentity();
		}
		buffer->Unmap(0, nullptr);
	}

	if (pModelData->GetBoneNumber() != 0 && pModelData->GetModelFormat() == MelLib::ModelData::ModelFormat::MODEL_FORMAT_FBX) {
		fbxAnimationData.animationTimes.frameTime.SetTime(0, 0, 0, 1, 0, FbxTime::EMode::eFrames60);

		// 0番目のアニメーションの時間をセット
		pModelData->GetAnimationTimeData(0, fbxAnimationData.animationTimes.startTime, fbxAnimationData.animationTimes.endTime);
	}
#pragma endregion



	return true;
}



std::vector<std::vector<Vector3>> MelLib::ModelObject::GetVerticesData(const bool scaleImpact, const bool angleImpact, const bool transformImpact)
{
	std::vector<std::vector<Vector3>>verticesPos = pModelData->GetVerticesPosition();

	for (int i = 0, size = objectNames.size(); i < size; i++)
	{
		std::string objectName = objectNames[i];
		for (auto& pos : verticesPos[i])
		{
			Matrix mat = Matrix::GetTranslationMatrix(pos);
			if (scaleImpact) mat *= Matrix::GetScalingMatrix(modelConstDatas[objectName].scale);
			if (angleImpact)mat *= Matrix::GetRotateZXYMatrix(modelConstDatas[objectName].angle);
			if (transformImpact)mat *= Matrix::GetTranslationMatrix(modelConstDatas[objectName].position);
			pos = Vector3(mat[3][0], mat[3][1], mat[3][2]);
		}
	}
	return verticesPos;

}

std::vector<std::vector<TriangleData>> MelLib::ModelObject::GetModelTriangleData() const
{

	std::vector<std::vector<Vector3>>vertPos = pModelData->GetVerticesPosition();
	std::vector<std::vector<USHORT>>vertIndex = pModelData->GetIndices();

	std::vector<std::vector<TriangleData>> result(vertIndex.size());


	for (int i = 0; i < objectNames.size(); i++)
	{
		std::string objectName = objectNames[i];
		result[i].resize(vertPos[i].size() / 3);

		for (int j = 0, loopCount = 0; j < vertIndex[i].size(); j += 3, loopCount++)
		{
			Value3<Vector3>pos(vertPos[i][vertIndex[i][j]], vertPos[i][vertIndex[i][j + 1]], vertPos[i][vertIndex[i][j + 2]]);

			result[i][loopCount].SetPosition(pos * Vector3(modelConstDatas.at(objectName).scale));
			result[i][loopCount].SetAngle(modelConstDatas.at(objectName).angle);
			result[i][loopCount].SetTranslationPosition(modelConstDatas.at(objectName).position);
		}
	}

	return result;
}

Vector3 MelLib::ModelObject::CalcAnimationPosition
(
	const Vector3& pos, const float weigth, const std::string& boneName, const std::string& meshName
	, const MelLib::Vector3& startPos , const MelLib::Vector3& startAngle, const MelLib::Vector3& startScale
)const
{

	// 一旦メッシュの移動量分動かして原点基準で回さないからバグる?
	// 今メッシュの移動とか適応してないから関係ない
	// モデルの拡縮とかが等倍の状態で計算しなかったせいだった

	// モデルの移動量や拡縮が0のときにワールド行列掛けるのがベスト?
	// それか、基準の座標を入れてもらうしかない

	// 親のノードの行列を掛けないとダメ
	//多分影響度100%で大丈夫?

	pModelData->SetFbxAnimStack(fbxAnimationData.currentAnimationName);

	DirectX::XMMATRIX posMat = DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z);

	// 逆行列を掛けることにより、拡縮などしたモデルにそのまま座標を合わせてセットしても正常に動かせる
	DirectX::XMMATRIX startWorldMat = DirectX::XMMatrixIdentity();
	startWorldMat *= DirectX::XMMatrixScaling
	(
		startScale.x,
		startScale.y,
		startScale.z
	);
	startWorldMat *= DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(startAngle.z));
	startWorldMat *= DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(startAngle.x));
	startWorldMat *= DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(startAngle.y));

	startWorldMat *= DirectX::XMMatrixTranslation
	(
		startPos.x,
		startPos.y,
		startPos.z
	);
	posMat *= DirectX::XMMatrixInverse(nullptr, startWorldMat);
	/*Vector3 nodeT = pModelData->GetNode(meshName).translation;
	posMat *= DirectX::XMMatrixTranslation(nodeT.x, nodeT.y, nodeT.z);*/



	// 乗算
	DirectX::XMMATRIX meshGlobalTransform = pModelData->GetMeshGlobalTransform(meshName);
	DirectX::XMMATRIX invMeshGlobalTransform = DirectX::XMMatrixInverse(nullptr, meshGlobalTransform);


	ModelData::FbxBone bone = pModelData->GetFbxBone(boneName);

	// 取得と変換
	DirectX::XMMATRIX matCurrentPose;

	FbxAMatrix fbxCurrentPose =
		bone.fbxCluster->GetLink()->EvaluateGlobalTransform(fbxAnimationData.currentTime);
	FbxLoader::GetInstance()->ConvertMatrixFromFbx(&matCurrentPose, fbxCurrentPose);


	DirectX::XMMATRIX boneMat = DirectX::XMMatrixIdentity();
	boneMat *=
		//meshGlobalTransform *
		bone.invInitialPose *
		matCurrentPose //*
		//invMeshGlobalTransform
		;

	boneMat *= weigth; 
	posMat *= boneMat;

	// 親ボーンの行列を乗算
	/*while (1)
	{
		if (bone.parentBone)
		{
			bone = pModelData->GetFbxBone(bone.parentBone->boneName);
		}
		else
		{
			break;
		}

		fbxCurrentPose =
			bone.fbxCluster->GetLink()->EvaluateGlobalTransform(fbxAnimationData.currentTime);
		FbxLoader::GetInstance()->ConvertMatrixFromFbx(&matCurrentPose, fbxCurrentPose);

		boneMat = DirectX::XMMatrixIdentity();
		boneMat *=
			bone.invInitialPose *
			matCurrentPose;

		posMat *= boneMat;
	}*/

	posMat *= startWorldMat;


	// 基準値を入れる場合、最後にそのままの値使ったワールド行列掛けちゃいけない
	// 基準値との差分を入れないといけない
	DirectX::XMMATRIX worldMat = DirectX::XMMatrixIdentity();
	worldMat *= DirectX::XMMatrixScaling
	(
		modelConstDatas.at(meshName).scale.x / startScale.x,
		modelConstDatas.at(meshName).scale.y / startScale.y,
		modelConstDatas.at(meshName).scale.z / startScale.z
	);
	worldMat *= DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(modelConstDatas.at(meshName).angle.z - startAngle.z));
	worldMat *= DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(modelConstDatas.at(meshName).angle.x - startAngle.x));
	worldMat *= DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(modelConstDatas.at(meshName).angle.y - startAngle.y));

	worldMat *= DirectX::XMMatrixTranslation
	(
		modelConstDatas.at(meshName).position.x - startPos.x,
		modelConstDatas.at(meshName).position.y - startPos.y,
		modelConstDatas.at(meshName).position.z - startPos.z
	);
	posMat *= worldMat;

	return MelLib::Vector3(posMat.r[3].m128_f32[0], posMat.r[3].m128_f32[1], posMat.r[3].m128_f32[2]);

}
void MelLib::ModelObject::SetAnimationReversePlayBack(const bool flag)
{
	animationReverse = flag;

	if (animationReverse) 
	{
		if (fbxAnimationData.currentTime <= fbxAnimationData.animationTimes.startTime)animationEnd = true;
		else animationEnd = false;
	}
	else 
	{
		if (fbxAnimationData.currentTime <= fbxAnimationData.animationTimes.endTime)animationEnd = true;
		else animationEnd = false;
	}
}

void MelLib::ModelObject::SetAnimation(const std::string& name)
{
	if (fbxAnimationData.currentAnimationName == name)return;
	//fbxAnimationData.currentTime = fbxAnimationData.animationTimes.startTime;
	fbxAnimationData.currentAnimationName = name;
	pModelData->GetAnimationTimeData(name, fbxAnimationData.animationTimes.startTime, fbxAnimationData.animationTimes.endTime);


	//pModelData->SetFbxAnimStack(name);
}

