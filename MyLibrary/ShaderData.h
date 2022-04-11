#pragma once

namespace MelLib
{
	enum class FORMAT_TYPE
	{
		UNSIGNED_INT,//unsigned int
		SIGNED_INT,//int
		FLOAT,//float
	};

	//�p�C�v���C���쐬����
	struct InputLayoutData
	{

		//�Z�}���e�B�N�X��
		std::string semantics;

		//����l�̐�
		UINT number = 4;

		//����l�̌^
		FORMAT_TYPE formatType = FORMAT_TYPE::FLOAT;


	};


	enum class DrawMode
	{
		SOLID,//�ʏ�ʂ胂�f����\�����܂�
		WIREFRAME,//���C���[�t���[���ŕ\�����܂�
	};

	enum class CullMode
	{
		NONE,//�J�����O���܂���
		FRONT,//�\�ʂ��J�����O���܂�
		BACK//���ʂ��J�����O���܂�
	};

	enum class BlendMode
	{
		NONE,
		ADD,
		SUB,
	};


	//�p�C�v���C���̐ݒ���܂Ƃ߂��\����
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

	//�V�F�[�_�[�̏����܂Ƃ߂��\����
	struct ShaderData
	{
		const wchar_t* shaderPath;
		const char* entryPoint;
		const char* ver;
	};

	struct ShaderDataSet
	{
		//���_�V�F�[�_�[���
		ShaderData vShaderData;

		//�n���V�F�[�_�[���
		ShaderData hShaderData;

		//�h���C���V�F�[�_�[���
		ShaderData dShaderData;

		//�W�I���g���V�F�[�_�[���
		ShaderData gShaderData;

		//�s�N�Z���V�F�[�_�[���
		ShaderData pShaderData;
	};
}
