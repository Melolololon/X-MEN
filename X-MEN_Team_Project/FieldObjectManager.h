#pragma once
#include "FieldObject.h"
#include <vector>

namespace FieldObjectManagerInfo
{
	// sec
	const float MAX_LOOMING_TIME = 1;
}

class FieldObjectManager
{
private:
	std::unordered_map<FieldObjectType,std::shared_ptr<std::vector<std::shared_ptr<FieldObject>>>> fieldObjects;

	bool isLooming;
	float loomingTime;
	int loomingCount;
private:
	// �ǂ�ǉ�����
	void AddWall(const MelLib::Vector3& pos,const MelLib::Vector3& size,const MelLib::Vector3& angle = MelLib::Vector3());
	// �ǌQ��ǉ�����
	void AddWalls();

	// ���p�`���w�肵�ĕǌQ��ǉ�����
	// isRotate�͗Ⴆ�Ύl�p�`�̏ꍇ�A�΂߂𐳂�
	void AddWalls(const unsigned int VALUE,bool isRotate);

	// �}�b�v�k�܂�t���O�֌W�̍X�V
	void LoomingUpdate();
public:
	static FieldObjectManager* GetInstance();

	// �������m�ۂȂ�
	void Initialize();

	void Update();

	// �ێ����Ă���I�u�W�F�N�g�̃|�C���^�z����N���A������A�ǉ���������ێ�����ϐ�������������
	void Finalize();

	// �����̃n�b�V���l�i�t�B�[���h�I�u�W�F�N�g�̃^�C�v�j�����Ƃɔz��̃X�}�[�g�|�C���^��Ԃ�
	// �^������ɕK�v��enum class���������� auto& �� GetInstance���ɂ��Ă����Ȃǂ���Ɖǐ��オ��܂�
	// auto& ������ ��) auto& fieldObjects = FieldObjectManager::GetInstance()->GetFieldObjects(FieldObjectType::FIELD_OBJECT_TYPE_WALL);
	std::shared_ptr<std::vector<std::shared_ptr<FieldObject>>> GetFieldObjects(FieldObjectType hash);

	// �}�b�v���k�߂�t���O�𗧂Ă�
	void Looming();

#pragma region Getter

	// �}�b�v���k�܂�t���O���m�F����
	bool IsLooming() const;
#pragma endregion

private:
	// �V���O���g���p
	FieldObjectManager();
	FieldObjectManager(const FieldObjectManager&) = delete;
	~FieldObjectManager() = default;
	FieldObjectManager& operator=(const FieldObjectManager&) = delete;
};
