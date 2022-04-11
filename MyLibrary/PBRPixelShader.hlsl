#include"FbxShaderHeader.hlsli"
#include"PBRHeader.hlsli"

static const float PI = 3.141592654f;
static float3 N;
Texture2D<float4> tex:register(t0);
SamplerState smp:register(s0);



float SchlickFresnel(float f0, float f90, float cosine)
{
	float m = saturate(1 - cosine);
	float m2 = m * m;
	float m5 = m2 * m2 * m;
	return lerp(f0, f90, m5);
}

float3 SchlickFresnel3(float3 f0, float3 f90, float cosine)
{
	float m = saturate(1 - cosine);
	float m2 = m * m;
	float m5 = m2 * m2 * m;
	return lerp(f0, f90, m5);
}


//UE4のGGX分布
float DistributionGGX(float alpha, float NdotH)
{
	float alpha2 = alpha * alpha;
	float t = NdotH * NdotH * (alpha2 - 1.0f) + 1.0f;

	return alpha2 / (PI * t * t);

}

//ディズニーのフレネル計算
float3 DisneyFresnel(float LdotH,float3 color)
{
	//輝度
	float luminance = 0.3f * color.r + 0.6f * color.g + 0.1f * color.b;
	
	//色合い
	float3 tintColor = color / luminance;

	//非金属の鏡面反射色
	float3 nonMetalColor = specular * 0.08f * tintColor;

	//metalnessによる補間(金属の場合はcolorになる)
	float3 specularColor = lerp(nonMetalColor, color, metalness);

	return SchlickFresnel3(specularColor, float3(1, 1, 1), LdotH);
}

//G項
float GeometricSmith(float cosine)
{
	float k = (roughness + 1.0f);
	k = k * k / 8.0f;
	return cosine / (cosine * (1.0f - k) + k);
}

//鏡面反射の計算
float3 CookTorranceSpecular(float NdotL,float NdotV,float NdotH,float LdotH, float3 color)
{
	//全体の計算式
	//return = D(h)F(v,h)G(l,v,h) / 4(n・l)(n・v)

	//D項(分布)(Distribution)
	float Ds = DistributionGGX(roughness * roughness, NdotH);

	//F項(フレネル)
	float3 Fs = DisneyFresnel(LdotH ,color);

	//G項(幾何減衰)
	float Gs = GeometricSmith(NdotL) * GeometricSmith(NdotV);

	//m項(分母)
	float m = 4.0f * NdotL * NdotV;

	return Ds * Fs * Gs / m;
}


//双方向反射率分布関数
float3 BRDF(float3 L,float3 V ,float3 color)
{
	//法線とライトの内積
	float NdotL = dot(N, L);
	//法線とカメラ方向の内積
	float NdotV = dot(N, V);

	//どちらかが90度以上だったら真っ黒
	if (NdotL < 0 || NdotV < 0) 
	{
		return float3(0, 0, 0); 
	}

	//ハーフベクトル
	float3 H = normalize(L + V);
	float NdotH = dot(N, H);
	float LdotH = dot(L, H);
	
	//拡散反射率
	float diffuseReflectance = 1.0f / PI;

	float energyBias = 0.5f * roughness;
	//入射角が90度の場合の拡散反射率
	float Fd90 = energyBias + 2.0f * LdotH * LdotH * roughness;
	//入っていく時の拡散反射率
	float FL = SchlickFresnel(1.0f, Fd90, NdotL);
	//出ていくときの拡散反射率
	float FV = SchlickFresnel(1.0f, Fd90, NdotV);
	float energyFactor = lerp(1.0f, 1.0f / 1.51f, roughness);
	//入って出ていくまでの拡散反射率
	float Fd = FL * FV * energyFactor;

	//拡散反射項
	float3 diffuseColor = diffuseReflectance * Fd * baseColor * (1 - metalness);


	//鏡面反射項
	float3 specularColor = CookTorranceSpecular(NdotL, NdotV, NdotH, LdotH,color);

	return diffuseColor + specularColor;
	
}




float4 main(GSOutput input) : SV_TARGET
{
	float4 color = tex.Sample(smp, input.uv) + baseColor;


	//以下PBRの処理
	N = input.normal;
	float3 finalRGB = float3(0, 0, 0);
	float3 eyedir = normalize(cameraPos.xyz - input.worldPos.xyz);

	for (int i = 0; i < DIR_LIGTH_MAX; i++)
	{
		finalRGB += BRDF(-light[i].xyz, eyedir,color.rgb) * lightColor[i];
	}

	return float4(finalRGB, color.a);
}
