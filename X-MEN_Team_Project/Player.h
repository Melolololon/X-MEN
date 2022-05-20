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
	// window�̃T�C�Y�ɑ΂���䗦 0 ~ 100
	const MelLib::Vector2 SIZE_PERCENT = MelLib::Vector2(10, 2);

	// �`��F
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

	// �K�E�Z�Q�[�W
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
	// ���݂̓��̓f�o�C�X����󂯎�������ʂɊ�Â��ăx�N�g����Ԃ�
	// ���K�����
	MelLib::Vector3 GetInputVector();

	// �����̃x�N�g�������Ɉړ�
	void Move(const MelLib::Vector3& vec);

	// �v���C���[�������������v�Z
	void CalclateDirection();

	// �o���A�t���O���I���ɂ���
	bool UseBarrier(bool key);

	// ������t���O���I���ɂ���
	bool ThrowingBall(bool key);

	// �K�E�Z�t���O���I���ɂ���
	void UseUltimateSkill(bool key);

	// �{�[����ǔ�������
	void TrackingBall();

	// �ێ����Ă���ʏ�o���A�̕������X�V����
	void UpdateBarrierDirection();
	
	// ������o���A�֘A�̕���Ȃǂ��s��
	void UseAbility(bool key);

	void Knockback(const MelLib::Vector3& vector);

	// �{�[���𓊂����t���O���o�ߎ��ԂŊǗ�����֐�
	void UpdateIsThrowing(const float PER_FRAME);

public:
	Player();

	~Player();

	// �X�V
	void Update()override;

	//�`��
	void Draw()override;

	/// <summary>
	/// �����������ɌĂ΂��֐�
	/// </summary>
	/// <param name="object">�Փ˂����I�u�W�F�N�g�̎Q��</param>
	/// <param name="shapeType">����ƏՓ˂��������̓����蔻��̌`��</param>
	/// <param name="shapeName">����ƏՓ˂��������̓����蔻��̖��O</param>
	/// <param name="hitObjShapeType">�Փ˂����I�u�W�F�N�g�̓����蔻��̌`��</param>
	/// <param name="hitShapeName">�Փ˂����I�u�W�F�N�g�̔���̖��O</param>
	void Hit
	(
		const GameObject& object,
		const MelLib::ShapeType3D shapeType,
		const std::string& shapeName,
		const MelLib::ShapeType3D hitObjShapeType,
		const std::string& hitShapeName
	)override;

	// �K�E�Z�Q�[�W�p�̒l�ɂ�������value�����Z
	void AddUltimatekillValue(int value);

	// �����̒l���g����HP�����炷
	void Damage(float value);

#pragma region Getter
	// �o���A�t���O���擾
	bool GetIsBarrier() const;
	// �{�[���𓊂���t���O���擾
	bool GetIsThrowingBall() const;
	// �K�E�Z�t���O���擾
	bool GetIsUltimateSkill() const;
	// �v���C���[�������Ă�������̃x�N�g�����擾
	const MelLib::Vector3& GetDirection() const;
	// �v���C���[��HP���擾
	float GetHp() const;
#pragma endregion

#pragma region Setter
	// �o���A�t���O������������
	void SetIsBarrier(bool flag);
	// �{�[���𓊂���t���O������������
	void SetIsThrowingBall(bool flag);
	// �v���C���[���g�p�ł���ʏ�̃o���A���Z�b�g
	void SetNormalBarrier(std::shared_ptr<NormalBarrier> setBarrier);
	// �{�[���̃|�C���^���擾
	void SetBall(std::shared_ptr<Ball> setBall);
#pragma endregion
};

