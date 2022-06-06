#include "ModelData.h"

#include"CreateBuffer.h"
#include"ModelLoader.h"
#include"FbxLoader.h"


using namespace MelLib;

std::unordered_map<std::string, std::unique_ptr<ModelData>>ModelData::pModelDatas;
std::unordered_map<ShapeType3D, std::unique_ptr<ModelData>>ModelData::pPrimitiveModelDatas;
std::unique_ptr<ADSAMaterial>ModelData::defaultMaterial;

ID3D12Device* ModelData::device = nullptr;




void ModelData::CreateVertexBufferSet
(
	const size_t vertexSize,
	const std::unordered_map < std::string, size_t>& vertexNum
)
{

	//オブジェクト分リサイズ
	vertexBufferSet.reserve(modelFileObjectNum);

	for (const auto& objectName : objectNames)
	{

		CreateBuffer::GetInstance()->CreateVertexBuffer
		(
			CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
			vertexSize,
			vertexNum.at(objectName),
			vertexBufferSet[objectName]
		);
	}


}

void ModelData::MapVertices(void** data, const std::string& name)
{
	vertexBufferSet[name].vertexBuffer->Map(0, nullptr, data);
}

void ModelData::UnmapVertices(const std::string& name)
{
	vertexBufferSet[name].vertexBuffer->Unmap(0, nullptr);
}

void ModelData::CreateIndexBufferSet(const std::unordered_map<std::string, std::vector<USHORT>>& indices)
{

	indexBufferSet.reserve(modelFileObjectNum);

	for (const auto& objectName : objectNames)
	{

		CreateBuffer::GetInstance()->CreateIndexBuffer
		(
			CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
			indices.at(objectName),
			indexBufferSet[objectName]
		);


	}
}

void ModelData::MapIndices(const std::unordered_map<std::string, std::vector<USHORT>>& indices)
{
	for (const auto& objectName : objectNames)
	{

		//Map
		USHORT* pIndices = nullptr;
		indexBufferSet[objectName].indexBuffer->Map(0, nullptr, (void**)&pIndices);
		std::copy(indices.at(objectName).begin(), indices.at(objectName).end(), pIndices);
		indexBufferSet[objectName].indexBuffer->Unmap(0, nullptr);
	}
}




void MelLib::ModelData::CreatePrimitiveModel()
{
	ModelData* pModelData = nullptr;
	std::vector<FbxVertex> vertices;
	std::vector<USHORT> indices;


	auto setData = [&pModelData, &vertices, &indices]()
	{
		const std::string PRIMITIVE_MODEL_NAME = "main";
		pModelData->objectNames.resize(1, PRIMITIVE_MODEL_NAME);
		pModelData->vertices.emplace(PRIMITIVE_MODEL_NAME,vertices);
		pModelData->indices.emplace(PRIMITIVE_MODEL_NAME, indices);
		pModelData->CreateModel();
		pModelData->directionMaxPos = CalcDirectionMaxPosition(pModelData->vertices, std::vector<std::string>(1, PRIMITIVE_MODEL_NAME));
		pModelData->meshGlobalTransform.emplace(PRIMITIVE_MODEL_NAME, DirectX::XMMatrixIdentity());
		
		/*pModelData->material.resize(1);
		pModelData->material[0] = std::make_unique<ADSAMaterial>();
		pModelData->material[0]->Create(PipelineState::GetDefaultDrawData(PipelineStateType::MODEL));*/
	};


#pragma region TRIANGLE
	pPrimitiveModelDatas.emplace(ShapeType3D::TRIANGLE, std::make_unique<ModelData>());
	pModelData = pPrimitiveModelDatas[ShapeType3D::TRIANGLE].get();

	vertices.resize(3);


	vertices[0].pos = { -0.5f,-0.5f,0.0f };
	vertices[0].uv = { 0.0f,1.0f };
	vertices[0].normal = { 0.0f,0.0f,-1.0f };
	vertices[1].pos = { 0.0f,0.5f ,0.0f };
	vertices[1].uv = { 0.0f,0.0f };
	vertices[1].normal = { 0.0f,0.0f,-1.0f };
	vertices[2].pos = { 0.5f,-0.5f ,0.0f };
	vertices[2].uv = { 1.0f,1.0f };
	vertices[2].normal = { 0.0f,0.0f,-1.0f };


	indices =
	{
		0,1,2,
	};

	setData();
#pragma endregion

#pragma region BOARD
	pPrimitiveModelDatas.emplace(ShapeType3D::BOARD, std::make_unique<ModelData>());
	pModelData = pPrimitiveModelDatas[ShapeType3D::BOARD].get();

	vertices.resize(4);


	vertices[0].pos = { -0.5f,-0.5f,0.0f };
	vertices[0].uv = { 0.0f,1.0f };
	vertices[0].normal = {0.0f,0.0f,-1.0f };
	vertices[1].pos = { -0.5f,0.5f ,0.0f };
	vertices[1].uv = { 0.0f,0.0f };
	vertices[1].normal = { 0.0f,0.0f,-1.0f };
	vertices[2].pos = { 0.5f,-0.5f ,0.0f };
	vertices[2].uv = { 1.0f,1.0f };
	vertices[2].normal = { 0.0f,0.0f,-1.0f };
	vertices[3].pos = { 0.5f,0.5f,0.0f };
	vertices[3].uv = { 1.0f,0.0f };
	vertices[3].normal = { 0.0f,0.0f,-1.0f };

	
	indices =
	{
		0,1,2,2,1,3,//正面
	};

	setData();
#pragma endregion

#pragma region BOX

	pPrimitiveModelDatas.emplace(ShapeType3D::BOX, std::make_unique<ModelData>());

	pModelData = pPrimitiveModelDatas[ShapeType3D::BOX].get();

	vertices.resize(24);
	
	float x = 0.5f;
	float y = 0.5f;
	float z = 0.5f;



	//正面
	vertices[0].pos = { -x,-y,-z };
	vertices[0].uv = { 0.0f,1.0f };
	vertices[1].pos = { -x,y,-z };
	vertices[1].uv = { 0.0f,0.0f };
	vertices[2].pos = { x,-y,-z };
	vertices[2].uv = { 1.0f,1.0f };
	vertices[3].pos = { x,y,-z };
	vertices[3].uv = { 1.0f,0.0f };


	//正面の上
	vertices[4].pos = { -x, y,-z };
	vertices[4].uv = { 0.0f,1.0f };
	vertices[5].pos = { -x, y,z };
	vertices[5].uv = { 0.0f,0.0f };
	vertices[6].pos = { x,y,-z };
	vertices[6].uv = { 1.0f,1.0f };
	vertices[7].pos = { x,y,z };
	vertices[7].uv = { 1.0f,0.0f };

	//正面の裏
	vertices[8].pos = { -x, y,z };
	vertices[8].uv = { 0.0f,1.0f };
	vertices[9].pos = { -x, -y,z };
	vertices[9].uv = { 0.0f,0.0f };
	vertices[10].pos = { x,y,z };
	vertices[10].uv = { 1.0f,1.0f };
	vertices[11].pos = { x,-y,z };
	vertices[11].uv = { 1.0f,0.0f };

	//正面の下
	vertices[12].pos = { -x, -y,z };
	vertices[12].uv = { 0.0f,1.0f };
	vertices[13].pos = { -x, -y,-z };
	vertices[13].uv = { 0.0f,0.0f };
	vertices[14].pos = { x,-y,z };
	vertices[14].uv = { 1.0f,1.0f };
	vertices[15].pos = { x,-y,-z };
	vertices[15].uv = { 1.0f,0.0f };

	//正面の右
	vertices[16].pos = { x,-y,-z };
	vertices[16].uv = { 0.0f,0.0f };
	vertices[17].pos = { x,y,-z };
	vertices[17].uv = { 1.0f,0.0f };
	vertices[18].pos = { x,-y,z };
	vertices[18].uv = { 0.0f,1.0f };
	vertices[19].pos = { x,y,z };
	vertices[19].uv = { 1.0f,1.0f };

	//正面の左
	vertices[20].pos = { -x,-y,z };
	vertices[20].uv = { 1.0f,1.0f };
	vertices[21].pos = { -x,y,z };
	vertices[21].uv = { 0.0f,1.0f };
	vertices[22].pos = { -x,-y,-z };
	vertices[22].uv = { 1.0f,0.0f };
	vertices[23].pos = { -x,y,-z };
	vertices[23].uv = { 0.0f,0.0f };


	//インデックスセット
	indices = 
	{
		0,1,2,2,1,3,//正面
		4,5,6,6,5,7,//正面の上
		8,9,10,10,9,11,	//正面の裏
		12,13,14,14,13,15,//正面の下
		16,17,18,18,17,19,//正面の右
		20,21,22,22,21,23,//正面の左
	};


	//法線計算
	CalcPrimitiveModelNormal(vertices, indices);


	setData();

#pragma endregion



}

void MelLib::ModelData::CalcPrimitiveModelNormal(std::vector<FbxVertex>& vertices, const std::vector<USHORT>& indices)
{
	for (int i = 0, size = indices.size() / 3; i < size; i++)
	{
		USHORT i0 = indices[i * 3 + 0];
		USHORT i1 = indices[i * 3 + 1];
		USHORT i2 = indices[i * 3 + 2];

		Vector3 p0 = vertices[i0].pos;
		Vector3 p1 = vertices[i1].pos;;
		Vector3 p2 = vertices[i2].pos;;

		Vector3 v1 = p1 - p0;
		Vector3 v2 = p2 - p0;

		Vector3 normal = Vector3::Cross(v1, v2).Normalize();

		vertices[i0].normal = normal.ToXMFLOAT3();
		vertices[i1].normal = normal.ToXMFLOAT3();
		vertices[i2].normal = normal.ToXMFLOAT3();

	}

}


void MelLib::ModelData::CreateModel()
{
	//頂点、インデックス、テクスチャバッファ作成
	modelFileObjectNum = vertices.size();
	std::unordered_map<std::string,size_t>verticesNum;

	for (const auto& objectName : objectNames)
	{
		verticesNum[objectName] = vertices[objectName].size();
	}



	BufferPreparationSetColor
	(
		sizeof(FbxVertex),
		verticesNum,
		indices
	);

	//Map
	for (const auto& objectName : objectNames)
	{

		FbxVertex* pFbxVertex = nullptr;
		MapVertices((void**)&pFbxVertex, objectName);

		for (int j = 0; j < verticesNum[objectName]; j++)
		{
			pFbxVertex[j] = vertices[objectName][j];
		}
		UnmapVertices(objectName);
	}


	modelFormat = ModelFormat::MODEL_FORMAT_PRIMITIVE;
	material.reserve(vertices.size());

	
	directionMaxPos = CalcDirectionMaxPosition(vertices,objectNames);
	
}

bool MelLib::ModelData::Create
(
	const std::unordered_map < std::string, std::vector<FbxVertex>>& vertices,
	const std::unordered_map < std::string, std::vector<USHORT>>& indices,
	const bool batchDeletionFlag,
	const std::string& name
)
{
	if(vertices.size() != indices.size())
	{
		
		return false;
	}

	pModelDatas.emplace(name, std::make_unique<ModelData>());

	ModelData* pModelData = pModelDatas[name].get();

	pModelData->vertices = vertices;
	pModelData->indices = indices;
	pModelData->CreateModel();
	pModelData->directionMaxPos = CalcDirectionMaxPosition(vertices, pModelData->objectNames);

	pModelData->material.reserve(vertices.size());
	pModelData->material[name] = std::make_unique<ADSAMaterial>();
	pModelData->material[name]->Create(PipelineState::GetDefaultDrawData(PipelineStateType::MODEL),1);


	pModelDatas[name]->batchDeletionFlag = batchDeletionFlag;

	return true;
}

bool MelLib::ModelData::Create
(
	const std::vector< std::vector<FbxVertex>>& vertices,
	const std::vector< std::vector<USHORT>>& indices,
	const bool batchDeletionFlag,
	const std::string& name
)
{


	if (vertices.size() != indices.size())
	{

		return false;
	}

	pModelDatas.emplace(name, std::make_unique<ModelData>());

	ModelData* pModelData = pModelDatas[name].get();

	//pModelData->vertices = vertices;
	for (int i = 0; i < vertices.size(); i++)
	{
		pModelData->vertices.emplace(std::to_string(i), vertices[i]);

		pModelData->indices.emplace(std::to_string(i), indices[i]);
	}


	pModelData->CreateModel();
	pModelData->directionMaxPos = CalcDirectionMaxPosition(pModelData->vertices,pModelData->objectNames);

	pModelData->material.reserve(vertices.size());
	pModelData->material[name] = std::make_unique<ADSAMaterial>();
	pModelData->material[name]->Create(PipelineState::GetDefaultDrawData(PipelineStateType::MODEL),1);


	pModelDatas[name]->batchDeletionFlag = batchDeletionFlag;

	return true;
}


void MelLib::ModelData::Create
(
	const std::unordered_map < std::string, std::vector<FbxVertex>>& vertices,
	const std::unordered_map < std::string, std::vector<USHORT>>& indices
)
{
	this->vertices = vertices;
	this->indices = indices;
	CreateModel();
	directionMaxPos = CalcDirectionMaxPosition(vertices,objectNames);

	/*material.resize(1);
	material[0] = std::make_unique<ADSAMaterial>();
	material[0]->Create(PipelineState::GetDefaultDrawData(PipelineStateType::MODEL));*/
}


void MelLib::ModelData::Create
(
	const std::vector<std::vector<FbxVertex>>& vertices,
	const std::vector<std::vector<USHORT>>& indices
)
{
}


bool ModelData::Load(const std::string& path, const bool batchDeletionFlag, const std::string& name)
{
	pModelDatas.emplace(name, std::make_unique<ModelData>());
	bool result = pModelDatas[name]->LoadModel(path, name);

	if(!result)
	{
		pModelDatas.erase(name);
		return false;
	}
	
	pModelDatas[name]->batchDeletionFlag = batchDeletionFlag;
	

	return true;
}

void ModelData::Delete(const std::string& name)
{
	pModelDatas.erase(name);
}

void MelLib::ModelData::BatchDeletion()
{
	std::vector<std::string>deleteName;
	deleteName.reserve(pModelDatas.size());
	for(const auto& p : pModelDatas)
	{
		if(p.second->batchDeletionFlag)
		{
			deleteName.push_back(p.first);
		}
	}

	for(const auto& name : deleteName)
	{
		delete pModelDatas[name].release();
		pModelDatas.erase(name);
	}
}

void ModelData::Initialize(ID3D12Device* pDevice)
{
	device = pDevice;
	defaultMaterial = std::make_unique<ADSAMaterial>();
	defaultMaterial->Create(PipelineState::GetDefaultDrawData(PipelineStateType::MODEL),1);
	CreatePrimitiveModel();
}

void MelLib::ModelData::GetAnimationTimeData(int index, FbxTime& start, FbxTime& end)
{
	FbxAnimStack* animStack = fbxData.fbxScene->GetSrcObject<FbxAnimStack>(index);
	if (!animStack)return;

	FbxTakeInfo* takeInfo = fbxData.fbxScene->GetTakeInfo(animStack->GetName());

	start = takeInfo->mLocalTimeSpan.GetStart();
	end = takeInfo->mLocalTimeSpan.GetStop();
}

void MelLib::ModelData::GetAnimationTimeData(const std::string& name, FbxTime& start, FbxTime& end)
{
	FbxTakeInfo* takeInfo = fbxData.fbxScene->GetTakeInfo(fbxData.animationDataGetName[name].c_str());
	if (!takeInfo)return;

	start = takeInfo->mLocalTimeSpan.GetStart();
	end = takeInfo->mLocalTimeSpan.GetStop();
}

std::vector<DirectX::XMMATRIX> MelLib::ModelData::GetMeshGlobalTransforms()
{
	std::vector<DirectX::XMMATRIX> mats(meshGlobalTransform.size());
	
	for (int i = 0; i < objectNames.size(); i++)
	{
		mats[i] = meshGlobalTransform[objectNames[i]];
	
	}
	return mats;


}


const MelLib::ModelData::FbxBone& MelLib::ModelData::GetFbxBone(const std::string& name) const
{
	for (int i = 0; i < boneNum; i++)
	{
		if (fbxData.bones[i].boneName == name)return fbxData.bones[i];
	}
	return FbxBone(name);
}

std::vector<std::vector<USHORT>> MelLib::ModelData::GetIndices() const
{
	std::vector<std::vector<USHORT>>index(objectNames.size());
	
	for (int i = 0; i < objectNames.size(); i++)
	{
		index[i] = indices.at(objectNames[i]);
	}
	return index;
}

std::vector<VertexBufferSet> MelLib::ModelData::GetVertexBufferSet()const
{
	std::vector<VertexBufferSet> set(objectNames.size());
	
	for (int i = 0; i < objectNames.size(); i++)
	{
		set[i] = vertexBufferSet.at(objectNames[i]);
	}
	return set;
}

std::vector<IndexBufferSet> MelLib::ModelData::GetIndexBufferSet()const
{
	std::vector<IndexBufferSet>set(objectNames.size());
	for (int i = 0; i < objectNames.size(); i++)
	{
		set[i] = indexBufferSet.at(objectNames[i]);
	}
	return set;
}

void MelLib::ModelData::SetFbxAnimStack(const std::string& name)
{
	FbxAnimStack* stack = fbxData.fbxScene->GetSrcObject<FbxAnimStack>(fbxData.animStackNum[name]);
	fbxData.fbxScene->SetCurrentAnimationStack(stack);
}


bool ModelData::LoadModel(const std::string& path, const std::string& name)
{
	bool result = false;
	
	//読み込み結果がfalseだったときの終了処理
	auto loadFalseEndProcess = [&path]()
	{
		OutputDebugStringA(path.data());
		OutputDebugStringW(L"を読み込めませんでした。対応していないモデル形式、または、pathの入力ミスの可能性が考えられます。\n");

		return false;
	};

	if (path.find(".obj") != std::string::npos)
	{
		//オブジェクトのマテリアル名格納
		std::string materialFileName;
		std::unordered_map<std::string,std::string>materialName;

		std::vector<Vector3> objBonePositions;
		std::unordered_map<std::string,std::vector<int>> objBoneNums;
		
		//[obj内のオブジェクト分]スムーズシェーディングの計算用データ
		std::unordered_map<std::string ,std::unordered_map < USHORT, std::vector<USHORT> >>smoothData;
		result = ModelLoader::GetInstance()->LoadObjModel
		(
			path,
			true,
			true,
			objectNames,
			vertices,
			indices,
			materialFileName,
			materialName,
			smoothData,
			nullptr,
			&objBonePositions,
			&objBoneNums
		);
		boneNum = objBonePositions.size();
		modelFileObjectNum = vertices.size();

		if (!result)return loadFalseEndProcess();


		if (boneNum == 0)
		{
			//ボーンがなかったら0にしとく
			for (auto& v : vertices)
			{
				std::string objName = v.first;
				auto vertexNum = vertices[objName].size();
				for (int j = 0; j < vertexNum; j++) 
				{
					vertices[objName][j].boneIndex[0] = 0;
					vertices[objName][j].boneWeight[0] = 1;

				}
			}
		}
		else
		{
			for (auto& v : vertices)
			{
				std::string objName = v.first;
				auto vertexNum = vertices[objName].size();

				for (int j = 0; j < vertexNum; j++)
				{
					vertices[objName][j].boneIndex[0] = objBoneNums[objName][j];
					vertices[objName][j].boneWeight[0] = 1;

					//objでは0しか使わないので、0
					for (int k = 1; k < _countof(vertices[objName][j].boneIndex); k++)
					{
						vertices[objName][j].boneIndex[k] = 0;
						vertices[objName][j].boneWeight[k] = 0;

					}
				}
			}
		}

		//テクスチャ反転
		for (auto& v : vertices)
		{
			for (auto& v2 : v.second)
			{
				v2.uv.y = (v2.uv.y - 1) * -1;
			}
		}


#pragma region 法線計算

		auto vertNum = vertices.size();
		smoothNormal.reserve(vertNum);
		for (const auto& objectName : objectNames)
		{
			smoothNormal[objectName].resize(vertices[objectName].size());
		}

		for (const auto& objectName : objectNames)
		{
			for (int j = 0; j < vertices[objectName].size(); j++)
			{
				smoothNormal[objectName][j] = vertices[objectName][j].normal;
			}
		}

		//法線(平均求める用配列。ここに入れて、平均を求める)
		std::vector<DirectX::XMFLOAT3>sNor;

		//オブジェクト分ループ
		for (const auto& objectName : objectNames)
		{
			auto itr = smoothData[objectName].begin();
			std::vector<USHORT>ver;
			for (; itr != smoothData[objectName].end(); ++itr)
			{
				ver = itr->second;
				for (auto& v : ver)
				{
					//一気に24個入ってるし、clearしてないからおかしかった
					sNor.push_back(vertices[objectName][v].normal);
				}

				//法線平均化
				DirectX::XMVECTOR aveNormal = { 0,0,0 };
				for (auto& n : sNor)
				{
					aveNormal.m128_f32[0] += n.x;
					aveNormal.m128_f32[1] += n.y;
					aveNormal.m128_f32[2] += n.z;
				}
				aveNormal.m128_f32[0] /= sNor.size();
				aveNormal.m128_f32[1] /= sNor.size();
				aveNormal.m128_f32[2] /= sNor.size();
				aveNormal = DirectX::XMVector3Normalize(aveNormal);
				for (auto& v : ver)
				{
					smoothNormal[objectName][v] = { aveNormal.m128_f32[0],aveNormal.m128_f32[1], aveNormal.m128_f32[2] };
				}
				sNor.clear();
			}
		}


#pragma endregion


#pragma region ディレクトリパスとモデル名取得
		std::string directoryPath;
		std::string fullPath = path;

		auto fullPathSize = fullPath.size();
		int loopCount = 0;
		for (int i = fullPathSize - 1;; i--)
		{
			if (fullPath[i] == '/' ||
				fullPath[i] == '\\')
			{
				directoryPath.resize(fullPathSize - loopCount);
				std::copy
				(
					fullPath.begin(),
					fullPath.begin() + i + 1,
					directoryPath.begin()
				);
				break;
			}
			else
			{
				modelName.push_back(fullPath[i]);
			}

			loopCount++;
		}

		std::reverse(modelName.begin(), modelName.end());
#pragma endregion





#pragma region マテリアル


		int materialNum = 0;
		std::unordered_map<std::string,std::string>texPath;
		std::unordered_map<std::string, ADSAMaterialData>mtlData;
		result = ModelLoader::GetInstance()->LoadObjMaterial
		(
			directoryPath,
			materialFileName,
			objectNames,
			texPath,
			mtlData,
			&materialNum
		);
		//if (!result)loadFalseEndProcess();


		if (materialNum != 0) 
		{
			pTexture.reserve(materialNum);
			material.reserve(materialNum);
			for(const auto& objectName:objectNames)
			{
				pTexture[objectName] = std::make_unique<Texture>();
				pTexture[objectName]->LoadModelTexture(texPath[objectName]);
				material[objectName] = std::make_unique<ADSAMaterial>();
				material[objectName]->Create(PipelineState::GetDefaultDrawData(PipelineStateType::MODEL),1);
				material[objectName]->SetTexture(pTexture[objectName].get());
				material[objectName]->SetMaterialData(mtlData[objectName]);
			}
		}
		else
		{


		}
#pragma endregion

		modelName = path;

		modelFormat = ModelFormat::MODEL_FORMAT_OBJ;

		// グローバルトランスフォーム行列は単位入れとく
		for (const auto& objectName : objectNames)
		{
			meshGlobalTransform[objectName] = DirectX::XMMatrixIdentity();
		}
	}
	else 
	if (path.find(".fbx") != std::string::npos)
	{	
		if (!FbxLoader::GetInstance()->LoadFbxModel(path, this))return loadFalseEndProcess();

		boneNum = fbxData.bones.size();
		modelFormat = ModelFormat::MODEL_FORMAT_FBX;

#pragma region アニメーション関係準備
		if (fbxData.bones.size() != 0)
		{
			
			// アニメーション情報が取得できなくなるまでループ
			for (int i = 0; ; i++) 
			{
				FbxAnimStack* animStack = fbxData.fbxScene->GetSrcObject<FbxAnimStack>(i);

				if (!animStack)break;

				// アニメーション名を取得
				std::string getAnimationName = animStack->GetName();
				size_t nameStart = getAnimationName.find_first_of("|") + 1;
				std::string animationName = getAnimationName.substr(nameStart, getAnimationName.size() - nameStart);

				fbxData.animStackNum.emplace(animationName, i);
				fbxData.animationDataGetName.emplace(animationName, getAnimationName);
			}


			/*FbxTakeInfo* takeInfo = fbxData.fbxScene->GetTakeInfo(animStackname);

			fbxData.animationTimes.startTime = takeInfo->mLocalTimeSpan.GetStart();
			fbxData.animationTimes.endTime = takeInfo->mLocalTimeSpan.GetStop();*/

			//1秒60フレームで設定されてるアニメーションの場合、eFream60って設定する?
			//fbxData.animationTimes.freamTime.SetTime(0, 0, 0, 1, 0, FbxTime::EMode::eFrames60);
		}


#pragma endregion

	}
	else
	{
		loadFalseEndProcess();
	}




	//頂点、インデックス、テクスチャバッファ作成
	modelFileObjectNum = vertices.size();
	std::unordered_map < std::string, size_t> verticesNum;

	for (const auto& objectName : objectNames)
	{
		verticesNum[objectName] = vertices[objectName].size();
	}


	BufferPreparationSetTexture
	(
		sizeof(FbxVertex),
		verticesNum,
		indices
	);

	//Map
	for (const auto& objectName : objectNames)
	{
		FbxVertex* pFbxVertex = nullptr;
		MapVertices((void**)&pFbxVertex, objectName);

		for(int j = 0; j < verticesNum[objectName];j++)
		{
			pFbxVertex[j] = vertices[objectName][j];
		}
		UnmapVertices(objectName);
	}

	//端っこ計算
	directionMaxPos = CalcDirectionMaxPosition(vertices,objectNames);

	//マテリアル作成
	//読み込み時に生成するからここに書く必要ない
	//material.resize(modelFileObjectNum);
	//for(int i = 0; i < modelFileObjectNum;i++)
	//{
	//	material[i].Create(PipelineState::GetDefaultDrawData(PipelineStateType::MODEL));
	//	
	//	//マテリアルの値読み込むときに読み込むからいらん
	//	//material[i].SetTexture(pTextures[i].get());
	modelPath = path;

	return true;
}



void ModelData::BufferPreparationSetTexture
(
	const size_t vertexSize,
	const std::unordered_map < std::string, size_t>& vertexNum,
	const std::unordered_map < std::string, std::vector<USHORT>>& indices
)
{
	CreateVertexBufferSet
	(
		vertexSize,
		vertexNum
	);


	CreateIndexBufferSet(indices);
	MapIndices(indices);


}


void ModelData::BufferPreparationSetColor
(
	const size_t vertexSize,
	const std::unordered_map < std::string, size_t>& vertexNum,
	const std::unordered_map < std::string, std::vector<USHORT>>& indices
)
{

	CreateVertexBufferSet
	(
		vertexSize,
		vertexNum
	);

	CreateIndexBufferSet(indices);
	MapIndices(indices);

}


std::vector<ADSAMaterial*> MelLib::ModelData::GetPMaterial()
{
	size_t size = material.size();
	if (size == 0)
	{
		size = objectNames.size();
		for (int i = 0; i < objectNames.size(); i++)
		{
			material[objectNames[i]] = nullptr;
		}
	}

	std::vector<ADSAMaterial*>pMtls(size);
	for (int i = 0; i < objectNames.size(); i++)
	{
		if (!material[objectNames[i]]) pMtls[i] = defaultMaterial.get();
		else pMtls[i] = material[objectNames[i]].get();
	}

	

	return pMtls;

}

std::vector<std::array<float, 6>> MelLib::ModelData::GetDirectionMaxPosition() const
{
	std::vector<std::array<float, 6>>pos(objectNames.size());
	for (int i = 0; i < objectNames.size(); i++)
	{
		pos[i] = directionMaxPos.at(objectNames[i]);
	}
	return pos;
}

std::vector<std::vector<FbxVertex>>MelLib::ModelData::GetVertices()const
{
	std::vector<std::vector<FbxVertex>>pos(objectNames.size());
	for (int i = 0; i < objectNames.size(); i++)
	{
		pos[i] = vertices.at(objectNames[i]);
	}
	return pos;
}


std::vector<std::vector<Vector3>> MelLib::ModelData::GetVerticesPosition()const
{
	std::vector<std::vector<Vector3>>verticesPos(vertices.size());
	
	
	for (int i = 0; i < objectNames.size(); i++)
	{
		verticesPos[i].resize(vertices.at(objectNames[i]).size());


		for (int j = 0,size = verticesPos[i].size(); j < size; j++)
		{
			verticesPos[i][j] = vertices.at(objectNames[i])[j].pos;
		}

	}

	return verticesPos;
	
}

std::unordered_map < std::string, std::array<float, 6>> MelLib::ModelData::CalcDirectionMaxPosition
(
	const std::unordered_map < std::string, std::vector<FbxVertex>>& vertices,
	const std::vector<std::string>& objectNames

)
{
	std::unordered_map < std::string, std::array<float, 6>> pos;

	for(int i = 0; i < objectNames.size();i++)
	{
		pos[objectNames[i]] = std::array<float, 6>({ -FLT_MAX, FLT_MAX, FLT_MAX,-FLT_MAX, FLT_MAX,-FLT_MAX });

		for(const auto& v : vertices.at(objectNames[i]))
		{
			if (v.pos.y > pos[objectNames[i]][0])pos[objectNames[i]][0] = v.pos.y;
			if (v.pos.y < pos[objectNames[i]][1])pos[objectNames[i]][1] = v.pos.y;
			if (v.pos.x < pos[objectNames[i]][2])pos[objectNames[i]][2] = v.pos.x;
			if (v.pos.x > pos[objectNames[i]][3])pos[objectNames[i]][3] = v.pos.x;
			if (v.pos.z < pos[objectNames[i]][4])pos[objectNames[i]][4] = v.pos.z;
			if (v.pos.z > pos[objectNames[i]][5])pos[objectNames[i]][5] = v.pos.z;
		}
	}
	return pos;
}

void MelLib::ModelData::AddLoadCollisionData(const std::string& modelName)
{

	auto copyKeyName = [&modelName](const int start,const int end)
	{
		std::string keyName;

		// 10は、Collision_の文字数
		keyName = modelName.substr(start + 10, modelName.size() - start - end);

		return keyName;
	};
	

	if(modelName.find("Sphere_") != std::string::npos)
	{
		SphereData data;

		Node& meshNode = fbxData.nodes[modelName];

		DirectX::XMVECTOR pos = meshNode.translation;
		DirectX::XMMATRIX posMat = DirectX::XMMatrixTranslation(pos.m128_f32[0], pos.m128_f32[1], pos.m128_f32[2]);
		posMat *= meshNode.parentNode->globalTransform;

		data.SetPosition(Vector3(posMat.r[3].m128_f32[0], posMat.r[3].m128_f32[1], posMat.r[3].m128_f32[2]));
		data.SetRadius(meshNode.scaling.m128_f32[0]);

		sphereDatas.emplace(copyKeyName(7, 0), data);
	}
	else
		if(modelName.find("Box_") != std::string::npos)
	{
	}


}

MelLib::ModelData::ModelData(ModelData& data)
{
	Create(data.vertices, data.indices);

	modelName = data.modelName;
	pTexture.reserve(data.vertices.size());
	material.reserve(data.material.size());

	for (int i = 0; i < data.objectNames.size(); i++)
	{
		//nullptrじゃなかったら生成(代入したら生成される)
		if (data.material[data.objectNames[i]])
		{
			material[data.objectNames[i]] = std::make_unique<ADSAMaterial>();
			*material[data.objectNames[i]] = *data.material[data.objectNames[i]];
		}
	}
}

ModelData& MelLib::ModelData::operator=(ModelData& data)
{
	Create(data.vertices, data.indices);

	modelName = data.modelName;
	pTexture.reserve(data.vertices.size());
	material.reserve(data.material.size());

	for (int i = 0; i < data.objectNames.size(); i++)
	{
		//nullptrじゃなかったら生成(代入したら生成される)
		if (data.material[data.objectNames[i]])
		{
			material[data.objectNames[i]] = std::make_unique<ADSAMaterial>();
			*material[data.objectNames[i]] = *data.material[data.objectNames[i]];
		}
	}

	return *this;
}

MelLib::ModelData::~ModelData()
{

	if(fbxData.fbxScene 
		&& FbxLoader::GetInstance()->GetInitializeFlag())fbxData.fbxScene->Destroy();
}

