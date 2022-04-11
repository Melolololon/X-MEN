#include"FbxShaderHeader.hlsli"

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


	//output.pos = input.pos;
	//output.normal = input.normal;
	//return output;

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
	float4 wnormal = normalize(mul(worldMat, float4(skinned.normal, 0)));

	VSOutput output;



	output.svpos = mul(worldMat, skinned.pos);
	output.worldPos = output.svpos;

	/*static const float3 PLANE_POS = float3(0, 1, 0);
	output.svpos.y = output.svpos.y >= PLANE_POS.y ? PLANE_POS.y : output.svpos.y;*/

	output.svpos = mul(mat, output.svpos);
	output.normal = wnormal.xyz;
	output.uv = input.uv;





	return output;
}
