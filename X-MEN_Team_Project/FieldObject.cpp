#include "FieldObject.h"

FieldObject::FieldObject()
	: fieldObjectType(FieldObjectType::FIELD_OBJECT_TYPE_NONE)
{
}

FieldObject::~FieldObject()
{
}

FieldObjectType FieldObject::GetFieldObjectType() const
{
	return fieldObjectType;
}

void FieldObject::SetFieldObjectType(FieldObjectType type)
{
	fieldObjectType = type;
}
