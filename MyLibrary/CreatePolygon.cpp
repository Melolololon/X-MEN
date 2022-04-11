//#include "CreatePolygon.h"
//
//
//CreatePolygon::CreatePolygon() 
//{
//}
//CreatePolygon::~CreatePolygon() {}
//
//CreatePolygon* CreatePolygon::GetInstance()
//{
//	static CreatePolygon c;
//	return &c;
//}
//
//void CreatePolygon::Initialize(const int& winWidth, const int& winHeight)
//{
//	windowWidth = winWidth;
//	windowHeight = winHeight;
//}
//
//std::vector<Vertex> CreatePolygon::SetPolygonVertex(float r, int vertexNum, XMFLOAT2 tyusin)
//{
//	std::vector<Vertex> vertices;
//
//
//	for (int i = 0; i < vertexNum; i++)
//	{
//		XMFLOAT3 vertex;
//		Vertex vertData;
//
//		vertex.x = r * sin(DirectX::XM_2PI / vertexNum * i);
//		vertex.y = r * cos(DirectX::XM_2PI / vertexNum * i);
//		vertex.z = 0;
//
//		//アスペクト比変更
//		//vertex.y *= ((float)windowWidth / (float)windowHeight);
//
//		//中心からどのくらい離すか
//		vertex.x += tyusin.x;
//		vertex.y += tyusin.y;
//
//		vertData.pos = vertex;
//		vertData.uv = { 0,0 };
//
//		vertices.push_back(vertData);
//	}
//
//	Vertex vertData;
//	XMFLOAT3 vert;
//	vert.x = tyusin.x;
//	vert.y = tyusin.y;
//	vert.z = 0;
//
//	vertData.pos = vert;
//	vertData.uv = { 0,0 };
//
//	vertices.push_back(vertData);
//
//	return vertices;
//}
//
//std::vector<unsigned short> CreatePolygon::SetPolygonIndex(int vertexNum)
//{
//	std::vector<unsigned short> indices;
//
//	for (int i = 0; i < vertexNum; i++)
//	{
//		if (i != vertexNum - 1)
//		{
//			indices.push_back(vertexNum);
//			indices.push_back(i + 1);
//			indices.push_back(i);
//		}
//		else
//		{
//			indices.push_back(vertexNum);
//			indices.push_back(0);
//			indices.push_back(i);
//		}
//	}
//
//	return indices;
//}
//
//std::vector<Vertex> CreatePolygon::GetTriangularPyramidVertex(float r, int vertexNum, XMFLOAT3 tyusin, float up)
//{
//	std::vector<Vertex> vertices;
//
//	for (int i = 0; i < vertexNum; i++)
//	{
//		XMFLOAT3 vertex;
//		Vertex vertData;
//
//		vertex.x = r * sin(DirectX::XM_2PI / vertexNum * i);
//		vertex.y = 0;
//		vertex.z = r * cos(DirectX::XM_2PI / vertexNum * i);
//
//		//アスペクト比変更
//		//vertex.y *= ((float)windowWidth / (float)windowHeight);
//
//		//中心からどのくらい離すか
//		vertex.x += tyusin.x;
//		vertex.z += tyusin.z;
//		vertex.y = tyusin.y;
//
//		vertData.pos = vertex;
//		vertData.uv = { 0,0 };
//
//		vertices.push_back(vertData);
//	}
//
//	Vertex vertData;
//	XMFLOAT3 vert;
//	vert.x = tyusin.x;
//	vert.y = tyusin.y;
//	vert.z = tyusin.z;
//
//	vertData.pos = vert;
//	vertices.push_back(vertData);
//
//
//	vert.x = tyusin.x;
//	vert.y = up;
//	vert.z = tyusin.z;
//
//	vertData.pos = vert;
//	vertices.push_back(vertData);
//
//
//
//	return vertices;
//}
//
//std::vector<unsigned short> CreatePolygon::SetTriangularPyramidIndex(int vertexNum)
//{
//	std::vector<unsigned short> indices;
//
//	for (int i = 0; i < vertexNum; i++)
//	{
//		if (i != vertexNum - 1)
//		{
//			indices.push_back(vertexNum);
//			indices.push_back(i + 1);
//			indices.push_back(i);
//		}
//		else
//		{
//			indices.push_back(vertexNum);
//			indices.push_back(0);
//			indices.push_back(i);
//		}
//	}
//
//	for (int i = 0; i < vertexNum; i++) 
//	{
//		if (i != vertexNum - 1)
//		{
//			indices.push_back(i);
//			indices.push_back(i + 1);
//			indices.push_back(vertexNum + 1);
//		}
//		else
//		{
//			indices.push_back(i);
//			indices.push_back(0);
//			indices.push_back(vertexNum + 1);
//		}
//	}
//
//
//
//	return indices;
//}
//
//
//std::vector<Vertex> CreatePolygon::SetPolygonalPrismVertex(float r, int vertexNum, XMFLOAT3 tyusin, float up)
//{
//	std::vector<Vertex> vertices;
//
//
//	for (int i = 0; i < vertexNum; i++)
//	{
//		XMFLOAT3 vertex;
//		Vertex vertData;
//
//		vertex.x = r * sin(DirectX::XM_2PI / vertexNum * i);
//		vertex.y = 0;
//		vertex.z = r * cos(DirectX::XM_2PI / vertexNum * i);
//
//		//アスペクト比変更
//		//vertex.y *= ((float)windowWidth / (float)windowHeight);
//
//		//中心からどのくらい離すか
//		vertex.x += tyusin.x;
//		vertex.z += tyusin.z;
//		vertex.y = tyusin.y;
//
//		vertData.pos = vertex;
//		vertData.uv = { 0,0 };
//
//		vertices.push_back(vertData);
//	}
//
//
//	for (int i = 0; i < vertexNum; i++)
//	{
//		XMFLOAT3 vertex;
//		Vertex vertData;
//
//		vertex.x = r * sin(DirectX::XM_2PI / vertexNum * i);
//		vertex.y = up;
//		vertex.z = r * cos(DirectX::XM_2PI / vertexNum * i);
//
//		//アスペクト比変更
//		//vertex.y *= ((float)windowWidth / (float)windowHeight);
//
//		//中心からどのくらい離すか
//		vertex.x += tyusin.x;
//		vertex.z += tyusin.z;
//		vertex.y = tyusin.y;
//
//		vertData.pos = vertex;
//		vertData.uv = { 0,0 };
//
//		vertices.push_back(vertData);
//	}
//
//	return vertices;
//}
//
//std::vector<unsigned short> CreatePolygon::SetPolygonalPrismIndex(int vertexNum) 
//{
//	std::vector<unsigned short> indices;
//
//	for (int i = 0; i < vertexNum; i++)
//	{
//		if (i != vertexNum - 1)
//		{
//			indices.push_back(vertexNum);
//			indices.push_back(i + 1);
//			indices.push_back(i);
//		}
//		else
//		{
//			indices.push_back(vertexNum);
//			indices.push_back(0);
//			indices.push_back(i);
//		}
//	}
//
//	for (int i = vertexNum + 1; i < vertexNum * 2; i++)
//	{
//		if (i != vertexNum - 1)
//		{
//			indices.push_back(vertexNum * 2);
//			indices.push_back(i + 1);
//			indices.push_back(i);
//		}
//		else
//		{
//			indices.push_back(vertexNum * 2);
//			indices.push_back(0);
//			indices.push_back(i);
//		}
//	}
//
//
//	for (int i = 0; i < vertexNum; i++)
//	{
//		if (i != vertexNum - 1) 
//		{
//			indices.push_back(i + 1);
//			indices.push_back(i + vertexNum + 1);
//			indices.push_back(i);
//
//			indices.push_back(i + vertexNum + 1);
//			indices.push_back(i + 1);
//			indices.push_back(i + vertexNum + 2);
//		}
//		else 
//		{
//			indices.push_back(0);
//			indices.push_back(i + vertexNum + 1);
//			indices.push_back(i);
//
//			indices.push_back(i + vertexNum + 1);
//			indices.push_back(0);
//			indices.push_back(i + 2);
//		}
//	}
//
//	return indices;
//}
//
//
//std::vector<Vertex> CreatePolygon::SetSlushPolygonVertex(XMFLOAT2 tyusin, XMFLOAT2 pos, int type)
//{
//	std::vector<Vertex> vertices;
//	Vertex vertData;
//
//	vertData.pos = { pos.x,pos.y,0 };
//	vertices.push_back(vertData);
//	vertData.pos = { -pos.x,pos.y,0 };
//	vertices.push_back(vertData);
//	vertData.pos = { pos.x,-pos.y,0 };
//	vertices.push_back(vertData);
//	vertData.pos = { -pos.x,-pos.y,0 };
//	vertices.push_back(vertData);
//
//	if (type == 0) 
//	{
//		vertData.pos = { pos.x + 0.05f,0,0 };
//		vertices.push_back(vertData);
//		vertData.pos = { -pos.x - 0.05f ,0,0 };
//		vertices.push_back(vertData);
//	}
//	else 
//	{
//		vertData.pos = { 0,pos.y + 0.05f ,0 };
//		vertices.push_back(vertData);
//		vertData.pos = { 0,-pos.y - 0.05f ,0 };
//		vertices.push_back(vertData);
//	}
//
//	for (int i = 0; i < (int)vertices.size(); i++) 
//	{
//		vertices[i].pos.x += tyusin.x;
//		vertices[i].pos.y += tyusin.y;
//	}
//
//	//for (int i = 0; i < (int)vertices.size(); i++)
//	{
//		//vertices[i].pos.y *= ((float)windowWidth / (float)windowHeight);
//	}
//	return vertices;
//}
//
//std::vector<unsigned short> CreatePolygon::SetSlushPolygonIndex(int type)
//{
//	std::vector<unsigned short> indices;
//
//	indices.push_back(0);
//	indices.push_back(1);
//	indices.push_back(2);
//	indices.push_back(1);
//	indices.push_back(2);
//	indices.push_back(3);
//
//	if (type == 0) 
//	{
//		indices.push_back(0);
//		indices.push_back(4);
//		indices.push_back(2);
//		indices.push_back(1);
//		indices.push_back(5);
//		indices.push_back(3);
//	}
//	else 
//	{
//		indices.push_back(1);
//		indices.push_back(4);
//		indices.push_back(0);
//		indices.push_back(3);
//		indices.push_back(5);
//		indices.push_back(2);
//	}
//
//	return indices;
//}
//
//std::vector<Vertex> CreatePolygon::SetBoxPolygonVertex(XMFLOAT2 startPos, XMFLOAT2 endPos)
//{
//	std::vector<Vertex> vertices;
//	Vertex vertData;
//
//	vertData.pos = { startPos.x ,startPos.y + endPos.y ,0};
//	vertices.push_back(vertData);
//	vertData.pos = { startPos.x,startPos.y ,0 };
//	vertices.push_back(vertData);
//	vertData.pos = { startPos.x + endPos.x,startPos.y + endPos.y ,0 };
//	vertices.push_back(vertData);
//	vertData.pos = { startPos.x + endPos.x,startPos.y ,0 };
//	vertices.push_back(vertData);
//
//	//for (int i = 0; i < 4; i++) 
//	{
//		//vertices[i].pos.y *= ((float)windowWidth / (float)windowHeight);
//	}
//
//	return vertices;
//}
//
//std::vector<unsigned short> CreatePolygon::SetBoxPolygonIndex() 
//{
//	std::vector<unsigned short> indices = { 0,1,2,2,1,3 };
//
//	return indices;
//}
//
//
//std::vector<Vertex> CreatePolygon::SetBoardPolygonVertex(XMFLOAT3 pos1, XMFLOAT3 pos2, XMFLOAT3 pos3, XMFLOAT3 pos4)
//{
//	std::vector<Vertex> vertices;
//	Vertex vertData;
//
//	vertData.pos = { pos1.x,pos1.y,pos1.z };
//	vertData.uv = { 0.0f,1.0f };
//	vertices.push_back(vertData);
//	vertData.pos = { pos2.x,pos2.y,pos2.z };
//	vertData.uv = { 0.0f,0.0f };
//	vertices.push_back(vertData);
//	vertData.pos = { pos3.x,pos3.y,pos3.z };
//	vertData.uv = { 1.0f,1.0f };
//	vertices.push_back(vertData);
//	vertData.pos = { pos4.x,pos4.y,pos4.z };
//	vertData.uv = { 1.0f,0.0f };
//	vertices.push_back(vertData);
//
//	//for (int i = 0; i < 4; i++) 
//	{
//		//vertices[i].pos.y *= ((float)windowWidth / (float)windowHeight);
//	}
//
//	return vertices;
//}
//
//std::vector<unsigned short> CreatePolygon::SetBoardPolygonIndex()
//{
//	std::vector<unsigned short> indices = { 0,1,2,2,1,3 };
//
//	return indices;
//}
//
//
//std::vector<Vertex> CreatePolygon::Get3DBox(DirectX::XMFLOAT3 size)
//{
//	float x = size.x;
//	float y = size.y;
//	float z = size.z;
//	x /= 2;
//	y /= 2;
//	z /= 2;
//
//	std::vector<Vertex> vertices;
//	Vertex vertData;
//
//	vertData.pos = { -x,-y,-z };
//	vertData.uv = { 0.0f,1.0f };
//	vertices.push_back(vertData);
//	vertData.pos = { -x,y,-z };
//	vertData.uv = { 0.0f,0.0f };
//	vertices.push_back(vertData);
//	vertData.pos = { x,-y,-z };
//	vertData.uv = { 1.0f,1.0f };
//	vertices.push_back(vertData);
//	vertData.pos = { x,y,-z };
//	vertData.uv = { 1.0f,0.0f };
//	vertices.push_back(vertData);
//
//
//	
//	//正面の上
//	//vertData.pos = { -x, y,-z };
//	//vertices.push_back(vertData);
//	vertData.pos = { -x, y,z };
//	vertData.uv = { 0.0f,1.0f };
//	vertices.push_back(vertData);
//	//vertData.pos = { x,y,-z };
//	//vertices.push_back(vertData);
//	vertData.pos = { x,y,z };
//	vertData.uv = { 1.0f,1.0f };
//	vertices.push_back(vertData);
//
//	//正面の裏
//	//vertData.pos = { -x, y,z };
//	//vertices.push_back(vertData);
//	vertData.pos = { -x, -y,z };
//	vertData.uv = { 0.0f,0.0f };
//	vertices.push_back(vertData);
//	//vertData.pos = { x,y,z };
//	//vertices.push_back(vertData);
//	vertData.pos = { x,-y,z };
//	vertData.uv = { 1.0f,0.0f };
//	vertices.push_back(vertData);
//
//	////正面の下
//	////vertData.pos = { -x, -y,z };
//	////vertices.push_back(vertData);
//	//vertData.pos = { -x, -y,-z };
//	//vertData.uv = { 0.0f,0.0f };
//	//vertices.push_back(vertData);
//	////vertData.pos = { x,-y,z };
//	////vertices.push_back(vertData);
//	//vertData.pos = { x,-y,-z };
//	//vertData.uv = { 1.0f,0.0f };
//	//vertices.push_back(vertData);
//
//	////正面の右
//	////vertData.pos = { x,y,-z };
//	////vertices.push_back(vertData);
//	//vertData.pos = { x,y,z };
//	//vertData.uv = { 0.0f,0.0f };
//	//vertices.push_back(vertData);
//	////vertData.pos = { x,-y,-z };
//	////vertices.push_back(vertData);
//	//vertData.pos = { x,-y,z };
//	//vertData.uv = { 0.0f,0.0f };
//	//vertices.push_back(vertData);
//
//	////正面の左
//	////vertData.pos = { -x,-y,-z };
//	////vertices.push_back(vertData);
//	//vertData.pos = { -x,-y,z };
//	//vertData.uv = { 0.0f,0.0f };
//	//vertices.push_back(vertData);
//	////vertData.pos = { -x,y,-z };
//	////vertices.push_back(vertData);
//	//vertData.pos = { -x,y,z };
//	//vertData.uv = { 0.0f,0.0f };
//	//vertices.push_back(vertData);
//
//	return vertices;
//}
//
//std::vector<unsigned short>  CreatePolygon::Get3DBoxIndex()
//{
//	std::vector<unsigned short> indices = 
//	{
//		0,1,2,2,1,3,
//		1,4,3,3,4,5,
//		4,6,5,5,6,7,
//		6,0,7,7,0,2,
//		3,5,2,2,5,7,
//		0,6,1,1,6,4
//	};
//
//	return indices;
//}
//
//std::vector<Vertex> CreatePolygon::GetVertexMany3DBox(DirectX::XMFLOAT3 size)
//{
//	float x = size.x;
//	float y = size.y;
//	float z = size.z;
//	x /= 2;
//	y /= 2;
//	z /= 2;
//
//	std::vector<Vertex> vertices;
//	Vertex vertData;
//
//	vertData.pos = { -x,-y,-z };
//	vertData.uv = { 0.0f,1.0f };
//	vertices.push_back(vertData);
//	vertData.pos = { -x,y,-z };
//	vertData.uv = { 0.0f,0.0f };
//	vertices.push_back(vertData);
//	vertData.pos = { x,-y,-z };
//	vertData.uv = { 1.0f,1.0f };
//	vertices.push_back(vertData);
//	vertData.pos = { x,y,-z };
//	vertData.uv = { 1.0f,0.0f };
//	vertices.push_back(vertData);
//
//
//
//	//正面の上
//	vertData.pos = { -x, y,-z };
//	vertData.uv = { 0.0f,1.0f };
//	vertices.push_back(vertData);
//	vertData.pos = { -x, y,z };
//	vertData.uv = { 0.0f,0.0f };
//	vertices.push_back(vertData);
//	vertData.pos = { x,y,-z };
//	vertData.uv = { 1.0f,1.0f };
//	vertices.push_back(vertData);
//	vertData.pos = { x,y,z };
//	vertData.uv = { 1.0f,0.0f };
//	vertices.push_back(vertData);//ここまでuv設定した
//
//	//正面の裏
//	vertData.pos = { -x, y,z };
//	vertData.uv = { 0.0f,1.0f };
//	vertices.push_back(vertData);
//	vertData.pos = { -x, -y,z };
//	vertData.uv = { 0.0f,0.0f };
//	vertices.push_back(vertData);
//	vertData.pos = { x,y,z };
//	vertData.uv = { 1.0f,1.0f };
//	vertices.push_back(vertData);
//	vertData.pos = { x,-y,z };
//	vertData.uv = { 1.0f,0.0f };
//	vertices.push_back(vertData);
//
//	//正面の下
//	vertData.pos = { -x, -y,z };
//	vertData.uv = { 0.0f,1.0f };
//	vertices.push_back(vertData);
//	vertData.pos = { -x, -y,-z };
//	vertData.uv = { 0.0f,0.0f };
//	vertices.push_back(vertData);
//	vertData.pos = { x,-y,z };
//	vertData.uv = { 1.0f,1.0f };
//	vertices.push_back(vertData);
//	vertData.pos = { x,-y,-z };
//	vertData.uv = { 1.0f,0.0f };
//	vertices.push_back(vertData);
//
//	//正面の右
//	vertData.pos = { x,y,-z };
//	vertData.uv = { 0.0f,0.0f };
//	vertices.push_back(vertData);
//	vertData.pos = { x,y,z };
//	vertData.uv = { 1.0f,0.0f };
//	vertices.push_back(vertData);
//	vertData.pos = { x,-y,-z };
//	vertData.uv = { 0.0f,1.0f };
//	vertices.push_back(vertData);
//	vertData.pos = { x,-y,z };	
//	vertData.uv = { 1.0f,1.0f };
//	vertices.push_back(vertData);
//
//	//正面の左
//	vertData.pos = {-x,-y,-z };
//	vertData.uv = { 1.0f,1.0f };
//	vertices.push_back(vertData);
//	vertData.pos = {-x,-y,z };
//	vertData.uv = { 0.0f,1.0f };
//	vertices.push_back(vertData);
//	vertData.pos = { -x,y,-z };
//	vertData.uv = { 1.0f,0.0f };
//	vertices.push_back(vertData);
//	vertData.pos = { -x,y,z };
//	vertData.uv = { 0.0f,0.0f };
//	vertices.push_back(vertData);
//
//	return vertices;
//}
//
//std::vector<unsigned short> CreatePolygon::GetVertexMany3DBoxIndex()
//{
//	std::vector<unsigned short> indices =
//	{
//		0,1,2,2,1,3,
//		4,5,6,6,5,7,
//		8,9,10,10,9,11,
//		12,13,14,14,13,15,
//		16,17,18,18,17,19,
//		20,21,22,22,21,23,
//	};
//
//	return indices;
//}
//
