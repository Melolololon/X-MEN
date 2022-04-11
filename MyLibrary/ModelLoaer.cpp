#include "ModelLoader.h"
#include<fstream>
#include<string>
#include<sstream>
#include<DirectXMath.h>

using namespace MelLib;

ModelLoader::ModelLoader()
{
}


ModelLoader::~ModelLoader()
{
}

ModelLoader* ModelLoader::GetInstance()
{
	static ModelLoader inst;
	return &inst;
}

bool ModelLoader::LoadPmdModel(const char* path, std::vector<PMDVertex>& vertex, std::vector<unsigned short>& indices)
{
	char sig[3];
	PMDHeader pmdHeader;
	int vertNum;
	PMDVertex pmdVertex;
	int indexNum;
	FILE* model;

	auto flag = fopen_s(&model, path, "rb");
	if (flag != 0)return false;

	fread(sig, sizeof(sig), 1, model);
	fread(&pmdHeader, sizeof(pmdHeader), 1, model);
	fread(&vertNum, sizeof(vertNum), 1, model);
	vertex.resize(vertNum);
	fread(vertex.data(), vertex.size() * sizeof(PMDVertex), 1, model);
	fread(&indexNum, sizeof(indexNum), 1, model);
	indices.resize(indexNum);
	fread(indices.data(), indices.size() * sizeof(unsigned short), 1, model);

	fclose(model);

	return true;
}


bool  ModelLoader::LoadObjModel
(
	const std::string& path,
	bool loadUV,
	bool loadNormal, 
	std::vector<std::string>& objectNames,
	std::unordered_map<std::string,std::vector<FbxVertex>>& vertices,
	std::unordered_map<std::string,std::vector<USHORT>>& indices,
	std::string& materialFileName,
	std::unordered_map<std::string, std::string>& materialName,
	std::unordered_map<std::string, std::unordered_map < USHORT, std::vector<USHORT>>>& smoothNormalCalcData,
	int* loadNum,
	std::vector<Vector3>* bonePosVector,
	std::unordered_map<std::string, std::vector<int>>* boneNumVector
)
{

	//とりあえず、マテリアル読み込みは強制
#pragma region 新読み込み
	vertices.clear();
	indices.clear();
	smoothNormalCalcData.clear();


	//読み取って一時的に入れる配列
	std::vector<DirectX::XMFLOAT3>loadPosVector;
	loadPosVector.reserve(1000);
	std::vector<DirectX::XMFLOAT2>loadUvVector;
	loadUvVector.reserve(1000);
	std::vector<DirectX::XMFLOAT3>loadNormalVector;
	loadNormalVector.reserve(1000);


	//添え字
	USHORT index[3];

	//一時的に入れる変数
	DirectX::XMFLOAT3 pos = { 0,0,0 };
	DirectX::XMFLOAT3 normal = { 0,0,0 };
	DirectX::XMFLOAT2 uv = { 0,0 };
	std::string mtlNama = "";

	//読み取ったテキストを格納する文字列
	std::string objText = "";


	//ファイルストリーム
	std::ifstream obj;
	obj.open(path);
	if (!obj)
	{
		OutputDebugStringA(path.data());
		OutputDebugString(L"の読み込みに失敗しました。.objファイルが見つかりません。\n");
		return false;
	}


	//マテリアル読み込みでtrueにし、座標読み込み時にtrueだったら配列clear
	bool loadPreparation = false;

	//読み込んだ.obj内のモデル数
	int loadObjectNum = 0;

	//ポリゴンの枚数
	int polygonCount = 0;

	//マテリアルを読み込んだかどうか
	bool loadMtlFile = false;

	//スムースシェーディング用データ格納用
	std::unordered_map<std::string,std::unordered_map<int, DirectX::XMFLOAT3>>smoothVertexPos;

	//頂点読み込み数
	int vertexLoadCount = 0;


	//ボーン用
	DirectX::XMFLOAT3 bonePos = { 0,0,0 };
	int boneObjectNum = 0;//何個目のオブジェクトのデータかを入れる変数
	int boneNum = 0;//ボーンの番号
	bool loadBoneData = false;
	std::unordered_map<std::string, std::vector<int>>temporaryBoneNumVec;//temporary は　仮

	std::string objectName = "";
	while (std::getline(obj, objText))
	{
		std::istringstream lineStream(objText);

		//先頭文字取得
		std::string topStr = "";
		lineStream >> topStr;

		// オブジェクト名取得
		if(topStr.find("o") != std::string::npos)
		{
			lineStream >> objectName;
			objectNames.push_back(objectName);
		}

#pragma region 頂点データ取得


		if (topStr.find("vt") != std::string::npos &&
			loadUV)
		{
			lineStream >> uv.x;
			lineStream >> uv.y;
			loadUvVector.push_back(uv);
		}
		else
			if (topStr.find("vn") != std::string::npos)
			{
				lineStream >> normal.x;
				lineStream >> normal.y;
				lineStream >> normal.z;
				loadNormalVector.push_back(normal);
			}
			else
				if (topStr.find("v") != std::string::npos)
				{
					if (!loadMtlFile)
					{
						OutputDebugStringA(path.data());
						OutputDebugString(L"の読み込みに失敗しました。.mtlファイルが.objに記述されていません。\n");
						return false;
					}

					if (!loadPreparation)
					{

						loadPreparation = true;

						//クリア
						//次のモデルのインデックスが前回の途中から始まるからリセットいらない
					/*	loadPosVector.clear();
						loadUvVector.clear();
						loadNormalVector.clear();*/

						//スムーズシェーディング用配列修正
						smoothNormalCalcData.reserve(smoothNormalCalcData.size() + 1);
						smoothVertexPos.reserve(smoothVertexPos.size() + 1);


						//読み込み数カウントインクリメント
						loadObjectNum++;

						//カウント初期化
						vertexLoadCount = 0;
						polygonCount = 0;

						//参照した配列のメモリ確保
						vertices.reserve(vertices.size() + 1);
						vertices[objectName].reserve(1000);
						indices.reserve(indices.size() + 1);
						indices[objectName].reserve(1000);

					}

					lineStream >> pos.x;
					lineStream >> pos.y;
					lineStream >> pos.z;
					loadPosVector.push_back(pos);


					std::vector<USHORT>u;
					smoothNormalCalcData[objectName].emplace(vertexLoadCount, u);
					smoothVertexPos[objectName].emplace(vertexLoadCount, pos);
					vertexLoadCount++;
				}

#pragma endregion

#pragma region マテリアル関係

		//マテリアルのファイル名取得
		if (topStr.find("mtllib") != std::string::npos)
		{
			lineStream >> materialFileName;
			loadMtlFile = true;
		}
		//マテリアル名取得
		if (topStr.find("usemtl") != std::string::npos)
		{
			lineStream >> mtlNama;
			materialName.emplace(objectName,mtlNama);


			loadPreparation = false;

		}

#pragma endregion

#pragma region 割り当て

		if (topStr.find("f") != std::string::npos)
		{
			//アルゴリズムメモ
			//文字列の/の数で、三角形とか判断する?

			//メモここまで


			//スラッシュ入れる用
			char sluch;

			//頂点。これに一時的に入れてから、配列に入れる
			FbxVertex vertex;

			//三角形ポリゴンを読み込むので3
			int polygonValue = 3;

			for (int i = 0; i < polygonValue; i++)
			{
				//インデックス読み込み
				lineStream >> index[0];
				lineStream >> sluch;
				if (loadUV)
					lineStream >> index[1];
				lineStream >> sluch;
				lineStream >> index[2];

				//参照してる配列に代入
				vertex.pos = loadPosVector[index[0] - 1];
				vertex.normal = loadNormalVector[index[2] - 1];
				if (loadUV)
					vertex.uv = loadUvVector[index[1] - 1];
				else
					vertex.uv = { 0,0 };

				//格納
				vertices[objectName].push_back(vertex);
				indices[objectName].push_back(polygonCount);
				polygonCount++;
			}


		}
#pragma endregion


		// オブジェクト名で管理し始めたが、objのボーン情報が番号で出力されて読み込めないため、コメントアウト
#pragma region ボーン読み込み

		////読み込み準備
		//if (objText.find("BoneData") != std::string::npos)
		//{
		//	loadBoneData = true;

		//	if (bonePosVector)bonePosVector->reserve(99);
		//	if (boneNumVector)temporaryBoneNumVec.reserve(vertices.size());

		//	int loopCount = 0;
		//	for (auto& num : temporaryBoneNumVec)
		//	{
		//		num[objectName].reserve(vertices[objectName].size());
		//		loopCount++;
		//	}
		//}


		////bpあったらボーン座標追加
		//if (objText.find("bp") != std::string::npos && bonePosVector)
		//{
		//	lineStream >> bonePos.x;
		//	lineStream >> bonePos.y;
		//	lineStream >> bonePos.z;
		//	bonePosVector->push_back(bonePos);
		//}

		//if (objText.find("bd") != std::string::npos && boneNumVector)
		//{

		//	//何個目のオブジェクトのデータか確認
		//	lineStream >> boneObjectNum;

		//	//番号取得
		//	lineStream >> boneNum;

		//	//仮配列にボーン番号追加
		//	temporaryBoneNumVec[boneObjectNum].push_back(boneNum);

		//}

#pragma endregion

	}

	if (loadNum)
		*loadNum = loadObjectNum;

	//仮配列から移動
	if (boneNumVector)
		*boneNumVector = temporaryBoneNumVec;


	obj.close();

#pragma region スムースシェーディング用データ格納

	//サイズをオブジェクトを読み込んだ回数分にしておく
	//smoothNormalCalcData.resize(smoothVertexPos.size());
	int v1Count = 0;

	for(auto& v : vertices)
	{
		std::string name = v.first;

		for (auto& v1 : vertices[name])
		{
			auto itr = smoothVertexPos[name].begin();
			int itrNum = 0;
			//smoothVertexPosと比較して、smoothNormalCalcDataに入れていく
			for (; itr != smoothVertexPos[name].end(); ++itr)
			{

				if (v1.pos.x == itr->second.x &&
					v1.pos.y == itr->second.y &&
					v1.pos.z == itr->second.z)
				{
					smoothNormalCalcData[name][itrNum].push_back(v1Count);
				}
				itrNum++;
			}

			v1Count++;
		}

		v1Count = 0;
	}

#pragma endregion



	return true;
#pragma endregion


}

bool ModelLoader::LoadObjMaterial
(
	const std::string& materialDirectoryPath,
	const std::string& materialFileName,
	const std::vector<std::string>& objectNames,
	std::unordered_map<std::string, std::string>& texPath,
	std::unordered_map<std::string, ADSAMaterialData>& materials,
	int* loadCount
)
{
	//読み込んだ回数
	int loadNum = 0;

	std::ifstream file;
	std::string fullPath = materialDirectoryPath + materialFileName;
	file.open(fullPath);
	if (file.fail())
	{
		OutputDebugStringA(fullPath.data());
		OutputDebugString(L"の読み込みに失敗しました。.mtlファイルが見つかりません。\n");
		return false;
	}

	std::string line;



	//int materialArrayNum = 0;//マテリアル情報を書き込む場所の番号
	//std::string materialNameKari;//マテリアル名を一時的に入れるstring

	std::string objectName;
	while (std::getline(file, line))
	{
		std::istringstream lineStream(line);
		std::string materialData;
		std::getline(lineStream, materialData, ' ');

		if (materialData[0] == '\t')materialData.erase(materialData.begin());
		if (materialData == "newmtl")
		{
			loadNum++;
			texPath.reserve(loadNum);
			materials.reserve(loadNum);

			objectName = objectNames[loadNum - 1];

			//名前取得
			//lineStream >> materials[loadNum - 1].materialName;

			//for (UINT i = 0; i < materialName.size(); i++) 
			//{
			//	//この関数で取得した名前と以前に取得した名前が一致したら、i番目にマテリアル情報を書き込む
			//	//こうすることで、objとmtlでのマテリアルの順番が違くてもちゃんと読み込める
			//	//objで最初に2、後に１  mtlで最初に1、後に2が来ても
			//	//2個目のマテリアルを1個目のオブジェクトに、1個目を2個目にセットできる
			//	materialArrayNum = i;
			//	if (materialName[i] == materialNameKari)
			//	{
			//		material[i].materialName = materialName[i];
			//		break;
			//	}
			//}

		}
		if (materialData == "Ka") //アンビエント
		{
			lineStream >> materials[objectName].ambient.v1;
			lineStream >> materials[objectName].ambient.v2;
			lineStream >> materials[objectName].ambient.v3;
			
		}
		if (materialData == "Kd")
		{
			lineStream >> materials[objectName].diffuse.v1;
			lineStream >> materials[objectName].diffuse.v2;
			lineStream >> materials[objectName].diffuse.v3;
			
		}
		if (materialData == "Ks")
		{
			lineStream >> materials[objectName].specular.v1;
			lineStream >> materials[objectName].specular.v2;
			lineStream >> materials[objectName].specular.v3;
			
		}
		if (materialData == "map_Kd")
		{
			lineStream >> texPath[objectName];
			
		}
	}
	file.close();

	*loadCount = loadNum;

	
	return true;
}
