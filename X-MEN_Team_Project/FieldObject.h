#pragma once
#include <GameObject.h>

enum class FieldObjectType
{
	FIELD_OBJECT_TYPE_NONE,
	FIELD_OBJECT_TYPE_WALL,

	// ��������d�l���ŃI�u�W�F�N�g�̎�ނ���������ǉ����Ă���
};

class FieldObject : public MelLib::GameObject
{
protected:
	FieldObjectType fieldObjectType;
public:
	FieldObject();
	virtual ~FieldObject();

#pragma region Getter
	FieldObjectType GetFieldObjectType() const;
#pragma endregion

#pragma region Setter
	void SetFieldObjectType(FieldObjectType type);
#pragma endregion
};