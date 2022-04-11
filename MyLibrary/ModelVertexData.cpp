//#include "ModelVertexData.h"
//
//#include"CreateBuffer.h"
//
//MelLib::ModelVertexData::ModelVertexData(const std::vector<std::vector<FbxVertex>>& vertices, const std::vector<std::vector<USHORT>>& indices)
//{
//	this->vertices = vertices;
//	this->indices = indices;
//
//
//	size_t vertSize = vertices.size();
//
//	//頂点バッファ作成
//	vertexBufferSet.resize(vertSize);
//
//	for (int i = 0; i < vertSize; i++)
//	{
//		CreateBuffer::GetInstance()->CreateVertexBuffer
//		(
//			CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
//			sizeof(FbxVertex),
//			vertices[i].size(),
//			vertexBufferSet[i]
//		);
//	}
//
//	//インデックスバッファ作成
//	indexBufferSet.resize(vertSize);
//	for (int i = 0; i < vertSize; i++)
//	{
//		CreateBuffer::GetInstance()->CreateIndexBuffer
//		(
//			CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
//			indices[i],
//			indexBufferSet[i]
//		);
//	}
//
//	//Map
//	for (int i = 0; i < vertSize; i++) 
//	{
//		std::vector<FbxVertex>vert;
//		MapVertices(vert, i);
//		for (int j = 0, size = vertices[i].size(); j < size; j++)
//		{
//			vert[j] = vertices[i][j];
//		}
//		UnmapVertices(i);
//
//		std::vector<USHORT>ind;
//		MapIndices(ind,i);
//		for (int j = 0, size = indices[i].size(); j < size; j++)
//		{
//			ind[j] = indices[i][j];
//		}
//		UnmapIndices(i);
//
//	}
//
//}
//
//void MelLib::ModelVertexData::MapVertices(const std::vector<FbxVertex>& vertices, const int bufferNum)
//{
//	MapVertices((void**)&vertices, bufferNum);
//}
//
//void MelLib::ModelVertexData::MapVertices(void** data, const int bufferNum)
//{
//	vertexBufferSet[bufferNum].vertexBuffer->Map(0, nullptr, data);
//}
//
//void MelLib::ModelVertexData::UnmapVertices(const int bufferNum)
//{
//	vertexBufferSet[bufferNum].vertexBuffer->Unmap(0, nullptr);
//}
//
//void MelLib::ModelVertexData::MapIndices(const std::vector<USHORT>& indices, const int bufferNum)
//{
//	indexBufferSet[bufferNum].indexBuffer->Map(0, nullptr, (void**)&indices);
//}
//
//void MelLib::ModelVertexData::UnmapIndices(const int bufferNum)
//{
//	indexBufferSet[bufferNum].indexBuffer->Unmap(0, nullptr);
//}
