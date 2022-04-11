#pragma once

namespace MelLib
{
	enum class FORMAT_TYPE
	{
		UNSIGNED_INT,//unsigned int
		SIGNED_INT,//int
		FLOAT,//float
	};

	//パイプライン作成時に
	struct InputLayoutData
	{

		//セマンティクス名
		std::string semantics;

		//送る値の数
		UINT number = 4;

		//送る値の型
		FORMAT_TYPE formatType = FORMAT_TYPE::FLOAT;


	};


	enum class DrawMode
	{
		SOLID,//通常通りモデルを表示します
		WIREFRAME,//ワイヤーフレームで表示します
	};

	enum class CullMode
	{
		NONE,//カリングしません
		FRONT,//表面をカリングします
		BACK//裏面をカリングします
	};

	enum class BlendMode
	{
		NONE,
		ADD,
		SUB,
	};


	//パイプラインの設定をまとめた構造体
	struct DrawData
	{
		DrawMode drawMode = DrawMode::SOLID;
		CullMode cullMode = CullMode::BACK;
		BlendMode blendMode = BlendMode::ADD;

		bool depthTest = false;
		bool alphaWrite = false;

		void SetModelDefData()
		{
			drawMode = DrawMode::SOLID;
			cullMode = CullMode::BACK;
			blendMode = BlendMode::ADD;
			depthTest = true;
			alphaWrite = true;
		}
	};

	//シェーダーの情報をまとめた構造体
	struct ShaderData
	{
		const wchar_t* shaderPath;
		const char* entryPoint;
		const char* ver;
	};

	struct ShaderDataSet
	{
		//頂点シェーダー情報
		ShaderData vShaderData;

		//ハルシェーダー情報
		ShaderData hShaderData;

		//ドメインシェーダー情報
		ShaderData dShaderData;

		//ジオメトリシェーダー情報
		ShaderData gShaderData;

		//ピクセルシェーダー情報
		ShaderData pShaderData;
	};
}
