//ModelDataないとヒープ用意できないから、
//マテリアルとかのコピーコンストラクタ作ったほうがいいかも





//#pragma once
//#include<vector>
//#include<d3d12.h>
//#include<d3dx12.h>
//
//#include"DirectXStruct.h"
//
//namespace MelLib 
//{
//	//モデルの頂点、インデックスをまとめたクラス
//	class ModelVertexData
//	{
//	private:
//		//頂点
//		std::vector<std::vector<FbxVertex>>vertices;
//		std::vector<VertexBufferSet> vertexBufferSet;
//
//		//インデックス
//		std::vector <IndexBufferSet> indexBufferSet;
//		std::vector<std::vector<USHORT>>indices;
//
//
//		void MapVertices(const std::vector<FbxVertex>& vertices, const int bufferNum);
//		void MapVertices(void** data, const int bufferNum);
//		void UnmapVertices(const int bufferNum);
//
//		void MapIndices(const std::vector<USHORT>& indices, const int bufferNum);
//		void UnmapIndices(const int bufferNum);
//	public:
//		ModelVertexData(const std::vector<std::vector<FbxVertex>>& vertices, const std::vector<std::vector<USHORT>>& indices);
//
//		std::vector<std::vector<FbxVertex>> GetVertices()const { return vertices; }
//		std::vector<std::vector<USHORT>> GetIndices()const { return indices; }
//
//
//	};
//}
//
