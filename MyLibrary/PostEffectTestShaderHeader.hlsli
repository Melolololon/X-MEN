cbuffer cbuff0 : register(b0)
{
	matrix mat;
	float4 color;
	float4 mulColor;
	float4 addColor;
	float4 subColor;
};


//cbuffer cbuff0 : register(b0)
//{
//	matrix boneMatArray[64];
//	matrix mat;
//	matrix normalMat;
//	matrix worldMat;
//	float4 mulColor;
//	float4 addColor;
//	float4 subColor;
//	float ex;
//};


struct VSOutput
{
	float4 svpos : SV_POSITION;
	float2 uv : TEXCOORD;
};