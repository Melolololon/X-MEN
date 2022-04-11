//ハルシェーダー書き方
//https://docs.microsoft.com/ja-jp/windows/win32/direct3d11/direct3d-11-advanced-stages-hull-shader-design

//テッセレーションステージ解説
//https://docs.microsoft.com/ja-jp/windows/win32/direct3d11/direct3d-11-advanced-stages-tessellation#hull-shader-stage

//ハルシェーダーは、2種類出力する(コントロールポイントと、パッチ定数データ)

#include"FbxShaderHeader.hlsli"
#include"QuadTessellateHeader.hlsli"

//パッチ定数関数
//EdgeTessFactorなどの変数の解説は、セマンティクスの解説に書かれている
HS_CONSTANT_DATA_OUTPUT CalcHSPatchConstants(
	InputPatch<VS_CONTROL_POINT_OUTPUT, NUM_CONTROL_POINTS> ip,
	uint PatchID : SV_PrimitiveID)
{
	HS_CONSTANT_DATA_OUTPUT Output;

	//出力を計算するコード(1から32?1で通常通り)
	Output.EdgeTessFactor[0] = 9;
	Output.EdgeTessFactor[1] = 9;
	Output.EdgeTessFactor[2] = 9;
	Output.EdgeTessFactor[3] = 9;
	Output.InsideTessFactor[0] = 9;
	Output.InsideTessFactor[1] = 9;


	return Output;
}


//ハルシェーダーを定義している
//partitioningなどの説明は、
//https://docs.microsoft.com/ja-jp/windows/win32/direct3d11/direct3d-11-advanced-stages-tessellation
//のテッセレータステージの部分に書かれている

//ドメイン(ハルシェーダーで使用される修正プログラム)"try"(3)"quad"(4)"isoline"(2)
//今回は三角形単位で分割するため、"try"(これコマンドリストでセットしたトポロジーに関係なく設定できる?)
[domain(DOMEIN)]
//テッセレーションスキーム(分割方法)
[partitioning("integer")]
//出力のプリミティブ(出力したときの形状)
[outputtopology("triangle_cw")]
//出力コントロールポイントの数(mainが実行される回数)
[outputcontrolpoints(NUM_CONTROL_POINTS)]
//パッチ定数データを計算する関数名
[patchconstantfunc("CalcHSPatchConstants")]
HS_CONTROL_POINT_OUTPUT main(
	InputPatch<VS_CONTROL_POINT_OUTPUT, NUM_CONTROL_POINTS> ip,
	uint i : SV_OutputControlPointID,
	uint PatchID : SV_PrimitiveID)
{
	HS_CONTROL_POINT_OUTPUT Output;

	Output.pos = ip[i].pos;
	Output.normal = ip[i].normal;
	Output.uv = ip[i].uv;

	return Output;
}
