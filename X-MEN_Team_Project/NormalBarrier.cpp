#include "NormalBarrier.h"

#include<Input.h>

NormalBarrier::NormalBarrier()
{
	// MelLib;;ModelObjectの配列
	// 四角形をセット
	modelObjects["main"].Create(MelLib::ModelData::Get(MelLib::ShapeType3D::BOX));
	//初期値
	const MelLib::Vector3 INIT_POS = { 0,0,5 };
	const MelLib::Vector3 INIT_SCALE = { 7,7,1 };
	SetPosition(INIT_POS);
	SetScale(INIT_SCALE);
	time = 0;
	isOpen = false;
	//sphereDatas["main"][0].SetRadius(0.5f);

	// 当たり判定の作成(OBB)
	obbDatas["main"].resize(1);
	obbDatas["main"][0].SetPosition(GetPosition());
	obbDatas["main"][0].SetSize(INIT_SCALE);
	obbDatas["main"][0].SetAngle(GetAngle());
};

void NormalBarrier::Update()
{
	//展開中なら
	if (isOpen)
	{
		//展開時間カウント
		time++;
		//指定した展開の時間経過したら
		if (time > openTime)
		{
			isOpen = false;
			time = 0;
		}
	}

	modelObjects["main"].SetMulColor(MelLib::Color(150, 150, 255, 255));
}

void NormalBarrier::Draw()
{
	//展開中のみ描画
	if (isOpen)
	{
		// ModelObjectsに追加されているModelObjectをすべて描画
		AllDraw();
	}
}

void NormalBarrier::Hit
(
	const GameObject& object,
	const MelLib::ShapeType3D shapeType,
	const std::string& shapeName,
	const MelLib::ShapeType3D hitObjShapeType,
	const std::string& hitShapeName
)
{
	// ここに当たった時の処理を記述
	// typeidなどで処理を分けたりする
}

void NormalBarrier::SetBarrierPosition(MelLib::Vector3 positon, MelLib::Vector3 move)
{
	//**************更新順番がわからなくて描画おかしくなるから常時更新にするね**************//
	////展開中なら
	//if (isOpen)
	//{
		//体の向きをmoveから求める
		float direction = atan2f(move.x, move.z);
		//バリアを展開する座標
		MelLib::Vector3 barrierPosition;
		//引数のpositionを中心に半径radius分離れた座標を求める
		barrierPosition= MelLib::Vector3(positon.x + (sinf(direction) * radius), positon.y, positon.z + (cosf(direction) * radius));
		SetPosition(barrierPosition);

		//OBBだけライブラリで勝手に反映されないっぽいのでとりあえず手動
		obbDatas["main"][0].SetPosition(GetPosition());

		//度数法→弧度法
		SetAngle(MelLib::Vector3(0.0f, direction * 57.32484076433121f, 0.0f));

		//OBBだけライブラリで勝手に反映されないっぽいのでとりあえず手動
		obbDatas["main"][0].SetAngle(GetAngle());

	//}
}
