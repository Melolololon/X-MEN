#pragma once
#include <GameObject.h>
#include "Player.h"

enum class BallState
{
	NONE,
	// プレイヤーが持ってる状態
	HOLD_PLAYER,
	// 敵が・・・
	HOLD_ENEMY,
	// プレイヤーが投げたor跳ね返した
	THROWING_PLAYER,
	// 敵が・・・
	THROWING_ENEMY,
};

class Player;
class Ball
	: public MelLib::GameObject
{
	using Vector3 = MelLib::Vector3;

	//定数
public:
	//ボールにセットする色
	static const MelLib::Color BALL_COLOR_RED;		//赤
	static const MelLib::Color BALL_COLOR_BLUE;		//青
	static const MelLib::Color BALL_COLOR_YELLOW;	//黄
	
private:
	//ボールの初期スピード
	const float INIT_THROW_SPEED = 0.5f;
	const float MAX_SPEED = 1.8f;
	const float BALL_ACCEL = 0.25F;

private:
	//移動方向
	Vector3 velocity = { 0,0,0 };
	//ボールの速さ
	float speed = 0;
	//投げられた後か
	bool isThrowed = true;

	BallState throwingState;

private:

	/// <summary>
	/// 位置情報の変動関連
	/// </summary>
	void Move();

	/// <summary>
	/// モデルに色を設定する
	/// </summary>
	/// <param name="color">色情報</param>
	void SetColor(const MelLib::Color& color);

	/// <summary>
	/// 反射するときの共通処理
	/// </summary>
	/// <param name="otherNormal">衝突したポリゴン面の法線ベクトル</param>
	/// <param name="isAddSpeed">加速させるか</param>
	void Reflection(const Vector3& otherNormal, bool isAddSpeed);

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
	/// ボールを投げる
	/// </summary>
	/// <param name="initVel">ボールの発射方向 投手の移動方向を渡す</param>
	void ThrowBall(const Vector3& initVel);

	/// <summary>
	/// ボールを拾ったときの処理
	/// </summary>
	/// <param name="initPos">ボールの初期位置</param>
	/// <param name="initColor">ボールの初期位置 プレイヤーなら青、敵なら赤</param>
	void PickUp(const Vector3& ballPos, const MelLib::Color& initColor);

#pragma region Getter
	/// <summary>
	/// ボールが投げられているかを返す
	/// </summary>
	/// <returns>ボールが投げられていればtrue それ以外はfalse</returns>
	bool GetIsThrowed()const { return isThrowed; }

	/// <summary>
	/// ボールの現在の速さを返す
	/// </summary>
	/// <returns>速さ</returns>
	float GetSpeed()const { return speed; }

	/// <summary>
	/// ボールを反射、投げ、を誰が行ったかの状態を返す
	/// </summary>
	/// <returns>状態</returns>
	BallState GetThrowingState()const { return throwingState; }

#pragma endregion

#pragma region Setter
	/// <summary>
	/// ボールを反射、投げ、を誰が行ったかの状態を変更
	/// </summary>
	/// <param name="setState">変更後の状態</param>
	void SetThrowingState(BallState setState) { throwingState = setState; }
#pragma endregion
};