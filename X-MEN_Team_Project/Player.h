#pragma once
#include "UltimateSkill.h"
#include "NormalBarrier.h"
#include<GameObject.h>
#include "Ball.h"
#include "GaugeUI.h"

namespace PlayerInitializeInfo
{
	const float MAX_HP = 10;
	const float MAX_THROWING_TIME = 0.3f;
}

namespace PlayerHPUIInfo
{
	// windowのサイズに対する比率 0 ~ 100
	const MelLib::Vector2 SIZE_PERCENT = MelLib::Vector2(10, 2);

	// 描画色
	const MelLib::Color FRONT_COLOR = MelLib::Color(255, 0, 0, 255);
	const MelLib::Color BACK_COLOR = MelLib::Color(64, 64, 64, 255);

	const MelLib::Vector2 DRAW_POSITION = MelLib::Vector2(20,100);
}

class Ball;
class Player :
	public MelLib::GameObject
{
private:
	float hp;

	// 必殺技ゲージ
	float ultimateSkillValue;

	bool isThrowingBall;
	float throwingElapsedTime;

	UltimateSkill ultimateSkill;

	MelLib::Vector3 dirVector;

	MelLib::Vector3 oldVelocity;

	std::shared_ptr<Ball> pBall;
	std::shared_ptr<NormalBarrier> barrier;

	GaugeUI hpGauge;

private:
	// 現在の入力デバイスから受け取った結果に基づいてベクトルを返す
	// 正規化状態
	MelLib::Vector3 GetInputVector();

	// 引数のベクトル方向に移動
	void Move(const MelLib::Vector3& vec);

	// プレイヤーが向く方向を計算
	void CalclateDirection();

	// バリアフラグをオンにする
	bool UseBarrier(bool key);

	// 投げるフラグをオンにする
	bool ThrowingBall(bool key);

	// 必殺技フラグをオンにする
	void UseUltimateSkill(bool key);

	// ボールを追尾させる
	void TrackingBall();

	// 保持している通常バリアの方向を更新する
	void UpdateBarrierDirection();
	
	// 投げやバリア関連の分岐などを行う
	void UseAbility(bool key);

	void Knockback(const MelLib::Vector3& vector);

	// ボールを投げたフラグを経過時間で管理する関数
	void UpdateIsThrowing(const float PER_FRAME);

public:
	Player();

	~Player();

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

	// 必殺技ゲージ用の値にを引数のvalueを加算
	void AddUltimatekillValue(int value);

	// 引数の値を使ってHPを減らす
	void Damage(float value);

#pragma region Getter
	// バリアフラグを取得
	bool GetIsBarrier() const;
	// ボールを投げるフラグを取得
	bool GetIsThrowingBall() const;
	// 必殺技フラグを取得
	bool GetIsUltimateSkill() const;
	// プレイヤーが向いている方向のベクトルを取得
	const MelLib::Vector3& GetDirection() const;
	// プレイヤーのHPを取得
	float GetHp() const;
#pragma endregion

#pragma region Setter
	// バリアフラグを書き換える
	void SetIsBarrier(bool flag);
	// ボールを投げるフラグを書き換える
	void SetIsThrowingBall(bool flag);
	// プレイヤーが使用できる通常のバリアをセット
	void SetNormalBarrier(std::shared_ptr<NormalBarrier> setBarrier);
	// ボールのポインタを取得
	void SetBall(std::shared_ptr<Ball> setBall);
#pragma endregion
};

