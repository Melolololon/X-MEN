#include"FbxMultiShaderHeader.hlsli"

struct SkinOutput
{
	float4 pos;
	float3 normal;
};

struct VSInput
{
	float4 pos : POSITION;
	float2 uv : TEXCOORD;
	float3 normal : NORMAL;
	uint4 boneIndices : BONEINDICES;
	float4 boneWeights : BONEWEIGHTS;

};


SkinOutput ComputeSkin(VSInput input)
{
	SkinOutput output = (SkinOutput)0;

	uint iBone;
	float weight;
	matrix m;

	//ボーン0
	iBone = input.boneIndices.x;
	weight = input.boneWeights.x;
	m = matSkinning[iBone];
	output.pos += weight * mul(m, input.pos);
	output.normal += weight * mul((float3x3)m, input.normal);

	//ボーン1
	iBone = input.boneIndices.y;
	weight = input.boneWeights.y;
	m = matSkinning[iBone];
	output.pos += weight * mul(m, input.pos);
	output.normal += weight * mul((float3x3)m, input.normal);

	//ボーン2
	iBone = input.boneIndices.z;
	weight = input.boneWeights.z;
	m = matSkinning[iBone];
	output.pos += weight * mul(m, input.pos);
	output.normal += weight * mul((float3x3)m, input.normal);

	//ボーン3
	iBone = input.boneIndices.w;
	weight = input.boneWeights.w;
	m = matSkinning[iBone];
	output.pos += weight * mul(m, input.pos);
	output.normal += weight * mul((float3x3)m, input.normal);


	return output;
}


VSOutput main(VSInput input)
{
	SkinOutput skinned = ComputeSkin(input);
	float4 wnormal = normalize(mul(normalMat, float4(skinned.normal, 0)));

	VSOutput output;

	//ここでmat掛けたせいで、ワールド行列が再度かけられている
	//ボーンが先なので、問題ないはず
	output.svpos = mul(mat, skinned.pos);
	output.normal = wnormal.xyz;

	output.worldPos = mul(worldMat, skinned.pos);
	output.uv = input.uv;

	return output;
}
