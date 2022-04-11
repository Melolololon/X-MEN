#include "GameObject.h"

#include"LibMath.h"
#include"Physics.h"

using namespace MelLib;

float GameObject::gravutationalAcc = Physics::GRAVITATIONAL_ACCELERATION_EARTH / 30;

#ifdef _DEBUG


//判定確認用モデルのパイプライン
PipelineState GameObject::collisionCheckModelPipelineState;

//マテリアル
ADSAMaterial GameObject::material;
#endif // _DEBUG

void MelLib::GameObject::SetModelPosition(const Vector3& vec)
{
	for(auto& m : modelObjects)
	{
		m.second.SetPosition(m.second.GetPosition() + vec);
	}
}

void MelLib::GameObject::SetDataPosition(const Vector3& vec)
{
	for(auto& d : sphereData)
	{
		d.SetPosition(d.GetPosition() + vec);
	}
	for(auto& d : boxData)
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

void GameObject::Hit
(
	const GameObject* const  object,
	const ShapeType3D& collisionType,
	const int arrayNum,
	const ShapeType3D& hitObjColType,
	const int hitObjArrayNum
)
{
}
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


void GameObject::FalsEraseManager() 
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
	for(auto& object:modelObjects)
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
	//力と重さと加速度と速度で座標を計算
	position += Physics::CalcMoveResult
	(
		velocity,
		acceleration,
		force,
		mass
	);

	//落下時の速度
	if (isFall) 
	{
		fallTime++;

		
		const float PRE_VEL_Y = currentFallVelovity;
		currentFallVelovity = Physics::CalcFallVelocity(fallStartSpeed, gravutationalAcc, fallTime);
		const float ADD_VEL_Y = currentFallVelovity - PRE_VEL_Y;

		//Velocity取得時に反映させるためにvelocityに代入
		//Get関数でvelocityに加算するようにする? 
		
		//計算では、今までの速度を加算した合計速度ではなく、現在の速度を求めるため、
		//velovcity.yは0秒の時の速度 + 現在の速度になるようにしないと
		//velocity.y + upThrowVelocity(加算すると現在の速度)と同じにならない。
		velocity.y += ADD_VEL_Y;

		//毎フレーム速度を加算
		position.y += currentFallVelovity;
		
	}


	//重力加速度は落下中のみ適応するように変更して!!!!!!!
	//重力加速度適応
	//position.y -= gravutationalAcc;


	//ここで移動量を計算して、判定の座標を調整する?
	//モデルはどうする?

	//反発の計算関数はhit関数で呼び出してもらう?

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
	//判定数に応じてモデルを生成したり削除したりします
	auto createOrDeleteModel = [](const size_t& dataNum,std::vector<ModelObject>& modelObjcts,const ShapeType3D type)
	{
		size_t objNum = modelObjcts.size();
		if(dataNum > objNum)
		{
			size_t addSize = dataNum - objNum;
			modelObjcts.resize(dataNum);

			//不足分生成
			for(int i = objNum; i < dataNum;i++)
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
	//球を作成
	createOrDeleteModel(capsuleData.size(), capsuleModelObjects[0], ShapeType3D::BOX);
	createOrDeleteModel(capsuleData.size(), capsuleModelObjects[1], ShapeType3D::BOX);
	//円注を作成
	//createOrDeleteModel(capsuleData.size(), capsuleModelObjects[2], ShapeType3D::BOX);
}

void MelLib::GameObject::SetCollisionCheckModelData()
{
	//Box
	size_t dataNum = boxData.size();
	for(size_t i = 0; i < dataNum;i++)
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
	for(size_t i = 0; i < dataNum;i++)
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
		
		//四角形はスケール1だと1辺が1なので、半径1のときは1辺を2にしないといけないため、2倍
		capsuleModelObjects[0][i].SetScale(capsuleData[i].GetRadius() * 2);
		capsuleModelObjects[0][i].SetPosition(lineSegmentPos.v1);
		capsuleModelObjects[0][i].SetAngle(capsuleData[i].GetSegment3DData().GetAngle());

		capsuleModelObjects[1][i].SetScale(capsuleData[i].GetRadius() * 2);
		capsuleModelObjects[1][i].SetPosition(lineSegmentPos.v2);
		capsuleModelObjects[1][i].SetAngle(capsuleData[i].GetSegment3DData().GetAngle());

		//円柱
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

	for(auto& box : boxModelObjects)
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
