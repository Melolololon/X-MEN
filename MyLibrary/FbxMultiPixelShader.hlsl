#include"FbxMultiShaderHeader.hlsli"

Texture2D<float4> tex:register(t0);
Texture2D<float4> tex2:register(t1);
SamplerState smp:register(s0);

struct PSOutput
{
	float4 target0 : SV_TARGET0;
	float4 target1 : SV_TARGET1;
};

PSOutput main(GSOutput input)
{
	float4 texColor = tex.Sample(smp, input.uv);
	

	float4 shaderColor;


	const float shininess = 4.0f;//Œõ‘ò“x

	float3 lightDir = normalize(mul(lightMat, light));
	const float3 eyeDir = normalize(cameraPos - input.worldPos.xyz);

	float3 dotlightnormal = dot(-lightDir, input.normal);
	float3 reflect = normalize(lightDir + 2 * dotlightnormal * input.normal);

	float3 ambient = m_ambient;
	float3 diffuse = dotlightnormal * m_diffuse;
	float3 specular = pow(saturate(dot(reflect, eyeDir)), shininess) * m_specular;


	shaderColor.rgb = (ambient + diffuse + specular) * lightColor;
	shaderColor.a = m_alpha;

	texColor += addColor - subColor;
	texColor *= mulColor;
	//return shaderColor * texColor;

	PSOutput output;

	output.target0 = texColor;
	//output.target1 = float4(1 - texColor.rgb, 1);
	output.target1 = float4(0,1,0, 1);
	
	return output;

}