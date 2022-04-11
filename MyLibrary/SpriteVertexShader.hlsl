#include"SpriteShaderHeader.hlsli"

VSOutput VSmain(float4 pos : POSITION, float2 uv : TEXCOORD)
{
	VSOutput output;
	output.svpos = mul(mat, pos);//s—ñ‚ÆÀ•W‚ğæZ
	output.uv = uv;
	return output;
}