#pragma once
#include "FieldObject.h"
#include <vector>

class FieldObjectManager
{
private:
	std::vector<std::shared_ptr<FieldObject>> fieldObjects;
private:
	// �ǂ�ǉ�����
	void AddWall(const MelLib::Vector3& pos,const MelLib::Vector3& size);
	// �ǌQ��ǉ�����
	void AddWalls();
public:
	FieldObjectManager();

	// �������m�ۂȂ�
	void Initialize();

	// �ێ����Ă���I�u�W�F�N�g�̃|�C���^�z����N���A������A�ǉ���������ێ�����ϐ�������������
	void Finalize();
};
