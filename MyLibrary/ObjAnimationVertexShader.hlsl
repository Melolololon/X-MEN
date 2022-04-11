#include"ObjShaderHeader.hlsli"

VSOutput VSmain
(
	float4 pos : POSITION, 
	float2 uv : TEXCOORD, 
	float3 normal : NORMAL,
	unsigned int bone : BONENUM
)
{
	matrix boneMat = boneMatArray[bone];

	VSOutput output;


	//ボーンの行列の乗算
	output.worldPos = mul(boneMat, pos);
	output.worldPos = mul(worldMat, output.worldPos);

	//ボーンの行列の乗算
	output.svpos = mul(boneMat, pos);
	output.svpos = mul(mat, output.svpos);

	//法線にもボーンの行列掛けないといけないのでは
	//正規化するからnormalMatいらないのでは
	//normalMatはビュープロジェクション掛けてないけど
	//matは掛けてるから違う?
	output.normal = normalize(mul(boneMat, normal));
	output.normal = normalize(mul(normalMat, output.normal));
	output.uv = uv;

	return output;

}
