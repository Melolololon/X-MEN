#include "Matrix.h"
#include<cmath>

using namespace MelLib;
Matrix::Matrix()
{
}

Matrix::Matrix(const DirectX::XMMATRIX& m)
{
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			matrix[i][j] = m.r[i].m128_f32[j];
}

#pragma region ‰‰ŽZŽq

Matrix Matrix::operator*(const Matrix& mat)const
{
	Matrix m = GetZeroMatrix();

	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			for (int k = 0; k < 4; k++)
				m[i][j] += matrix[i][k] * mat[k][j];

	return m;
}


void Matrix::operator*=(const Matrix& mat)
{
	Matrix m;
	m.matrix = matrix;
	m = m * mat;
	matrix = m.matrix;
}

std::array<float, 4>& Matrix::operator[](const unsigned int num)
{
	return matrix[num];
}

std::array<float, 4> Matrix::operator[](const unsigned int num)const
{
	return matrix[num];
}



#pragma endregion

DirectX::XMMATRIX Matrix::ToXMMATRIX()
{
	return DirectX::XMMatrixSet
	(
		matrix[0][0], matrix[0][1], matrix[0][2], matrix[0][3],
		matrix[1][0], matrix[1][1], matrix[1][2], matrix[1][3],
		matrix[2][0], matrix[2][1], matrix[2][2], matrix[2][3],
		matrix[3][0], matrix[3][1], matrix[3][2], matrix[3][3]
	);
}


Matrix Matrix::GetZeroMatrix()
{
	Matrix m;
	for(auto& mat : m.matrix)
		for(auto& mat2 : mat)
			mat2 = 0;
	return m;
}

Matrix Matrix::GetIdentityMatrix()
{
	Matrix m = GetZeroMatrix();

	for(int i = 0; i < 4;i++)
	m[i][i] = 1;

	return m;
}

Matrix Matrix::GetRotateXMatrix(const float angle)
{
	Matrix m = GetIdentityMatrix();

	m[1][1] = std::cos(angle);
	m[1][2] = std::sin(angle);
	m[2][1] = std::sin(-angle);
	m[2][2] = std::cos(angle);

	return m;
}

Matrix Matrix::GetRotateYMatrix(const float angle)
{
	Matrix m = GetIdentityMatrix();

	m[0][0] = std::cos(angle);
	m[0][2] = std::sin(-angle);
	m[2][0] = std::sin(angle);
	m[2][2] = std::cos(angle);

	return m;
}

Matrix Matrix::GetRotateZMatrix(const float angle)
{
	Matrix m = GetIdentityMatrix();

	m[0][0] = std::cos(angle);
	m[0][1] = std::sin(angle);
	m[1][0] = std::sin(-angle);
	m[1][1] = std::cos(angle);
	
	return m;
}

Matrix MelLib::Matrix::GetRotateZXYMatrix(const Vector3& angle)
{
	return Matrix::GetRotateZMatrix(angle.z) * Matrix::GetRotateXMatrix(angle.x) * Matrix::GetRotateYMatrix(angle.y);
}

Matrix Matrix::GetScalingMatrix(const Vector3& vector)
{
	Matrix m = GetIdentityMatrix();

	m[0][0] = vector.x;
	m[1][1] = vector.y;
	m[2][2] = vector.z;

	return m;
}

Matrix Matrix::GetTranslationMatrix(const Vector3& vector)
{
	Matrix m = GetIdentityMatrix();

	m[3][0] = vector.x;
	m[3][1] = vector.y;
	m[3][2] = vector.z;

	return m;
}

