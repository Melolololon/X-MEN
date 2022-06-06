#pragma once
#include <GameObject.h>
#include "Player.h"
#include "Dome.h"

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

//ボールの軌跡表示用
class BallTrajectory
	: public MelLib::GameObject
{
	using Vector3 = MelLib::Vector3;

public:
	static const Vector3 TRAJECTORY_SCALE;

private:
	MelLib::Color color;
	bool isDisp = true;
public:

	//描画
	void Draw()override;

	//モデル生成
	void CreateModel(MelLib::ModelData* pModelData);

	//色セット
	void SetColor(const MelLib::Color& color);

	//色ゲット
	const MelLib::Color& GetColor() { return color; }

	//描画するかのフラグセット
	void SetIsDisp(bool isDisp) { this->isDisp = isDisp; }

	//描画するかのフラグゲット
	bool GetIsDisp() { return isDisp; }

};

//ボール本体
class Ball
	: public MelLib::GameObject
{
	using Vector3 = MelLib::Vector3;

	//定数
public:
	//ボールにセットする色
	static const MelLib::Color BALL_COLOR_RED;		//赤
	static const MelLib::Color BALL_COLOR_BLUE;		//青
	static const MelLib::Color BALL_COLOR_BLUE2;	//青2
	static const MelLib::Color BALL_COLOR_YELLOW;	//黄

private:
	//ボールの初期スピード
	const float INIT_THROW_SPEED = 0.5f;
	const float INIT_SCALE = 4.0f;
	const float MAX_SCALE = 10.0f;
	const float BALL_ACCEL = 1.5F;
	const float BALL_ACCEL_DOME = 1.0F;
	const float BALL_MAX_SPEED_NORMAL = 3.0f;
	const float BALL_MAX_SPEED_DOME = 8.0f;
	const float BALL_FRICTION = 0.0025f;

private:
	//移動方向
	Vector3 velocity = { 0,0,0 };
	//ボールの速さ
	float speed = 0;
	//最大スピード
	float maxSpeed = 3.0f;
	//ボールの大きさ
	float scale = INIT_SCALE;
	//投げられた後か
	bool isThrowed = true;

	BallState throwingState;

	//軌跡表示用オブジェクト
	std::shared_ptr<BallTrajectory> pBallTrajectories[60];

	//ドームのポインタ
	Dome* pDome;

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

	/// <summary>
	/// BallStateから状態に合わせた色取得
	/// </summary>
	/// <param name="ballState">BallState</param>
	const MelLib::Color GetColorFromBallState(const BallState& ballState);

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

	/// <summary>
	/// 軌跡更新
	/// </summary>
	void UpdateTrajectories();

	/// <summary>
	/// 軌跡描画
	/// </summary>
	void DrawTrajectories();

	/// <summary>
	/// ドームとの当たり判定
	/// </summary>
	/// <returns>反射したフレームはtrue、それ以外はfalse</returns>
	bool UpdateBallToDome();

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

	void SetPDome(Dome* pDome) { this->pDome = pDome; }
#pragma endregion
};