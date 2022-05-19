#pragma once
#include "FieldObject.h"
#include <vector>

class FieldObjectManager
{
private:
	std::unordered_map<FieldObjectType,std::shared_ptr<std::vector<std::shared_ptr<FieldObject>>>> fieldObjects;
private:
	// �ǂ�ǉ�����
	void AddWall(const MelLib::Vector3& pos,const MelLib::Vector3& size,const MelLib::Vector3& angle = MelLib::Vector3());
	// �ǌQ��ǉ�����
	void AddWalls();

	// ���p�`���w�肵�ĕǌQ��ǉ�����
	void AddWalls(const unsigned int VALUE);
public:
	static FieldObjectManager* GetInstance();

	// �������m�ۂȂ�
	void Initialize();

	// �ێ����Ă���I�u�W�F�N�g�̃|�C���^�z����N���A������A�ǉ���������ێ�����ϐ�������������
	void Finalize();

	// �����̃n�b�V���l�i�t�B�[���h�I�u�W�F�N�g�̃^�C�v�j�����Ƃɔz��̃X�}�[�g�|�C���^��Ԃ�
	// �^������ɕK�v��enum class���������� auto& �� GetInstance���ɂ��Ă����Ȃǂ���Ɖǐ��オ��܂�
	// auto& ������ ��) auto& fieldObjects = FieldObjectManager::GetInstance()->GetFieldObjects(FieldObjectType::FIELD_OBJECT_TYPE_WALL);
	std::shared_ptr<std::vector<std::shared_ptr<FieldObject>>> GetFieldObjects(FieldObjectType hash);


private:
	// �V���O���g���p
	FieldObjectManager();
	FieldObjectManager(const FieldObjectManager&) = delete;
	~FieldObjectManager() = default;
	FieldObjectManager& operator=(const FieldObjectManager&) = delete;
};
