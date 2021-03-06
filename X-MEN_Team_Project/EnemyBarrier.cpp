#include "EnemyBarrier.h"
#include "Ball.h"

#include<Input.h>


EnemyBarrier::EnemyBarrier()
{
	// MelLib;;ModelObjectの配列
	// 四角形をセット
	modelObjects["main"].Create(MelLib::ModelData::Get(MelLib::ShapeType3D::BOX));
	//初期値
	const MelLib::Vector3 INIT_POS = { 0,0,5 };
	const MelLib::Vector3 INIT_SCALE = { 5,5,1 };
	SetPosition(INIT_POS);
	SetScale(INIT_SCALE);
	currentHp = parameter::HP;
	isOpen = false;

	// 当たり判定の作成(OBB)
	obbDatas["main"].resize(1);
	obbDatas["main"][0].SetPosition(GetPosition());
	obbDatas["main"][0].SetSize(INIT_SCALE);
	obbDatas["main"][0].SetAngle(GetAngle());
}

void EnemyBarrier::Update()
{
	//展開中なら
	if (isOpen)
	{
		//耐久値が0になったら
		if (currentHp < 0.0f)
		{
			isOpen = false;
		}
	}
	modelObjects["main"].SetMulColor(MelLib::Color(255, 150, 150, 255));
}

void EnemyBarrier::Draw()
{
	//展開中のみ描画
	if (isOpen)
	{
		// ModelObjectsに追加されているModelObjectをすべて描画
		AllDraw();
	}
}

void EnemyBarrier::Hit
(
	const GameObject& object,
	const MelLib::ShapeType3D shapeType,
	const std::string& shapeName,
	const MelLib::ShapeType3D hitObjShapeType,
	const std::string& hitShapeName
)
{
	//盾が展開されていなければリターン
	if (!isOpen) { return; }

	//敵バリア&ボール
	if (typeid(object) == typeid(Ball)) {

		const Ball* other = static_cast<const Ball*>(&object);
		
		currentHp-=other->GetSpeed();
	}
}

void EnemyBarrier::SetBarrierPosition(MelLib::Vector3 position, MelLib::Vector3 move)
{
	//展開中なら
	if (isOpen)
	{
		//体の向きをmoveから求める
		float direction = atan2f(move.x, move.z);
		//バリアを展開する座標
		MelLib::Vector3 barrierPosition;
		//引数のpositionを中心に半径radius分離れた座標を求める
		barrierPosition = MelLib::Vector3(position.x + (sinf(direction) * parameter::RADIUS), position.y, position.z + (cosf(direction) * parameter::RADIUS));
		SetPosition(barrierPosition);

		//OBBだけライブラリで勝手に反映されないっぽいのでとりあえず手動
		obbDatas["main"][0].SetPosition(GetPosition());

		//度数法→弧度法
		SetAngle(MelLib::Vector3(0.0f, direction * 57.32484076433121f, 0.0f));
		//OBBだけライブラリで勝手に反映されないっぽいのでとりあえず手動
		obbDatas["main"][0].SetAngle(GetAngle());
	}
}

void EnemyBarrier::OpenBarrier()
{
	isOpen = true;
	currentHp = parameter::HP;
}
