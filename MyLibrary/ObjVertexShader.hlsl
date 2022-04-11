#include"ObjShaderHeader.hlsli"

VSOutput VSmain(float4 pos : POSITION, float2 uv : TEXCOORD, float3 normal : NORMAL)
{

	VSOutput output;
	output.worldPos = mul(worldMat, pos);
	output.svpos = mul(mat, pos);
	output.normal = normalize(mul(normalMat, normal));
	output.uv = uv;

	return output;
}