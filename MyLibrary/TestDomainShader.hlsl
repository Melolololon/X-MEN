#include"FbxShaderHeader.hlsli"
#include"TriTessellateHeader.hlsli"

//テッセレーションコラム
//https://game.watch.impress.co.jp/docs/series/3dcg/590707.html

//出力された点(コントロールポイント)をいじるシェーダー
//ここで折り曲げる処理を実装することによって、ローポリモデルを細かい滑らかなモデルにすることができる


//テッセレータステージから出力された頂点ごとに呼び出される
[domain(DOMEIN)]//ドメイン属性
DS_OUTPUT main(
	HS_CONSTANT_DATA_OUTPUT input,
	float3 domain : SV_DomainLocation,//UV座標のこと? tryだとfloat3(UVW) この座標は、ポリゴン上のどの辺にあるかを示す
	const OutputPatch<HS_CONTROL_POINT_OUTPUT, NUM_CONTROL_POINTS> patch)
{
	//仮の平面
	static const float3 PLANE_POS = float3(0, 1, 0);


	//上手くやれば三角形でも四角形と同じようなきれいな分割ができるはず

	DS_OUTPUT Output;

	//pは、テッセレータに渡した頂点(コントロールポイント)と、ドメイン(ポリゴン上UV(W)座標)から求めた頂点座標

	//三角形の場合は、uvw全部が0になることはない?
	//四角形だとある?あるから patch[0].pos * domain.x + patch[0].pos * domain.yの式で原点に頂点配置される?
	float4 p = patch[0].svpos * domain.x + patch[1].svpos * domain.y + patch[2].svpos * domain.z;
	
	//p.y = p.y >= PLANE_POS.y ? PLANE_POS.y : p.y;
	
	Output.svpos = p;
	
	//partitioningで頂点がどの頂点とつながるかが決まるから下の式では無理
	/*float3 p0ToP1 = patch[1].pos.xyz - patch[0].pos.xyz;
	float3 p1ToP2 = patch[2].pos.xyz - patch[1].pos.xyz;
	float3 p2ToP0 = patch[0].pos.xyz - patch[2].pos.xyz;
	float4 p = 
		float4(patch[0].pos.xyz + p0ToP1 * domain.x
	     + (p1ToP2 * domain.y + p0ToP1 * domain.y)
	     + (p2ToP0 * domain.z + -p2ToP0 * domain.z)
			, 1);*/

	//無理やりquadでやる方法
	//うまくいかなかった
	//float3 p0ToP1 = patch[1].pos.xyz - patch[0].pos.xyz;
	//float3 p1ToP2 = patch[2].pos.xyz - patch[1].pos.xyz;
	//float4 p = float4(patch[0].pos.xyz + p0ToP1 * domain.y
	//	+ (p1ToP2 * domain.x + p0ToP1 * (1 - domain.y) * domain.x)
	//	, 1);


	//Output.worldPos = mul(worldMat, p);
	//Output.svpos = mul(mat, p);

	Output.uv = patch[0].uv * domain.x + patch[1].uv * domain.y + patch[2].uv * domain.z;


	p = patch[0].worldPos * domain.x + patch[1].worldPos * domain.y + patch[2].worldPos * domain.z;
	
	Output.worldPos = p;

	//折り曲げないから法線はそのまま
	Output.normal = patch[0].normal;

	

	return Output;
}






