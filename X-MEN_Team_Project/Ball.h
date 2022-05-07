#pragma once
#include <GameObject.h>

class Ball    
	: public MelLib::GameObject
{
	using Vector3 = MelLib::Vector3;

//定数
private:
	const float INIT_SPEED = 0.5f;

private:
	Vector3 velocity = { 0,0,0.1 };
	float speed = INIT_SPEED;
	bool isThrowed = false;

private:

	void Move();

public:
	Ball();
	~Ball();

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
	/// ボールを投げる プレイヤーから呼ぶこと
	/// </summary>
	/// <param name="initPos">ボールの初期位置 プレイヤーの位置をそのまま渡す</param>
	void ThrowBall(const Vector3& initPos);

	/// <summary>
	/// ボールが投げられているかを返す
	/// </summary>
	/// <returns>ボールが投げられていればtrue それ以外はfalse</returns>
	bool GetIsThrowed()const { return isThrowed; }

	/// <summary>
	/// ボールの位置を決め打ちでセットする プレイヤーがボールを持った状態時の追尾に使用
	/// </summary>
	/// <param name="pos"></param>
	void SetBallPos(const Vector3& pos);
};