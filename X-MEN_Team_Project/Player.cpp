#include "Player.h"

#include<Input.h>
#include "InputDeviceManager.h"
#include"TestObject.h"
#include <GameObjectManager.h>
#include "FieldObjectWall.h"
#include <ImguiManager.h>
#include "Enemy/BarrierEnemy.h"
#include "Enemy/FollowEnemy.h"
#include "GameManager.h"

MelLib::Vector3 Player::GetInputVector()
{
	auto inputDeviceManager = InputDeviceManager::GetInstance();
	auto currentInputDevice = inputDeviceManager->GetCurrentInputDevice();
	MelLib::Vector3 moveVector;
	bool isInput = false;

	// ���݂̓��̓f�o�C�X�����ƂɈړ��x�N�g�����v�Z
	if (currentInputDevice == InputDeviceType::CONTROLLER)
	{
		moveVector = MelLib::Input::LeftStickVector3(InputDeviceInfo::STICK_START_ANGLE);
		isInput = MelLib::Input::LeftStickAngle(InputDeviceInfo::STICK_START_ANGLE) > 0 ? true : false;
	}

	if (currentInputDevice == InputDeviceType::KEYBOARD)
	{
		if (MelLib::Input::KeyState(DIK_A))
		{
			moveVector.x--;
			isInput = true;
		}
		if (MelLib::Input::KeyState(DIK_D))
		{
			moveVector.x++;
			isInput = true;
		}
		if (MelLib::Input::KeyState(DIK_W))
		{
			moveVector.z++;
			isInput = true;
		}
		if (MelLib::Input::KeyState(DIK_S))
		{
			moveVector.z--;
			isInput = true;
		}
	}

	// �ړ��L�[����͂��Ă���Ȃ�����x�N�g�����X�V
	if (isInput)dirVector = moveVector.Normalize();

	return moveVector.Normalize();
}

void Player::Move(const MelLib::Vector3& vec)
{
	static const float MOVE_SPEED = 1.0f;
	velocity += vec * MOVE_SPEED * GameManager::GetInstance()->GetGameTime();

	GameObject::AddPosition(velocity);

	oldVelocity = velocity;

	CalclateDirection();
}

void Player::CalclateDirection()
{
	const float PI = 3.141592f;
	const float MAX_ANGLE = 180.0f;

	// �����x�N�g�������Ƃɉ�]�l���v�Z
	float angleY = MAX_ANGLE / PI * std::atan2f(-dirVector.z, dirVector.x);
	GameObject::SetAngle(MelLib::Vector3(0, angleY, 0));
}

bool Player::UseBarrier(bool key)
{
	if (!key)return false;
	if (!barrier)return false;
	if (!pBall)return false;

	// �o���A���g�p���Ȃ瑦�I��
	if (barrier.get()->GetIsOpen())return false;

	barrier.get()->SetIsOpen(true);
	return true;
}

bool Player::ThrowingBall(bool key)
{
	if (!key)return false;

	if (pBall == nullptr)return false;

	//�{�[�����������Ă���ꍇ�̓��^�[��
	if (pBall->GetIsThrowed()) {
		return false;
	}

	//�{�[���𓊂���
	const float MODEL_SIZE = 4;
	pBall->SetPosition(GetPosition() + (dirVector * pBall->GetScale().x * 0.5) + (dirVector * MODEL_SIZE * 0.5));
	pBall->ThrowBall(dirVector);

	pBall.get()->SetThrowingState(BallState::THROWING_PLAYER);

	isThrowingBall = true;

	return true;
}

void Player::UseUltimateSkill(bool key)
{
	if (!key)return;

	// �X�L���g�p���Ȃ瑦�I��
	if (pUltimateSkill.get()->GetIsUsingSkill())return;

	// �X�L���g�p
	pUltimateSkill.get()->Use(GetPosition());
}

void Player::TrackingBall()
{
	if (pBall == nullptr)return;

	//�{�[�����������Ă���ꍇ�̓��^�[��
	if (pBall->GetIsThrowed()) {
		return;
	}

	MelLib::Vector3 rightVector = { -dirVector.z,0,dirVector.x };
	MelLib::Vector3 addVector = dirVector + rightVector;
	addVector /= 10;

	//�����̂�����ƉE���ɔz�u (��Ɏ����Ă�C���[�W)
	pBall->SetPosition(GetPosition() + addVector);
}

void Player::UpdateBarrierDirection()
{
	if (!barrier)return;

	barrier.get()->SetBarrierPosition(GetPosition(), dirVector);
}

void Player::UseAbility(bool key)
{
	if (!key)return;

	// �{�[�����v���C���[���ێ����Ă����Ԃ��ŕ���
	if (pBall.get()->GetThrowingState() == BallState::HOLD_PLAYER)
	{
		ThrowingBall(key);
	}
	else
	{
		UseBarrier(key);
	}
}

void Player::Knockback(const MelLib::Vector3& vector)
{
	const int POWER = 3;

	//SetPosition(GetPosition() + vector * POWER);

	knockbackPower = POWER;
	knockbackVelocity = vector * POWER;
}

void Player::UpdateIsThrowing(const float PER_FRAME)
{
	if (!isThrowingBall)return;

	if (throwingElapsedTime >= PlayerInitializeInfo::MAX_THROWING_TIME)
	{
		throwingElapsedTime = 0;
		isThrowingBall = false;
	}

	throwingElapsedTime += PER_FRAME;
}

void Player::UpdateKnockback()
{
	if (!isKnockback)return;
	// �m�b�N�o�b�N�̑��x�x�N�g���𖈃t���[���ǂꂾ�����������邩
	const float DECAY_VALUE = 2;

	MelLib::Vector3 backupDecayKnockbackVelocity = knockbackVelocity - knockbackVelocity / DECAY_VALUE;
	if (backupDecayKnockbackVelocity.Length() > knockbackVelocity.Length())
	{
		isKnockback = false;
		backupDecayKnockbackVelocity = MelLib::Vector3();
		return;
	}

	velocity += backupDecayKnockbackVelocity;
	knockbackVelocity -= backupDecayKnockbackVelocity;
}

Player::Player()
	: hp(PlayerInitializeInfo::MAX_HP)
	, ultimateSkillValue(0)
	, isThrowingBall(false)
	, throwingElapsedTime(0)
	, pUltimateSkill(nullptr)
	, dirVector(MelLib::Vector3())
	, pBall(nullptr)
	, barrier(nullptr)
	, hpGauge(GaugeUI())
{
	// MelLib;;ModelObject�̔z��
	// �l�p�`���Z�b�g
	const float SCALE = 2;
	const float MODEL_SIZE = 2 * SCALE;
	//modelObjects["main"].Create(MelLib::ModelData::Get(MelLib::ShapeType3D::BOX));
	modelObjects["main"].Create(MelLib::ModelData::Get("playerModel"));
	modelObjects["main"].SetScale(MODEL_SIZE);
	// �����ʒu��0,0,5��
	SetPosition(MelLib::Vector3(0, 0, -10));

	// �����蔻��̍쐬(��)
	// Player�̍��W���擾���A������Z�b�g
	sphereDatas["main"].resize(1);
	sphereDatas["main"][0].SetPosition(GetPosition());
	sphereDatas["main"][0].SetRadius(MODEL_SIZE * 0.5f);

	hpGauge.SetFrontColor(PlayerHPUIInfo::FRONT_COLOR);
	hpGauge.SetBackColor(PlayerHPUIInfo::BACK_COLOR);
	hpGauge.SetMaxValue(PlayerInitializeInfo::MAX_HP);
	hpGauge.SetPosition(PlayerHPUIInfo::DRAW_POSITION);
	hpGauge.SetSizePercent(PlayerHPUIInfo::SIZE_PERCENT);

	// ���G��ԊǗ��Ɠ_�ŊǗ��N���X�̏�����
	invincibleFlag.SetMaxTime(PlayerInitializeInfo::INVINCIBLE_TIME);
	flashingFlag.SetMaxTime(0.016f * 10);

	modelObjects["main"].SetAnimationPlayFlag(true);
	modelObjects["main"].SetAnimation("walk");
	modelObjects["main"].SetAnimationSpeedMagnification(3);
}

Player::~Player()
{
	pUltimateSkill = nullptr;
	pBall = nullptr;
	barrier = nullptr;

	// �Ǘ��N���X����폜
	eraseManager = true;
}

void Player::Update()
{
	bool isInputAbilityKey = MelLib::Input::KeyTrigger(DIK_SPACE) || MelLib::Input::PadButtonTrigger(MelLib::PadButton::A);
	velocity = MelLib::Vector3();

	UpdateKnockback();
	Move(GetInputVector());

	TrackingBall();
	UpdateBarrierDirection();

	pUltimateSkill.get()->Update();

	// �e�Z�������s���֐��ɑΉ������L�[�̃g���K�[�𑗂��Ċ֐����Ŏ��s���邩���f������
	UseAbility(isInputAbilityKey);
	UseUltimateSkill(MelLib::Input::KeyTrigger(DIK_Z) || MelLib::Input::PadButtonTrigger(MelLib::PadButton::X));

	if (flashingFlag.IsFlag())modelObjects["main"].SetMulColor(MelLib::Color(255, 0, 0, 255));
	else modelObjects["main"].SetMulColor(MelLib::Color(255,255,255,255));

	hpGauge.Update(hp);

	const float PER_FRAME = 1.0f / 60.0f;
	UpdateIsThrowing(PER_FRAME);

	if (invincibleFlag.IsFlag())flashingFlag.FlagOn(false);

	// �t���O�Ǘ��N���X�̍X�V
	flashingFlag.Update(PER_FRAME);
	invincibleFlag.Update(PER_FRAME);

	// �A�j���[�V�����Ȃǂ̍X�V
	modelObjects["main"].Update();
}

void Player::Draw()
{
	// ModelObjects�ɒǉ�����Ă���ModelObject�����ׂĕ`��
	AllDraw();
	// �K�E�Z�Q�[�W�̕`��
	pUltimateSkill.get()->Draw();

	hpGauge.Draw();
}

void Player::Hit
(
	const GameObject& object,
	const MelLib::ShapeType3D shapeType,
	const std::string& shapeName,
	const MelLib::ShapeType3D hitObjShapeType,
	const std::string& hitShapeName
)
{
	// �����ɓ����������̏������L�q
	// typeid�Ȃǂŏ����𕪂����肷��

	// �e�X�g�I�u�W�F�N�g�ƏՓ˂�����F�ύX
	if (typeid(object) == typeid(TestObject)) {
		modelObjects["main"].SetMulColor(MelLib::Color(100, 100, 100, 255));
	}
	else if (typeid(object) == typeid(Ball)) {
		//�X�s�[�h�擾�̂��߂Ɍ^�ϊ�
		const Ball* other = static_cast<const Ball*>(&object);

		if (!pBall)return;
		if (isThrowingBall)return;

		BallState ballThrowingState = pBall.get()->GetThrowingState();
		switch (ballThrowingState)
		{
		case BallState::NONE:
			pBall.get()->PickUp(GetPosition() + MelLib::Vector3(0.25f, 0, -0.25f), Ball::BALL_COLOR_BLUE2);
			pBall.get()->SetThrowingState(BallState::HOLD_PLAYER);
			break;
		case BallState::THROWING_PLAYER:
			pBall.get()->PickUp(GetPosition() + MelLib::Vector3(0.25f, 0, -0.25f), Ball::BALL_COLOR_BLUE2);
			pBall.get()->SetThrowingState(BallState::HOLD_PLAYER);
			break;
		case BallState::THROWING_ENEMY:

			// ���G��Ԃ���Ȃ��Ƃ�
			if (!invincibleFlag.IsFlag())
			{
				Damage(pBall.get()->GetSpeed());
				MelLib::Vector3 knockbackVector = GetPosition() - pBall.get()->GetPosition();
				Knockback(knockbackVector.Normalize());
				invincibleFlag.FlagOn();
			}
			break;
		}
	}

	// �ǂƃq�b�g�����Ƃ��̉��o����
	if (typeid(object) == typeid(FieldObjectWall))
	{
		// �q�b�g������Q���̃q�b�g�����@�������ɉ����o���������炻�̖@�����擾
		MelLib::Vector3 otherNormal = GetSphereCalcResult().GetOBBHitSurfaceNormal();
		MelLib::Vector3 pos = GetPosition() + -oldVelocity;
		SetPosition(pos);

		// �ǂ���x�N�g�����v�Z����AddPosition
		MelLib::Vector3 moveVector = oldVelocity - MelLib::Vector3Dot(oldVelocity, otherNormal) * otherNormal;
		moveVector *= oldVelocity.Length();
		AddPosition(moveVector);
	}

	if (typeid(object) == typeid(FollowEnemy) ||
		typeid(object) == typeid(BarrierEnemy))
	{
		const Enemy* other = static_cast<const Enemy*>(&object);
		MelLib::Vector3 knockbackVector = GetPosition() - other->GetPosition();

		// ���G��Ԃ���Ȃ��Ƃ�
		if (!invincibleFlag.IsFlag())
		{
			Damage(PlayerDamageInfo::HIT_ENEMY_DAMAGE);
			invincibleFlag.FlagOn();
		}

		Knockback(knockbackVector.Normalize());
	}
}

void Player::AddUltimatekillValue(int value)
{
	ultimateSkillValue += value;
}

void Player::Damage(float value)
{
	hp -= value;
}

bool Player::GetIsBarrier() const
{
	if (!barrier)return false;

	return barrier.get()->GetIsOpen();
}

bool Player::GetIsThrowingBall() const
{
	return isThrowingBall;
}

bool Player::GetIsUltimateSkill() const
{
	return pUltimateSkill.get()->GetIsUsingSkill();
}

const MelLib::Vector3& Player::GetDirection() const
{
	return dirVector;
}

float Player::GetHp() const
{
	return hp;
}

const float* Player::GetHPPointer() const
{
	return &hp;
}

void Player::SetIsBarrier(bool flag)
{
	if (!barrier)return;
	barrier.get()->SetIsOpen(flag);
}

void Player::SetIsThrowingBall(bool flag)
{
	isThrowingBall = flag;
}

void Player::SetNormalBarrier(std::shared_ptr<NormalBarrier> setBarrier)
{
	barrier = setBarrier;
}

void Player::SetBall(std::shared_ptr<Ball> setBall)
{
	pBall = setBall;
}

void Player::SetUltimateSkill(std::shared_ptr<UltimateSkill> setSkill)
{
	pUltimateSkill = setSkill;
}
