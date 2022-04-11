cbuffer cbuff0 : register(b0)
{
	float4 lightColor;
	float4 light;
	float4 cameraPos;
	matrix lightMat;
	matrix mat;
	matrix normalMat;
	matrix worldMat;
	float4 mulColor;
	float4 addColor;
	float4 subColor;
	float ex;
};

cbuffer cbuff2 : register(b2)
{
	float3 m_ambient : packoffset(c0);
	float3 m_diffuse : packoffset(c1);
	float3 m_specular : packoffset(c2);
	float m_alpha : packoffset(c2.w);
};

cbuffer cbuff3 : register(b3)
{
	matrix matSkinning[64];
}

struct VSOutput
{
	float4 worldPos:POSITION;
	float4 svpos : SV_POSITION;
	float3 normal:NORMAL;
	float2 uv : TEXCOORD;
};

struct GSOutput
{
	float4 worldPos:POSITION;
	float4 svpos : SV_POSITION;
	float3 normal : NORMAL;
	float2 uv : TEXCOORD;
};
