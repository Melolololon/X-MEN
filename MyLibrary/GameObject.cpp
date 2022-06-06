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
	for (auto& m : modelObjects)
	{
		m.second.SetPosition(m.second.GetPosition() + vec);
	}
}

void MelLib::GameObject::SetDataPosition(const Vector3& vec)
{
	for (auto& d : sphereDatas)
	{
		for (auto& d2 : d.second) 
		{
			d2.SetPosition(d2.GetPosition() + vec);
		}
	}
	for (auto& d : boxDatas)
	for (auto& d : obbDatas)
	{
		for (auto& d2 : d.second)
		{
			d2.SetPosition(d2.GetPosition() + vec);
		}
	}
	for (auto& d : boardDatas)
	{
		for (auto& d2 : d.second)
		{
			d2.SetPosition(d2.GetPosition() + vec);
		}
	}
	for (auto& d : segment3DDatas)
	{
		for (auto& d2 : d.second)
		{
			d2.SetPosition(d2.GetPosition() + vec);
		}
	}
	for (auto& d : capsuleDatas)
	{
		for (auto& d2 : d.second)
		{
			d2.GetRefSegment3DData().SetPosition(d2.GetSegment3DData().GetPosition() + vec);
		}
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
	for (auto& d : obbDatas)
	{
		for (auto& d2 : d.second)
		{
			d2.SetAngle(angle);
		}
	}

	for (auto& d : boardDatas)
	{
		for (auto& d2 : d.second)
		{
			d2.SetAngle(angle);
		}
	}
	for (auto& d : segment3DDatas)
	{
		for (auto& d2 : d.second)
		{
			d2.SetAngle(angle);
		}
	}
	for (auto& d : capsuleDatas)
	{
		for (auto& d2 : d.second)
		{
			d2.GetRefSegment3DData().SetAngle(angle);
		}
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
	for (auto& d : sphereDatas)
	{
		for (auto& d2 : d.second)
		{
			float setRadius = scale.x;
			if (setRadius < scale.y)setRadius = scale.y;
			if (setRadius < scale.z)setRadius = scale.z;

			d2.SetRadius(setRadius / 2);
		}
	}
	for (auto& d : boxDatas)
	{
		for (auto& d2 : d.second)
		{
			d2.SetSize(scale);
		}
	}
	for (auto& d : obbDatas)
	{
		for (auto& d2 : d.second)
		{
			d2.SetSize(scale);
		}
	}
	for (auto& d : boardDatas)
	{
		for (auto& d2 : d.second)
		{
			d2.SetSize(scale.ToVector2());
		}
	}
	for (auto& d : segment3DDatas)
	{
		for (auto& d2 : d.second)
		{
			d2.SetPosition(d2.GetPosition() * scale);
		}
	}
	for (auto& d : capsuleDatas)
	{
		for (auto& d2 : d.second)
		{
			d2.GetRefSegment3DData().SetPosition(d2.GetSegment3DData().GetPosition() * scale);
		}
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
	const ShapeType3D shapeType,
	const std::string& shapeName,
	const ShapeType3D hitObjShapeType,
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


std::shared_ptr<GameObject> MelLib::GameObject::GetNewPtr()
{
	return nullptr;
}

void MelLib::GameObject::SetPreDataPositions()
{
	for (auto& data : sphereDatas)
	{
		std::vector<Vector3>& refPrePositions = sphereDataPrePositions[data.first];

		// サイズが違ったら変更
		size_t preDataSize = refPrePositions.size();
		size_t dataSize = data.second.size();
		if (preDataSize != dataSize)refPrePositions.resize(dataSize);

		for (int i = 0; i < dataSize; i++) 
		{
			refPrePositions[i] = data.second[i].GetPosition();
		}
	}
	for (auto& data : boxDatas)
	{
		std::vector<Vector3>& refPrePositions = boxDataPrePositions[data.first];

		// サイズが違ったら変更
		size_t preDataSize = refPrePositions.size();
		size_t dataSize = data.second.size();
		if (preDataSize != dataSize)refPrePositions.resize(dataSize);

		for (int i = 0; i < dataSize; i++)
		{
			refPrePositions[i] = data.second[i].GetPosition();
		}
	}
	for (auto& data : segment3DDatas)
	{
		std::vector<Value2<Vector3>>& refPrePositions = segment3DDataPrePositions[data.first];

		// サイズが違ったら変更
		size_t preDataSize = refPrePositions.size();
		size_t dataSize = data.second.size();
		if (preDataSize != dataSize)refPrePositions.resize(dataSize);

		for (int i = 0; i < dataSize; i++)
		{
			refPrePositions[i] = data.second[i].GetPosition();
		}
	}
	for (auto& data : rayDatas)
	{
		std::vector<Vector3>& refPrePositions = rayDataPrePositions[data.first];

		// サイズが違ったら変更
		size_t preDataSize = refPrePositions.size();
		size_t dataSize = data.second.size();
		if (preDataSize != dataSize)refPrePositions.resize(dataSize);

		for (int i = 0; i < dataSize; i++)
		{
			refPrePositions[i] = data.second[i].GetPosition();
		}
	}
	for (auto& data : planeDatas)
	{
		std::vector<Vector3>& refPrePositions = planeDataPrePositions[data.first];

		// サイズが違ったら変更
		size_t preDataSize = refPrePositions.size();
		size_t dataSize = data.second.size();
		if (preDataSize != dataSize)refPrePositions.resize(dataSize);

		for (int i = 0; i < dataSize; i++)
		{
			refPrePositions[i] = data.second[i].GetPosition();
		}
	}
	for (auto& data : boardDatas)
	{
		std::vector<Vector3>& refPrePositions = boardDataPrePositions[data.first];

		// サイズが違ったら変更
		size_t preDataSize = refPrePositions.size();
		size_t dataSize = data.second.size();
		if (preDataSize != dataSize)refPrePositions.resize(dataSize);

		for (int i = 0; i < dataSize; i++)
		{
			refPrePositions[i] = data.second[i].GetPosition();
		}
	}
	for (auto& data : capsuleDatas)
	{
		std::vector<Value2<Vector3>>& refPrePositions = capsuleDataPrePositions[data.first];

		// サイズが違ったら変更
		size_t preDataSize = refPrePositions.size();
		size_t dataSize = data.second.size();
		if (preDataSize != dataSize)refPrePositions.resize(dataSize);

		for (int i = 0; i < dataSize; i++)
		{
			refPrePositions[i] = data.second[i].GetSegment3DData().GetPosition();
		}
	}
	for (auto& data : triangleDatas)
	{
		std::vector<Value3<Vector3>>& refPrePositions = triangleDataPrePositions[data.first];

		// サイズが違ったら変更
		size_t preDataSize = refPrePositions.size();
		size_t dataSize = data.second.size();
		if (preDataSize != dataSize)refPrePositions.resize(dataSize);

		for (int i = 0; i < dataSize; i++)
		{
			refPrePositions[i] = data.second[i].GetPosition();
		}
	}
	for (auto& data : obbDatas)
	{
		std::vector<Vector3>& refPrePositions = obbDataPrePositions[data.first];

		// サイズが違ったら変更
		size_t preDataSize = refPrePositions.size();
		size_t dataSize = data.second.size();
		if (preDataSize != dataSize)refPrePositions.resize(dataSize);

		for (int i = 0; i < dataSize; i++)
		{
			refPrePositions[i] = data.second[i].GetPosition();
		}
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


void GameObject::AllDraw() 
{
	for(auto& object:modelObjects)
	{
		object.second.Draw();
	}
}


unsigned int MelLib::GameObject::GetFrameHitCheckNumber(ShapeType3D type) const
{
	switch (type)
	{
	case MelLib::ShapeType3D::SPHERE:
		return sphereFrameHitCheckNum;
		break;
	case MelLib::ShapeType3D::BOX:
		return boxFrameHitCheckNum;
		break;
	case MelLib::ShapeType3D::OBB:
		return obbFrameHitCheckNum;
		break;
	case MelLib::ShapeType3D::PLANE:
		return planeFrameHitCheckNum;
		break;
	case MelLib::ShapeType3D::TRIANGLE:
		return triangleFrameHitCheckNum;
		break;
	case MelLib::ShapeType3D::BOARD:
		return boardFrameHitCheckNum;
		break;
	case MelLib::ShapeType3D::SEGMENT:
		return segment3DFrameHitCheckNum;
		break;
	case MelLib::ShapeType3D::RAY:
		return rayFrameHitCheckNum;
		break;
	case MelLib::ShapeType3D::CAPSULE:
		return capsuleFrameHitCheckNum;
		break;
	default:
		break;
	}

	return 1;
}


#ifdef _DEBUG



void MelLib::GameObject::CreateCollisionCheckModelPipelineState()
{
	DrawOption pData = PipelineState::GetDefaultDrawData(PipelineStateType::MODEL);
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

	DrawOption data = PipelineState::GetDefaultDrawData(PipelineStateType::MODEL);
	data.cullMode = CullMode::NONE;
	data.drawMode = DrawMode::WIREFRAME;

	material.Create(data,1);

}

void MelLib::GameObject::CollisionCheckModelCreateOrDelete
(
	const std::unordered_map<std::string,size_t>& datas, 
	std::unordered_map<std::string, std::vector<ModelObject>>& modelObjects,
	const ShapeType3D type
)
{
	// umapの要素数(名前の数分)ループ
	for(const auto& data: datas)
	{
		// 現在の確認用モデル数を取得
		size_t objNum = modelObjects.at(data.first).size();

		// 現在の当たり判定数を取得
		size_t dataNum = data.second;

		if (dataNum > objNum)
		{
			modelObjects[data.first].resize(dataNum);

			//不足分生成
			for (int i = objNum; i < dataNum; i++)
			{
				modelObjects[data.first][i].Create(ModelData::Get(type), nullptr);

				modelObjects[data.first][i].SetMaterial(&material);
			}
		}
		else if (dataNum < objNum)
		{
			modelObjects[data.first].resize(dataNum);
		}
	}


}

void MelLib::GameObject::CreateCollisionCheckModel()
{
	////判定数に応じてモデルを生成したり削除したりします
	//auto createOrDeleteModel = [](const size_t& dataNum, std::vector<ModelObject>& modelObjcts, const ShapeType3D type)
	//{
	//	size_t objNum = modelObjcts.size();
	//	if (dataNum > objNum)
	//	{
	//		size_t addSize = dataNum - objNum;
	//		modelObjcts.resize(dataNum);

	//		//不足分生成
	//		for (int i = objNum; i < dataNum; i++)
	//		{
	//			modelObjcts[i].Create(ModelData::Get(type), nullptr);

	//			modelObjcts[i].SetMaterial(&material);
	//		}
	//	}
	//	else if (dataNum < objNum)
	//	{
	//		modelObjcts.resize(objNum);
	//	}

	//};

	////Box
	//createOrDeleteModel(boxDatas.size(), boxModelObjects, ShapeType3D::BOX);

	////Sphere
	//createOrDeleteModel(sphereDatas.size(), sphereModelObjects, ShapeType3D::BOX);

	////Board
	//createOrDeleteModel(boardDatas.size(), boardModelObjects, ShapeType3D::BOARD);

	////Segment
	//createOrDeleteModel(segment3DDatas.size(), segmentModelObjects[0], ShapeType3D::BOX);
	//createOrDeleteModel(segment3DDatas.size(), segmentModelObjects[1], ShapeType3D::BOX);

	////Capsule
	////球を作成
	//createOrDeleteModel(capsuleDatas.size(), capsuleModelObjects[0], ShapeType3D::BOX);
	//createOrDeleteModel(capsuleDatas.size(), capsuleModelObjects[1], ShapeType3D::BOX);
	////円注を作成
	////createOrDeleteModel(capsuleData.size(), capsuleModelObjects[2], ShapeType3D::BOX);


	// 新しいやつ

}


void MelLib::GameObject::SetCollisionCheckModelData()
{
	//Box
	for (auto& data : boxModelObjects)
	{
		for (size_t i = 0; i < data.second.size(); i++)
		{
			data.second[i].SetScale(boxDatas[data.first][i].GetSize());
			data.second[i].SetPosition(boxDatas[data.first][i].GetPosition());


			/*boxModelObjects[i].SetScale(boxData[i].GetSize());
			boxModelObjects[i].SetPosition(boxData[i].GetPosition());*/
		}
	}

	//Sphere
	for (auto& data : sphereModelObjects)
	{
		for (size_t i = 0; i < data.second.size(); i++)
		{
			data.second[i].SetScale(sphereDatas[data.first][i].GetRadius() * 2);
			data.second[i].SetPosition(sphereDatas[data.first][i].GetPosition());

		}
	}

	//Board
	for (auto& data : boardModelObjects)
	{
		for (size_t i = 0; i < data.second.size(); i++)
		{
			boardModelObjects[data.first][i].SetScale(boardDatas[data.first][i].GetSize().ToVector3());
			boardModelObjects[data.first][i].SetAngle(boardDatas[data.first][i].GetAngle());
			boardModelObjects[data.first][i].SetPosition(boardDatas[data.first][i].GetPosition());
		}
	}

	//Segment
	for (auto& data : segmentModelObjects)
	{
		for (size_t i = 0; i < data.second.size(); i++)
		{
			Value2<Vector3>& lineSegmentPos = segment3DDatas[data.first][i].GetRotatePosition();


			segmentModelObjects[data.first][0][i].SetScale(1);
			segmentModelObjects[data.first][0][i].SetPosition(lineSegmentPos.v1);
			segmentModelObjects[data.first][0][i].SetAngle(segment3DDatas[data.first][i].GetAngle());

			segmentModelObjects[data.first][1][i].SetScale(1);
			segmentModelObjects[data.first][1][i].SetPosition(lineSegmentPos.v2);
			segmentModelObjects[data.first][1][i].SetAngle(segment3DDatas[data.first][i].GetAngle());
		}
	}
	
	

	//Capsule
	for (auto& data : capsuleModelObjects)
	{
		for (size_t i = 0; i < data.second.size(); i++)
		{
			Value2<Vector3>& lineSegmentPos = capsuleDatas[data.first][i].GetSegment3DData().GetRotatePosition();


			capsuleModelObjects[data.first][0][i].SetScale(1);
			capsuleModelObjects[data.first][0][i].SetPosition(lineSegmentPos.v1);
			capsuleModelObjects[data.first][0][i].SetAngle(segment3DDatas[data.first][i].GetAngle());

			capsuleModelObjects[data.first][1][i].SetScale(1);
			capsuleModelObjects[data.first][1][i].SetPosition(lineSegmentPos.v2);
			capsuleModelObjects[data.first][1][i].SetAngle(segment3DDatas[data.first][i].GetAngle());
		}
	}


}

void MelLib::GameObject::DrawCollisionCheckModel()
{
	if (!drawCollisionModel)return;

	for (auto& objects : boxModelObjects)
	{
		for (auto& obj : objects.second) 
		{
			obj.Draw();
		}
	}

	for (auto& objects : sphereModelObjects)
	{
		for (auto& obj : objects.second)
		{
			obj.Draw();
		}
	}



	for (int i = 0, size = capsuleModelObjects.size(); i < size; i++)
	{
		for (auto& objects : capsuleModelObjects)
		{
			for (auto& obj : objects.second[i])
			{
				obj.Draw();
			}
		}
	}
}

#endif // _DEBUG
