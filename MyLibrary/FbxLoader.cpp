#include "FbxLoader.h"

#include<cassert>
#include<filesystem>


using namespace MelLib;

FbxLoader::FbxLoader()
{
}


FbxLoader::~FbxLoader()
{
}

FbxLoader* FbxLoader::GetInstance()
{
	static FbxLoader instance;
	return &instance;
}

void FbxLoader::Initialize(ID3D12Device* device)
{
	assert(!fbxManager);

	this->device = device;

	fbxManager = FbxManager::Create();

	FbxIOSettings* ios = FbxIOSettings::Create(fbxManager, IOSROOT);
	fbxManager->SetIOSettings(ios);

	fbxImporter = FbxImporter::Create(fbxManager, "");

	initializeFlag = true;
}

void FbxLoader::Finalize()
{
	//Importerを開放してからManagerを開放する
	fbxImporter->Destroy();
	fbxManager->Destroy();

	initializeFlag = false;
}

bool FbxLoader::LoadFbxModel(const std::string& modelPath, ModelData* fbxModel)
{
	std::string modelName;
	ExtractFileName(modelPath, &modelDirectryPath, &modelName);

	if (!fbxImporter->Initialize
	(
		modelPath.c_str(),
		-1,
		fbxManager->GetIOSettings()
	)) return false;

	ModelData* model = fbxModel;

	FbxScene*& fbxScene = model->fbxData.fbxScene;
	fbxScene = FbxScene::Create(fbxManager, "fbxScene");
	fbxImporter->Import(fbxScene);


	model->modelName = modelName;

	//ノード数取得
	int nodeCount = fbxScene->GetNodeCount();
	//parseNodeRecursive()で配列を参照してるので、メモリを確保しなおさないようにするためにreserveしてる
	model->fbxData.nodes.reserve(nodeCount);


	ParseNodeRecursive(model, fbxScene->GetRootNode());

	return true;
}


void FbxLoader::ParseNodeRecursive
(
	ModelData* fbxModel,
	FbxNode* fbxNode,
	Node* parentNode
)
{
	using namespace DirectX;

	//fbxModel->fbxData.nodes.emplace(,);
	Node& node = fbxModel->fbxData.nodes[fbxNode->GetName()];

	node.nodeName = fbxNode->GetName();

	FbxDouble3 rotation = fbxNode->LclRotation.Get();
	FbxDouble3 scaling = fbxNode->LclScaling.Get();
	FbxDouble3 translation = fbxNode->LclTranslation.Get();

	node.rotation =
	{
		(float)rotation[0],
		(float)rotation[1],
		(float)rotation[2],
		0.0f
	};

	node.scaling =
	{
		(float)scaling[0],
		(float)scaling[1],
		(float)scaling[2],
		0.0f
	};


	node.translation =
	{
		(float)translation[0],
		(float)translation[1],
		(float)translation[2],
		0.0f
	};

	XMMATRIX matScaling, matRotation, matTranslation;
	matScaling = XMMatrixScalingFromVector(node.scaling);
	matRotation = XMMatrixRotationRollPitchYawFromVector(node.rotation);
	matTranslation = XMMatrixTranslationFromVector(node.translation);

	node.transform = XMMatrixIdentity();
	node.transform *= matScaling;
	node.transform *= matRotation;
	node.transform *= matTranslation;

	node.globalTransform = node.transform;

	if (parentNode)
	{
		//親ノード代入
		node.parentNode = parentNode;

		//親の行列を乗算
		node.globalTransform *= parentNode->globalTransform;
	}

	//FbxNodeAttribute ノードの追加情報
	FbxNodeAttribute* fbxNodeAttribute = fbxNode->GetNodeAttribute();
	if (fbxNodeAttribute)
	{
		if (fbxNodeAttribute->GetAttributeType() ==
			FbxNodeAttribute::eMesh)
		{
			fbxModel->fbxData.meshNode = &node;
			ParseMesh(fbxModel, fbxNode, &node);
		}
	}

	for (int i = 0; i < fbxNode->GetChildCount(); i++) {
		ParseNodeRecursive(fbxModel, fbxNode->GetChild(i), &node);
	}

}



void FbxLoader::ParseMesh(ModelData* fbxModel, FbxNode* node, Node* meshNode)
{
	FbxMesh* fbxMesh = node->GetMesh();

	//fbxModel->meshGlobalTransform.push_back(meshNode->globalTransform);
	fbxModel->meshGlobalTransform.emplace(meshNode->nodeName, meshNode->globalTransform);

	std::string objectName = meshNode->nodeName;
	fbxModel->objectNames.push_back(objectName);

	// 判定モデルか確認
	if (objectName.find("Collision_") != std::string::npos)
	{
		fbxModel->AddLoadCollisionData(objectName);
	}

	ParseMeshVertices(fbxModel, fbxMesh, meshNode, objectName);
	ParseMeshFaces(fbxModel, fbxMesh, objectName);
	ParseMaterial(fbxModel, node, objectName);
	ParseSkin(fbxModel, fbxMesh, node, objectName);
}

void FbxLoader::ParseMeshVertices(ModelData* fbxModel, FbxMesh* fbxMesh, Node* meshNode, const std::string& name)
{
	//頂点数取得
	const int vertexNum = fbxMesh->GetControlPointsCount();

	auto& vertices = fbxModel->vertices;
	vertices[name].resize(vertexNum);

	//座標取得
	FbxVector4* pCount = fbxMesh->GetControlPoints();

	for (int i = 0; i < vertexNum; i++)
	{
		// コピー
		vertices[name][i].pos.x = (float)pCount[i][0];
		vertices[name][i].pos.y = (float)pCount[i][1];
		vertices[name][i].pos.z = (float)pCount[i][2];


		// これはここじゃなくてMapするときに掛けること
		// そうしないとスキンアニメーションバグる
		// もしかして最初から適応されてる?
	/*	DirectX::XMFLOAT3 pos = vertices[name][i].pos;
		DirectX::XMMATRIX posMat = DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z);
		posMat *= meshNode->transform;

		vertices[name][i].pos.x = posMat.r[3].m128_f32[0];
		vertices[name][i].pos.y = posMat.r[3].m128_f32[1];
		vertices[name][i].pos.z = posMat.r[3].m128_f32[2];*/
	}

}

void FbxLoader::ParseMeshFaces(ModelData* fbxModel, FbxMesh* fbxMesh, const std::string& name)
{
	auto& vertices = fbxModel->vertices;
	auto& indicesVector = fbxModel->indices;
	auto& indices = indicesVector[name];

	assert(indices.size() == 0);

	//面数
	const int polygonCount = fbxMesh->GetPolygonCount();
	const int textureUVCount = fbxMesh->GetTextureUVCount();

	//UV名のリスト
	FbxStringList uvNames;
	fbxMesh->GetUVSetNames(uvNames);

	//面ごとの情報読み込み
	for (int i = 0; i < polygonCount; i++)
	{
		//面のインデックス数
		const int polygonSize = fbxMesh->GetPolygonSize(i);
		assert(polygonSize <= 4);

		for (int j = 0; j < polygonSize; j++)
		{
			//verticesの添え字
			int index = fbxMesh->GetPolygonVertex(i, j);
			assert(index >= 0);


			//法線読み込み
			FbxVector4 normal;

			//GetPolygonVertexNormal(面番号,面の何個目の頂点か)
			if (fbxMesh->GetPolygonVertexNormal(i, j, normal))
			{
				vertices[name][index].normal.x = (float)normal[0];
				vertices[name][index].normal.y = (float)normal[1];
				vertices[name][index].normal.z = (float)normal[2];
			}


			//UV読み込み
			if (textureUVCount > 0)
			{
				FbxVector2 uvs;
				bool lUnmappedUV;

				if (fbxMesh->GetPolygonVertexUV(i, j, uvNames[0], uvs, lUnmappedUV))
				{
					vertices[name][index].uv.x = (float)uvs[0];
					vertices[name][index].uv.y = (float)uvs[1];
				}
			}

			if (j < 3)
				indices.push_back(index);
			else
			{
				int index2 = indices[indices.size() - 1];
				int index3 = index;
				int index0 = indices[indices.size() - 3];
				indices.push_back(index2);
				indices.push_back(index3);
				indices.push_back(index0);
			}
		}

	}
}

void FbxLoader::ParseMaterial(ModelData* fbxModel, FbxNode* fbxNode, const std::string& name)
{
	const int materialCount = fbxNode->GetMaterialCount();

	if (materialCount == 0)return;

	fbxModel->material[name] = std::make_unique<ADSAMaterial>();
	fbxModel->material[name]->Create(PipelineState::GetDefaultDrawData(PipelineStateType::MODEL),1);

	fbxModel->pTexture[name] = std::make_unique<Texture>();


	ADSAMaterialData mtl;

	//テスト用
	/*auto& pbrMaterialVector = fbxModel->pbrMaterials;
	pbrMaterialVector.resize(1);
	PbrMaterial& pbrModelMaterial = pbrMaterialVector[0];*/



	if (materialCount > 0)
	{
		FbxSurfaceMaterial* materials = fbxNode->GetMaterial(0);

		bool textureLoader = false;

		if (materials)
		{

			if (materials->GetClassId().Is(FbxSurfaceLambert::ClassId))
			{
				FbxSurfaceLambert* lambert = static_cast<FbxSurfaceLambert*>(materials);


				FbxPropertyT<FbxDouble3> ambient = lambert->Ambient;
				mtl.ambient.v1 = (float)ambient.Get()[0];
				mtl.ambient.v2 = (float)ambient.Get()[1];
				mtl.ambient.v3 = (float)ambient.Get()[2];

				FbxPropertyT<FbxDouble3> diffuse = lambert->Diffuse;
				mtl.diffuse.v1 = (float)diffuse.Get()[0];
				mtl.diffuse.v2 = (float)diffuse.Get()[1];
				mtl.diffuse.v3 = (float)diffuse.Get()[2];



			}
			else if (materials->GetClassId().Is(FbxSurfacePhong::ClassId))
			{
				FbxSurfacePhong* phong = static_cast<FbxSurfacePhong*>(materials);


				FbxPropertyT<FbxDouble3> ambient = phong->Ambient;
				mtl.ambient.v1 = (float)ambient.Get()[0];
				mtl.ambient.v2 = (float)ambient.Get()[1];
				mtl.ambient.v3 = (float)ambient.Get()[2];

				FbxPropertyT<FbxDouble3> diffuse = phong->Diffuse;
				mtl.diffuse.v1 = (float)diffuse.Get()[0];
				mtl.diffuse.v2 = (float)diffuse.Get()[1];
				mtl.diffuse.v3 = (float)diffuse.Get()[2];

				FbxPropertyT<FbxDouble3> specular = phong->Specular;
				mtl.specular.v1 = (float)specular.Get()[0];
				mtl.specular.v2 = (float)specular.Get()[1];
				mtl.specular.v3 = (float)specular.Get()[2];
			}
			else
			{/*
				const FbxProperty propBaseColor =
					FbxSurfaceMaterialUtils::GetProperty("baseColor", materials);
				if (propBaseColor.IsValid())
				{

					FbxDouble3 baseColor = propBaseColor.Get<FbxDouble3>();

					pbrModelMaterial.baseColor.x = (float)baseColor.Buffer()[0];
					pbrModelMaterial.baseColor.y = (float)baseColor.Buffer()[1];
					pbrModelMaterial.baseColor.z = (float)baseColor.Buffer()[2];
				}

				const FbxProperty propMetalness =
					FbxSurfaceMaterialUtils::GetProperty("metalness", materials);
				if (propMetalness.IsValid())
				{
					pbrModelMaterial.metalness = propMetalness.Get<float>();
				}


				const FbxProperty propSpecular =
					FbxSurfaceMaterialUtils::GetProperty("specular", materials);
				if (propSpecular.IsValid())
				{
					pbrModelMaterial.fSpecular = propSpecular.Get<float>();
				}

				const FbxProperty propSuecularRoughness =
					FbxSurfaceMaterialUtils::GetProperty("specularRoughness", materials);
				if (propSuecularRoughness.IsValid())
				{
					pbrModelMaterial.roughness = propSuecularRoughness.Get<float>();
				}*/

			}


			fbxModel->material[name]->SetMaterialData(mtl);

			const FbxProperty diffuseProperty =
				materials->FindProperty(FbxSurfaceMaterial::sDiffuse);

			if (diffuseProperty.IsValid())
			{
				const FbxFileTexture* texture =
					diffuseProperty.GetSrcObject<FbxFileTexture>();
				if (texture)
				{
					const char* filePath = texture->GetFileName();

					std::string path_str(filePath);
					std::string texName;
					ExtractFileName(path_str, nullptr, &texName);

					//読み込み処理またはModelDataクラスにパスを渡す処理をここに
					//ファイル名のみ記述されてた

					//fbxModel->material[0].SetLoadTexture(modelDirectryPath + name);
					fbxModel->pTexture[name]->LoadModelTexture(modelDirectryPath + texName);
					fbxModel->material[name]->SetTexture(fbxModel->pTexture[name].get());


					textureLoader = true;
				}
			}

		}


		//if(!textureLoader)
		//{
		//	//ライブラリで実装してるべた塗りテクスチャを持たせるようにするので、
		//	//そのうちここ変更する
		//	
		//	fbxModel->pTextures.resize(1);
		//	fbxModel->pTextures[0] = std::make_unique<Texture>();
		//	fbxModel->pTextures[0]->LoadModelTexture("Resources/Texture/WhiteTex.png");
		//}
	}
}

void FbxLoader::ParseSkin(ModelData* fbxModel, FbxMesh* fbxMesh, FbxNode* node, const std::string& name)
{


	//スキニング情報取得
	FbxSkin* fbxSkin =
		static_cast<FbxSkin*>(fbxMesh->GetDeformer(0, FbxDeformer::eSkin));


	//スキニング情報がない場合return
	if (!fbxSkin)
	{
		for (int i = 0; i < fbxModel->vertices[name].size(); i++)
		{
			//最初のボーンの影響度を100%にする(単位行列の結果を反映させるため)
			fbxModel->vertices[name][i].boneIndex[0] = 0;
			fbxModel->vertices[name][i].boneWeight[0] = 1.0f;
		}
		return;
	}

	std::vector<ModelData::FbxBone>& bones = fbxModel->fbxData.bones;

	// クラスターカウント = アーマチュア(ボーン)数
	const int clusterCount = fbxSkin->GetClusterCount();
	if (clusterCount == 0)return;
	bones.reserve(clusterCount);

	//ボーン番号とスキンウェイト
	struct WeightSet
	{
		UINT index;
		float weight;
	};
	std::vector<std::list<WeightSet>>weightLists(fbxModel->vertices[name].size());


	for (int i = 0; i < clusterCount; i++)
	{
		//ボーン情報(クラスターはSDKで定義されているボーン?)
		FbxCluster* fbxCluster = fbxSkin->GetCluster(i);

		//ボーンのノード名取得
		const char* boneName = fbxCluster->GetLink()->GetName();

		//ボーン追加

		bones.emplace_back(ModelData::FbxBone(boneName));
		ModelData::FbxBone& bone = bones.back();
		//自作ボーンとfbxのボーンとの紐付け
		bone.fbxCluster = fbxCluster;



		//初期姿勢行列の取得
		FbxAMatrix fbxMat;
		fbxCluster->GetTransformLinkMatrix(fbxMat);

		//XMMATRIXに変換
		DirectX::XMMATRIX initialPose;
		ConvertMatrixFromFbx(&initialPose, fbxMat);

		//逆行列をボーンに渡す
		bone.invInitialPose = DirectX::XMMatrixInverse(nullptr, initialPose);




		int controlPointIndicesCount = fbxCluster->GetControlPointIndicesCount();
		int* controlPointIndices = fbxCluster->GetControlPointIndices();
		double* controlPointWeight = fbxCluster->GetControlPointWeights();
		for (int j = 0; j < controlPointIndicesCount; j++)
		{
			const int vertIndex = controlPointIndices[j];
			const float weight = (float)controlPointWeight[j];

			weightLists[vertIndex].emplace_back(WeightSet{ (UINT)i,weight });
		}
	}


	auto& vertices = fbxModel->vertices;
	for (int i = 0; i < vertices[name].size(); i++)
	{
		auto& weightList = weightLists[i];
		weightList.sort
		(
			[](auto const& lhs, auto const& rhs)
			{
				return lhs.weight > rhs.weight;
			}
		);

		int weightArrayIndex = 0;
		for (auto& weightSet : weightList)
		{
			vertices[name][i].boneIndex[weightArrayIndex] = weightSet.index;
			vertices[name][i].boneWeight[weightArrayIndex] = weightSet.weight;


			if (++weightArrayIndex >= FbxVertex::MAX_BONE_INDICES)
			{
				float weight = 0.0f;

				for (int j = 1; j < FbxVertex::MAX_BONE_INDICES; j++) {
					weight += vertices[name][i].boneWeight[j];
				}

				vertices[name][i].boneWeight[0] = 1.0f - weight;
				break;

			}
		}

	}

	// ボーンの先頭ノードをを頼りに親ボーンをセット
	SetParentBone(fbxModel, fbxSkin->GetCluster(0)->GetLink(), nullptr);

}

void MelLib::FbxLoader::SetParentBone(ModelData* fbxModel, FbxNode* node, FbxNode* parentNode)
{
	std::string name = node->GetName();
	for (int i = 0; i < node->GetChildCount(); i++)
	{
		SetParentBone(fbxModel, node->GetChild(i), node);
	}

	if (!parentNode)return;

	// 送られてきたノードを元に親ボーンを設定

	// ボーンの参照を取得
	std::vector<ModelData::FbxBone>& bones = fbxModel->fbxData.bones;

	for (auto& bone : bones)
	{
		if (bone.boneName == node->GetName())
		{
			for (auto& bone2 : bones)
			{
				if (bone2.boneName == parentNode->GetName())
				{
					bone.parentBone = &bone2;
					break;
				}
			}

			break;
		}
	}


}


void FbxLoader::ConvertMatrixFromFbx(DirectX::XMMATRIX* dst, const FbxAMatrix& src)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
			dst->r[i].m128_f32[j] = (float)src.Get(i, j);
	}

}

std::string FbxLoader::ExtractFileName
(
	const std::string& path,
	std::string* directry,
	std::string* fileName
)
{
	size_t pos1;

	pos1 = path.rfind('\\');
	if (pos1 != std::string::npos)
	{
		if (fileName)
			*fileName = path.substr(pos1 + 1, path.size() - pos1 - 1);

		if (directry)
			*directry = path.substr(0, pos1 + 1);
	}
	pos1 = path.rfind('/');
	if (pos1 != std::string::npos)
	{
		if (fileName)
			*fileName = path.substr(pos1 + 1, path.size() - pos1 - 1);


		if (directry)
			*directry = path.substr(0, pos1 + 1);
	}

	return path;
}
