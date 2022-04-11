//PBRマテリアル
cbuffer cbuff2 : register(b2)
{
	//float3で宣言したとき、float4分の領域を確保するが、
	//次の変数が隙間と同じかそれ以下だったらそっちに入れてくれるっぽい?
	//逆にオーバーしてると、隊が変なところに入る?

	float metalness;
	float specular;
	float roughness;
}



