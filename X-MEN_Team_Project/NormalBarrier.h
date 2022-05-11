#pragma once
#include<GameObject.h>
namespace
{
	//展開できる時間
	int openTime = 60;
	//Playerとバリアの間の間隔
	float radius = 3.0f;
}
class NormalBarrier :
	public MelLib::GameObject
{
public:
	NormalBarrier();

	// 更新
	void Update()override;

	//描画
	void Draw()override;

	/// <summary>
	/// 当たった時に呼ばれる関数
	/// </summary>
	/// <param name="object">衝突したオブジェクトの参照</param>
	/// <param name="shapeType">相手と衝突した自分の当たり判定の形状</param>
	/// <param name="shapeName">相手と衝突した自分の当たり判定の名前</param>
	/// <param name="hitObjShapeType">衝突したオブジェクトの当たり判定の形状</param>
	/// <param name="hitShapeName">衝突したオブジェクトの判定の名前</param>
	void Hit
	(
		const GameObject& object,
		const MelLib::ShapeType3D shapeType,
		const std::string& shapeName,
		const MelLib::ShapeType3D hitObjShapeType,
		const std::string& hitShapeName
	)override;
	/// <summary>
	/// 指定した座標を中心にバリアを展開
	/// 移動量で角度計算
	/// </summary>
	/// <param name="positon">バリアを持たせたいキャラクターの座標</param>
	/// <param name="move">バリアを持たせたいキャラクターの移動値(velocity等)</param>
	void SetBarrierPosition(MelLib::Vector3 positon, MelLib::Vector3 move);

	//Setter
	void SetIsOpen(bool flag) { isOpen = flag; }
	//Getter
	bool GetIsOpen()const { return isOpen; }

private:
	//展開されているか
	bool isOpen;
	//寿命
	int time;
};

