
#define NUM_CONTROL_POINTS 3
#define DOMEIN "tri"


//ハルシェーダー記述

//コントロールポイント = 頂点?

//入力コントロールポイント
struct VS_CONTROL_POINT_OUTPUT
{
	//float3 vPosition : WORLDPOS;


	//float4 worldPos:POSITION;
	//float4 svpos : SV_POSITION;
	float4 worldPos : POSITION;
	float4 svpos : SV_POSITION;
	float3 normal:NORMAL;
	float2 uv : TEXCOORD;
};

//Outputするコントロールポイント
struct HS_CONTROL_POINT_OUTPUT
{
	//float3 vPosition : WORLDPOS;

	/*float4 worldPos :POSITION;
	float4 svpos : SV_POSITION;*/
	float4 worldPos : POSITION;
	float4 svpos : SV_POSITION;
	float3 normal:NORMAL;
	float2 uv: TEXCOORD;
};


//出力パッチ定数データ
struct HS_CONSTANT_DATA_OUTPUT
{
	//パッチの各頂点のテッセレーション量(どのくらい分割するか?)(外側の辺をどのくらい分割するか?)
	//[]内は頂点数?
	//見た目だけ細かくしたい場合は、ここだけ値を大きくすればいい?
	float EdgeTessFactor[3] : SV_TessFactor;

	//パッチサーフェス内のテッセレーションの量(ポリゴンの内側の分割量?)
	//スプライトをばらばらにするときや波を作るときなどの平べったい部分(内側。面)も細かくしたいときはこれの値を大きくする?
	float InsideTessFactor : SV_InsideTessFactor;

};



//ドメインシェーダー記述
//ドメインシェーダーのアウトプット
struct DS_OUTPUT
{
	//float4 vPosition  : SV_POSITION;

	float4 worldPos :POSITION;
	float4 svpos : SV_POSITION;
	float3 normal:NORMAL;
	float2 uv: TEXCOORD;
};



