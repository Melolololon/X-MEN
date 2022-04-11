#include"ObjShaderHeader.hlsli"


[maxvertexcount(3)]//最大頂点数
void GSmain
(
	//triangleはプリミティブのこと
	//[3]は、トライアングルリスト、ストリップということ
	triangle VSOutput input[3],
	inout TriangleStream< GSOutput > output
)
{

	GSOutput element;

	for (uint i = 0; i < 3; i++)
	{
		element.worldPos = input[i].worldPos;
		element.svpos = input[i].svpos + float4(input[i].normal  * ex, 0);
		element.normal = input[i].normal;
		element.uv = input[i].uv;
		output.Append(element);
	}



	output.RestartStrip();

}