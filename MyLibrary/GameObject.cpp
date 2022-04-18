#include "GameObject.h"

#include"LibMath.h"
#include"Physics.h"

using namespace MelLib;

float GameObject::gravutationalAcc = Physics::GRAVITATIONAL_ACCELERATION_EARTH / 30;

#ifdef _DEBUG


//����m�F�p���f���̃p�C�v���C��
PipelineState GameObject::collisionCheckModelPipelineState;

//�}�e���A��
ADSAMaterial GameObject::material;
#endif // _DEBUG

void MelLib::GameObject::SetModelPosition(const Vector3& vec)
{
	for (auto& m : modelObjects)
	{
		m.second.SetPosition(m.second.GetPosition() + vec);
	}
}

void MelLib::GameObject::SetDataPosition(const Vector3& vec)
{
	for (auto& d : sphereData)
	{
		d.SetPosition(d.GetPosition() + vec);
	}
	for (auto& d : boxData)
	{
		d.SetPosition(d.GetPosition() + vec);
	}
	for (auto& d : boardData)
	{
		d.SetPosition(d.GetPosition() + vec);
	}
	for (auto& d : segment3DData)
	{
		d.SetPosition(d.GetPosition() + vec);
	}
	for (auto& d : capsuleData)
	{
		d.GetRefSegment3DData().SetPosition(d.GetSegment3DData().GetPosition() + vec);
	}
}

void MelLib::GameObject::SetModelAngle(const Vector3& angle)
{

	for (auto& m : modelObjects)
	{
		m.second.SetAngle(angle);
	}
}

void MelLib::GameObject::SetDataAngle(const Vector3& angle)
{

	for (auto& d : boardData)
	{
		d.SetAngle(angle);
	}
	for (auto& d : segment3DData)
	{
		d.SetAngle(angle);
	}
	for (auto& d : capsuleData)
	{
		d.GetRefSegment3DData().SetAngle(angle);
	}
}

void MelLib::GameObject::SetModelScale(const Vector3& scale)
{
	for (auto& m : modelObjects)
	{
		m.second.SetScale(scale);
	}
}

void MelLib::GameObject::SetDataScale(const Vector3& scale)
{
	for (auto& d : sphereData)
	{
		float setRadius = scale.x;
		if (setRadius < scale.y)setRadius = scale.y;
		if (setRadius < scale.z)setRadius = scale.z;

		d.SetRadius(setRadius / 2);
	}
	for (auto& d : boxData)
	{
		d.SetSize(scale);
	}
	for (auto& d : boardData)
	{
		d.SetSize(scale.ToVector2());
	}
	for (auto& d : segment3DData)
	{
		d.SetPosition(d.GetPosition() * scale);
	}
	for (auto& d : capsuleData)
	{
		d.GetRefSegment3DData().SetPosition(d.GetSegment3DData().GetPosition() * scale);
	}
}

GameObject::GameObject()
{
}


GameObject::~GameObject()
{
}

//void GameObject::Initialize()
//{
//}

void GameObject::Update()
{
}

void GameObject::Draw()
{

}

//void GameObject::Hit
//(
//	const GameObject* const  object,
//	const ShapeType3D& collisionType,
//	const int arrayNum,
//	const ShapeType3D& hitObjColType,
//	const int hitObjArrayNum
//)
//{
//}
//
//const void* GameObject::GetPtr() const
//{
//	return nullptr;
//}

//void GameObject::CalcHitPhysics(GameObject* hitObject, const Vector3& hutPreVelocity, const CollisionType& collisionType)
//{
//
//	//switch (collisionType)
//	//{
//	//case CollisionType::COLLISION_SPHERE:
//	//	
//	//	velocity = Physics::CalcRepulsionVelocity
//	//	(
//	//	);
//	//	break;
//	//}
//}


void MelLib::GameObject::Hit
(
	const GameObject& object,
	const ShapeType3D& shapeType,
	const std::string& shapeName,
	const ShapeType3D& hitObjShapeType,
	const std::string& hitShapeName
)
{
}

void GameObject::FalseEraseManager()
{
	eraseManager = false;
}

void MelLib::GameObject::AddPosition(const Vector3& vec)
{
	SetModelPosition(vec);
	SetDataPosition(vec);
	position += vec;
}

void MelLib::GameObject::SetPosition(const Vector3& pos)
{
	SetModelPosition(pos - position);
	SetDataPosition(pos - position);

	position = pos;

}

void MelLib::GameObject::SetAngle(const Vector3& angle)
{
	if (this->angle == angle)return;

	this->angle = angle;

	SetModelAngle(angle);
	SetDataAngle(angle);
}

void MelLib::GameObject::SetScale(const Vector3& scale)
{
	if (this->scale == scale)return;

	this->scale = scale;

	SetModelScale(scale);
	SetDataScale(scale);
}

void MelLib::GameObject::SetAddColor(const Color& color)
{
	for (auto& object : modelObjects)
	{
		object.second.SetAddColor(color);
	}
}

void MelLib::GameObject::SetSubColor(const Color& color)
{
	for (auto& object : modelObjects)
	{
		object.second.SetSubColor(color);
	}
}

void MelLib::GameObject::SetMulColor(const Color& color)
{
	for (auto& object : modelObjects)
	{
		object.second.SetMulColor(color);
	}
}

void GameObject::CalcMovePhysics()
{
	Vector3 prePos = position;
	//�͂Əd���Ɖ����x�Ƒ��x�ō��W���v�Z
	position += Physics::CalcMoveResult
	(
		velocity,
		acceleration,
		force,
		mass
	);

	//�������̑��x
	if (isFall)
	{
		fallTime++;


		const float PRE_VEL_Y = currentFallVelovity;
		currentFallVelovity = Physics::CalcFallVelocity(fallStartSpeed, gravutationalAcc, fallTime);
		const float ADD_VEL_Y = currentFallVelovity - PRE_VEL_Y;

		//Velocity�擾���ɔ��f�����邽�߂�velocity�ɑ��
		//Get�֐���velocity�ɉ��Z����悤�ɂ���? 

		//�v�Z�ł́A���܂ł̑��x�����Z�������v���x�ł͂Ȃ��A���݂̑��x�����߂邽�߁A
		//velovcity.y��0�b�̎��̑��x + ���݂̑��x�ɂȂ�悤�ɂ��Ȃ���
		//velocity.y + upThrowVelocity(���Z����ƌ��݂̑��x)�Ɠ����ɂȂ�Ȃ��B
		velocity.y += ADD_VEL_Y;

		//���t���[�����x�����Z
		position.y += currentFallVelovity;

	}


	//�d�͉����x�͗������̂ݓK������悤�ɕύX����!!!!!!!
	//�d�͉����x�K��
	//position.y -= gravutationalAcc;


	//�����ňړ��ʂ��v�Z���āA����̍��W�𒲐�����?
	//���f���͂ǂ�����?

	//�����̌v�Z�֐���hit�֐��ŌĂяo���Ă��炤?

	SetModelPosition(position - prePos);
	SetDataPosition(position - prePos);
}






#ifdef _DEBUG



void MelLib::GameObject::CreateCollisionCheckModelPipelineState()
{
	DrawData pData = PipelineState::GetDefaultDrawData(PipelineStateType::MODEL);
	pData.drawMode = DrawMode::WIREFRAME;
	pData.cullMode = CullMode::NONE;

	ShaderDataSet set =
	{
		{L"LIB","",""},
		{L"NULL","",""},
		{L"NULL","",""},
		{L"LIB","",""},
		{L"LIB","",""}
	};

	collisionCheckModelPipelineState.CreatePipeline
	(
		pData,
		set,
		PipelineStateType::MODEL,
		nullptr,
		1
	);

	DrawData data = PipelineState::GetDefaultDrawData(PipelineStateType::MODEL);
	data.cullMode = CullMode::NONE;
	data.drawMode = DrawMode::WIREFRAME;

	material.Create(data);

}

void MelLib::GameObject::CreateCollisionCheckModel()
{
	//���萔�ɉ����ă��f���𐶐�������폜�����肵�܂�
	auto createOrDeleteModel = [](const size_t& dataNum, std::vector<ModelObject>& modelObjcts, const ShapeType3D type)
	{
		size_t objNum = modelObjcts.size();
		if (dataNum > objNum)
		{
			size_t addSize = dataNum - objNum;
			modelObjcts.resize(dataNum);

			//�s��������
			for (int i = objNum; i < dataNum; i++)
			{
				modelObjcts[i].Create(ModelData::Get(type), nullptr);

				modelObjcts[i].SetMaterial(&material);
			}
		}
		else if (dataNum < objNum)
		{
			modelObjcts.resize(objNum);
		}

	};

	//Box
	createOrDeleteModel(boxData.size(), boxModelObjects, ShapeType3D::BOX);

	//Sphere
	createOrDeleteModel(sphereData.size(), sphereModelObjects, ShapeType3D::BOX);

	//Board
	createOrDeleteModel(boardModelObjects.size(), boardModelObjects, ShapeType3D::BOARD);

	//Segment
	createOrDeleteModel(segment3DData.size(), segmentModelObjects[0], ShapeType3D::BOX);
	createOrDeleteModel(segment3DData.size(), segmentModelObjects[1], ShapeType3D::BOX);

	//Capsule
	//�����쐬
	createOrDeleteModel(capsuleData.size(), capsuleModelObjects[0], ShapeType3D::BOX);
	createOrDeleteModel(capsuleData.size(), capsuleModelObjects[1], ShapeType3D::BOX);
	//�~�����쐬
	//createOrDeleteModel(capsuleData.size(), capsuleModelObjects[2], ShapeType3D::BOX);
}

void MelLib::GameObject::SetCollisionCheckModelData()
{
	//Box
	size_t dataNum = boxData.size();
	for (size_t i = 0; i < dataNum; i++)
	{
		boxModelObjects[i].SetScale(boxData[i].GetSize());
		boxModelObjects[i].SetPosition(boxData[i].GetPosition());
	}

	//Sphere
	dataNum = sphereData.size();
	for (size_t i = 0; i < dataNum; i++)
	{
		sphereModelObjects[i].SetScale(sphereData[i].GetRadius() * 2);
		sphereModelObjects[i].SetPosition(sphereData[i].GetPosition());
	}

	//Board
	dataNum = boardModelObjects.size();
	for (size_t i = 0; i < dataNum; i++)
	{
		boardModelObjects[i].SetScale(boardData[i].GetSize().ToVector3());
		boardModelObjects[i].SetAngle(boardData[i].GetAngle());
		boardModelObjects[i].SetPosition(boardData[i].GetPosition());
	}

	//Segment
	dataNum = segment3DData.size();
	for (size_t i = 0; i < dataNum; i++)
	{
		Value2<Vector3>lineSegmentPos = segment3DData[i].GetRotatePosition();

		segmentModelObjects[0][i].SetScale(1);
		segmentModelObjects[0][i].SetPosition(lineSegmentPos.v1);
		segmentModelObjects[0][i].SetAngle(segment3DData[i].GetAngle());

		segmentModelObjects[1][i].SetScale(1);
		segmentModelObjects[1][i].SetPosition(lineSegmentPos.v2);
		segmentModelObjects[1][i].SetAngle(segment3DData[i].GetAngle());

	}


	//Capsule
	dataNum = capsuleData.size();
	for (size_t i = 0; i < dataNum; i++)
	{
		Value2<Vector3>lineSegmentPos = capsuleData[i].GetSegment3DData().GetRotatePosition();

		//�l�p�`�̓X�P�[��1����1�ӂ�1�Ȃ̂ŁA���a1�̂Ƃ���1�ӂ�2�ɂ��Ȃ��Ƃ����Ȃ����߁A2�{
		capsuleModelObjects[0][i].SetScale(capsuleData[i].GetRadius() * 2);
		capsuleModelObjects[0][i].SetPosition(lineSegmentPos.v1);
		capsuleModelObjects[0][i].SetAngle(capsuleData[i].GetSegment3DData().GetAngle());

		capsuleModelObjects[1][i].SetScale(capsuleData[i].GetRadius() * 2);
		capsuleModelObjects[1][i].SetPosition(lineSegmentPos.v2);
		capsuleModelObjects[1][i].SetAngle(capsuleData[i].GetSegment3DData().GetAngle());

		//�~��
		/*Value2<Vector3>segmentPos = capsuleData[i].GetSegment3DData().GetRotatePosition();
		float segmentLength = (segmentPos.v1.y - segmentPos.v2.y);

		capsuleModelObjects[2][i].SetScale
		(Vector3(capsuleData[i].GetRadius() * 2, segmentLength, capsuleData[i].GetRadius() * 2));
		capsuleModelObjects[2][i].SetPosition(LibMath::CalcCenterPosition3D(segmentPos.v1, segmentPos.v2));
		capsuleModelObjects[2][i].SetAngle(capsuleData[i].GetSegment3DData().GetAngle());*/
	}

}

void MelLib::GameObject::DrawCollisionCheckModel()
{
	if (!drawCollisionModel)return;

	for (auto& box : boxModelObjects)
	{
		box.Draw();
	}

	for (auto& sphere : sphereModelObjects)
	{
		sphere.Draw();
	}

	for (int i = 0, size = capsuleModelObjects.size(); i < size; i++)
	{
		for (auto& capsule : capsuleModelObjects[i])
		{
			capsule.Draw();
		}
	}
}

#endif // _DEBUG
